import java.net.*;
import java.io.*;
import java.util.*;

public class DataServer
{
	public static void main(String args[]) {
		try {
			ServerSocket sock = new ServerSocket(5575);
			while (true) {
				Socket client = sock.accept();
				PrintWriter out = new PrintWriter(client.getOutputStream(), true);
				BufferedReader bin = new BufferedReader(new InputStreamReader(client.getInputStream()));
				String line;
				while ((line = bin.readLine()) != null) {
						System.out.println(line);
						out.println(line);
				}
				client.close();
			}
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
