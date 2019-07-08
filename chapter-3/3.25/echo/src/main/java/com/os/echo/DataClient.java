import java.net.*;
import java.io.*;

public class DataClient
{
	public static void main(String args[]) {
		try {
			Socket sock = new Socket("127.0.0.1", 5575);
			PrintWriter 	out   = new PrintWriter(sock.getOutputStream(), true);
			BufferedReader  bin   = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			BufferedReader  input = new BufferedReader(new InputStreamReader(System.in));
			String line;
			while ((line = input.readLine()) != null) {
					out.println(line);
					System.out.println("echo: " + bin.readLine());
			}
			sock.close();
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
