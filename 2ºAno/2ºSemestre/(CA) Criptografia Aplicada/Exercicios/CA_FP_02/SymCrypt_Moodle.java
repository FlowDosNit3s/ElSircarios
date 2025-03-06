import java.security.NoSuchAlgorithmException;

import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.Cipher;

public class SymCrypt {
	//private static byte[] iv = { 0x0a, 0x01, 0x02, 0x03, 0x04, 0x0b, 0x0c, 0x0d };

	private static byte[] encrypt(byte[] inpBytes, SecretKey key, String algoritmoMP) throws Exception {
		Cipher cipher = ...;
	    //IvParameterSpec ips = new IvParameterSpec(iv);
	    //cipher.init(Cipher.ENCRYPT_MODE, key, ips);
		cipher.init(..., key);
	    return cipher.xxx(inpBytes);
	}

	private static byte[] decrypt(byte[] inpBytes, SecretKey key, String algoritmoMP) throws Exception {
	    Cipher cipher = ...;
	    //IvParameterSpec ips = new IvParameterSpec(iv);
	    //cipher.init(Cipher.DECRYPT_MODE, key, ips);
		cipher.init(..., key);
	    return cipher.xxx(inpBytes);
	}
	  
	public static void main(String[] args) throws Exception {
		// Definicao do Algoritmo, Modo e Padding
		String algModPad = "DES/ECB/PKCS5Padding";
		
		//Geracao da Chave
		KeyGenerator kg = KeyGenerator.xxx("DES");
		kg.init(56); // 56 is the keysize. Fixed for DES
		xxx key = kg.xxx();
		
		// Dados a cifrar
		byte[] dataBytes = "Exemplo de teste de Cifra Simétrica em Java - Criptografia Aplicada fevereiro 2025".getBytes();
		System.out.println("Dados Originais:\n" + new String(dataBytes));
		
		// Cifrar
		byte[] encBytes = encrypt(dataBytes, key, algModPad);
		System.out.println("\nEnc:\n" + new String(encBytes));
		System.out.println("Chave:\n" + key.toString());
		
		// Decifrar e apresentar resultado
		byte[] decBytes = decrypt(encBytes, key, algModPad);
		System.out.println("\nDecrypt:\n" + new String(decBytes));
		
		// Testar Cifra/Decifra
		boolean expected = java.util.Arrays.equals(dataBytes, decBytes);
		System.out.println("O Teste " + (expected ? "Funcionou!" : "FALHOU!"));
	}
	
	private static void saveToFile(String path, byte[] data) throws IOException {
		  FileOutputStream fop = new FileOutputStream(new File(path));
		  fop.write( data );
		  fop.close();
	}

	private static byte[]  readFromFile(String path) throws IOException {
		  File file2Read = new File(path);
		  byte[] retBytes = null;
		  FileInputStream fis = new FileInputStream(path);
		  retBytes = new byte[(int) file2Read.length()];
		  fis.read(retBytes);
		  fis.close();
		  return retBytes;
	}
}

