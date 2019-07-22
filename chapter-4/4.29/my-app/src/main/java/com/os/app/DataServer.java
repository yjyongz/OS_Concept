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
                MultiClient mclient = new MultiClient(client);
                new Thread(mclient).start();
			}
		} catch (IOException ioe) {
			System.err.println(ioe);
		}
	}
}
