/**
 * @author 8230069
 * @file SymetricCrypto.java
 * @copyright ESTG IPP
 * @brief Criptografia Aplicada, Ficha Prática 2, Exercicio 1
 * @date 2025/03/04
 **/

package ca_fp_02_1;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.InputMismatchException;
import java.util.Scanner;

public class SymetricCrypto {

	/**
	 * Generates a random Initialization Vector (IV) for cryptographic operations.
	 * The IV is always 16 bytes in size, which is required for AES encryption.
	 *
	 * @return a byte array representing the generated 16-byte initialization vector (IV)
	 */
	private static byte[] generateIV() {
		SecureRandom secureRandom = new SecureRandom();
		/*
		 * Para AES, o IV é sempre 16 bytes, seja AES-128, AES-192, AES-256
		 */
		byte[] iv = new byte[16];
		secureRandom.nextBytes(iv);
		return iv;
	}

	/**
	 * Encrypts the given input data using a specified symmetric encryption algorithm and mode with
	 * the provided key. It also appends an HMAC (Hash-based Message Authentication Code) for
	 * integrity verification.
	 *
	 * @param inputBytes the byte array of plaintext data to be encrypted
	 * @param secretKey the symmetric key used for encryption
	 * @param transformation the transformation string defining the algorithm, mode, and padding
	 *                       (e.g., "AES/CTR/NoPadding")
	 * @param hmac the secret key used to generate the HMAC for data integrity validation
	 * @return a byte array containing the encrypted data, concatenated with the initialization vector (IV)
	 *         and HMAC for authentication, or null in case of an error during encryption
	 */
	private static byte[] encrypt(byte[] inputBytes, SecretKey secretKey, String transformation, SecretKey hmac) {
		byte[] encryptedBytes = null;
		byte[] encryptedData = null;
		Cipher cipher = null;
		byte[] initialVector = generateIV();

		try {
			IvParameterSpec ivParaSpec = new IvParameterSpec(initialVector);

			cipher = Cipher.getInstance(transformation);
			cipher.init(Cipher.ENCRYPT_MODE, secretKey, ivParaSpec);

			encryptedBytes = cipher.doFinal(inputBytes);
			encryptedData = concatenate(initialVector, encryptedBytes);

			Mac mac = Mac.getInstance("HmacSHA256");
			mac.init(hmac);
			byte[] calculatedHmac = mac.doFinal(encryptedData);

			encryptedData = concatenate(encryptedData, calculatedHmac);

		} catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException |
				 InvalidAlgorithmParameterException | IllegalBlockSizeException |
				 BadPaddingException excep) {
			System.err.println(excep.getMessage());
		}
		return encryptedData;
	}

	/**
	 * Decrypts the given encrypted data using the specified symmetric key, transformation,
	 * and HMAC for validation. It extracts the Initialization Vector (IV), data, and HMAC
	 * from the input encrypted data, validates the HMAC to ensure data integrity, and then
	 * decrypts the extracted data using the provided transformation and secret key.
	 *
	 * @param encryptedData the byte array containing the encrypted data, IV, and HMAC
	 * @param secretKey the symmetric key used for decryption
	 * @param transformation the transformation string defining the algorithm, mode, and padding
	 *                       (e.g., "AES/CTR/NoPadding")
	 * @param hmac the secret key used to validate the HMAC for data integrity
	 * @return a byte array containing the decrypted plaintext data, or null if decryption fails
	 */
	private static byte[] decrypt(byte[] encryptedData, SecretKey secretKey, String transformation, SecretKey hmac) {
		byte[] extractedIV = extractIV(encryptedData);
		byte[] extractedData = extractData(encryptedData);
		byte[] extractedHMAC = extractHMAC(encryptedData);
		byte[] decryptedBytes = null;


		/*
		 * Re-inputting identical data won't compromise its cryptographic integrity.
		 *
		 * HMAC ou AES-GCM so deteta modficacao, nao "replays"
		 *
		 * Para prever um ataque "replay" deve-se implementar tokens ou timestamps
		 */
		validateHMAC(extractedIV, extractedData, extractedHMAC, hmac);

		try {
			IvParameterSpec ivParaSpec = new IvParameterSpec(extractedIV);

			Cipher cipher = Cipher.getInstance(transformation);
			cipher.init(Cipher.DECRYPT_MODE, secretKey, ivParaSpec);

			decryptedBytes = cipher.doFinal(extractedData);
		} catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException |
				 InvalidAlgorithmParameterException | IllegalBlockSizeException |
				 BadPaddingException excep) {
			System.err.println(excep.getMessage());
		}
		return decryptedBytes;
	}

	/**
	 * Extracts the Initialization Vector (IV) from the beginning of the encrypted data.
	 * The IV is assumed to be 16 bytes long, as per AES encryption standards.
	 *
	 * @param encryptedData the byte array containing the encrypted data, which includes the IV followed by the ciphertext and optional HMAC
	 * @return a 16-byte array representing the Initialization Vector (IV) extracted from the encrypted data
	 */
	private static byte[] extractIV(byte[] encryptedData) {
		byte[] iv = new byte[16];
		System.arraycopy(encryptedData, 0, iv, 0, iv.length);
		return iv;
	}

	/**
	 * Extracts the encrypted data by removing the Initialization Vector (IV)
	 * and the HMAC from the provided encrypted data array.
	 *
	 * @param encryptedData the byte array containing the IV, encrypted data,
	 *                      and HMAC to be processed
	 * @return a byte array containing only the extracted encrypted data
	 */
	private static byte[] extractData(byte[] encryptedData) {
		byte[] data = new byte[encryptedData.length - 16 - 32];
		System.arraycopy(encryptedData, 16, data, 0, data.length);
		return data;
	}

	/**
	 * Extracts the HMAC (Hash-based Message Authentication Code) from the encrypted data.
	 * The HMAC is assumed to be the last 32 bytes of the encrypted data array.
	 *
	 * @param encryptedData the byte array containing the encrypted data, which includes
	 *                      the IV, encrypted data, and appended HMAC
	 * @return a byte array containing the extracted 32-byte HMAC
	 */
	private static byte[] extractHMAC(byte[] encryptedData) {
		byte[] hmac = new byte[32];
		System.arraycopy(encryptedData, encryptedData.length - 32, hmac, 0, hmac.length);
		return hmac;
	}

	/**
	 * Validates the integrity and authenticity of provided data using HMAC (Hash-based Message
	 * Authentication Code). It calculates the HMAC of concatenated IV and data using the provided
	 * secret key, and compares it with the provided HMAC to detect tampering.
	 *
	 * @param extractedIV the byte array representing the extracted Initialization Vector (IV)
	 * @param extractedData the byte array representing the data to be authenticated
	 * @param extractedHMAC the byte array representing the HMAC extracted from the encrypted data
	 * @param hmac the secret key used to generate and validate the HMAC
	 * @throws SecurityException if the calculated HMAC does not match the provided HMAC, indicating tampering
	 */
	private static void validateHMAC(byte[] extractedIV, byte[] extractedData, byte[] extractedHMAC, SecretKey hmac) {
		try {
			byte[] dataToAuthenticate = concatenate(extractedIV, extractedData);
			Mac mac = Mac.getInstance("HmacSHA256");
			mac.init(hmac);
			byte[] recalculatedHMAC = mac.doFinal(dataToAuthenticate);

			if (!Arrays.equals(extractedHMAC, recalculatedHMAC)) {
				throw new SecurityException("DATA COMPROMISED");
			}
		} catch (InvalidKeyException | NoSuchAlgorithmException excep) {
			System.out.println(excep.getMessage());
		}
	}

	/**
	 * Concatenates two byte arrays into a single byte array.
	 *
	 * @param firstByteArray the first byte array to be concatenated
	 * @param secondByteArray the second byte array to be concatenated
	 * @return a new byte array containing the elements of the first byte array followed by the elements of the second byte array
	 */
	private static byte[] concatenate(byte[] firstByteArray, byte[] secondByteArray) {
		byte[] concatenated = new byte[firstByteArray.length + secondByteArray.length];
		System.arraycopy(firstByteArray, 0, concatenated, 0, firstByteArray.length);
		System.arraycopy(secondByteArray, 0, concatenated, firstByteArray.length, secondByteArray.length);
		return concatenated;
	}

	/**
	 * Alters the provided encrypted data by allowing user input to replace the data,
	 * and potentially decrypts it using the provided cryptographic parameters.
	 *
	 * @param encBytes the byte array containing the original encrypted data to be altered
	 * @param secretKey the symmetric key used for decryption
	 * @param transformation the transformation string defining the algorithm, mode, and padding
	 *                       (e.g., "AES/CTR/NoPadding")
	 * @param hmac the secret key used to validate the HMAC for data integrity
	 */
	private static byte[] alterData(byte[] encBytes, SecretKey secretKey, String transformation, SecretKey hmac) {
		Scanner scanner = new Scanner(System.in);
		System.out.println("Alter the data: (You can avoid altering the data by pressing ENTER) ");
		String alteredData = scanner.nextLine();
		scanner.close();

		/*
		 * Re-inputting identical data won't compromise its cryptographic integrity.
		 *
		 */
		if (alteredData.length() != 0) {
			return encrypt(alteredData.getBytes(), secretKey, transformation, hmac);


		}
		return encrypt(encBytes, secretKey, transformation,hmac);
	}

	public static void main(String[] args) throws Exception {
		// Definicao do Algoritmo, Modo e Padding
		String algModPad = "AES/CTR/NoPadding";

		// Geracao da Chave
		KeyGenerator keyGen = KeyGenerator.getInstance("AES");
		keyGen.init(256);
		SecretKey secretKey = keyGen.generateKey();

		// Geracao da Chave HMAC para garantir integridade
		keyGen = KeyGenerator.getInstance("HmacSHA256");
		keyGen.init(256);
		SecretKey hmac = keyGen.generateKey();

		// Dados a cifrar
		byte[] dataBytes = "Exemplo de teste de Cifra Simétrica em Java - Criptografia Aplicada fevereiro 2025".getBytes();
		System.out.println("Dados Originais:\n" + new String(dataBytes));

		// Cifrar
		byte[] encBytes = encrypt(dataBytes, secretKey, algModPad, hmac);
		System.out.println("\nEnc:\n" + new String(encBytes));
		System.out.println("Chave:\n" + Arrays.toString(secretKey.getEncoded()));

		// Decifrar e alterar os dados e apresentar o resultado
		Scanner input = new Scanner(System.in);
		int option = -1;
		System.out.println("---------------------");
		System.out.println("\t\t[ALTER DATA]\n[1] YES\n[2] NO");
		System.out.println("\n[9] GO BACK");
		System.out.println("[0] EXIT");
		try {
			option = input.nextInt();
		} catch (InputMismatchException e) {
			System.err.println("SYMBOLS OR LETTERS ARE NO OPTION.");
		}
		switch (option) {
			case 1:
				encBytes = alterData(encBytes, secretKey, algModPad, hmac);
				break;
			case 2:
				break;
			case 9:
				return;
			case 0:
				System.exit(1);
			default:
				System.out.println("INVALID INPUT");
				break;
		}

		// Decifrar e apresentar resultado
		byte[] decBytes = decrypt(encBytes, secretKey, algModPad, hmac);
		System.out.println("\nDecrypt:\n" + new String(decBytes));

		// Testar Cifra/Decifra
		boolean expected = java.util.Arrays.equals(dataBytes, decBytes);
		System.out.println("O Teste " + (expected ? "Funcionou!" : "FALHOU!"));
	}
}
