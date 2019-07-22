import java.net.*;
import java.io.*;
import java.util.*;

public class MultiClient implements Runnable 
{
    private Socket sock;
    public MultiClient(Socket sock) {
        this.sock = sock;
    }
    public void run()
    {
        try {
            Socket client = this.sock;
		    PrintWriter out = new PrintWriter(client.getOutputStream(), true);
            BufferedReader bin = new BufferedReader(new InputStreamReader(client.getInputStream()));
            String line;
            while ((line = bin.readLine()) != null) {
                    System.out.println(line);
                    out.println(line);
            }
            client.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
