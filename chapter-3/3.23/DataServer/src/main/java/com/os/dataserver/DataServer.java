import java.net.*;
import java.io.*;
import java.util.*;

public class DataServer
{
	public static void main(String args[]) {
		try {
			ServerSocket sock = new ServerSocket(6017);
			while (true) {
				Socket client = sock.accept();
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				String quotes[] = new String[5];
				quotes[0] = "Do More of what makes you happy";
				quotes[1] = "Judge nothign, you will be happy";
				quotes[2] = "Forgive everything, you will be happier";
				quotes[3] = "Love everything, you will be happiest";
				quotes[4] = "Tomorrow never dies";
				pout.println(quotes[(int)(Math.random() * 5) % 5]);
				client.close();
			}
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
