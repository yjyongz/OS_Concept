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
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				String quotes[] = new String[3];
				quotes[0] = "An ocean voyage";
				quotes[1] = "As waves break over the bow,";
				quotes[2] = "the sea welcomes me";
				pout.println(quotes[0]);
				pout.println(quotes[1]);
				pout.println(quotes[2]);
				client.close();
			}
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
