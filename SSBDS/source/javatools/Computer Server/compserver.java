import java.net.*;
import java.io.*;
import java.util.*;
import java.lang.String;

class compserver
{
	public static void main(String[] args) throws Exception
	{
		System.out.println("Server Online");

		ServerSocket serv = new ServerSocket(12345,5);
		Socket service = (Socket)null;
			
		service = serv.accept();
		
//		InetAddress ia=serv.getInetAddress();
//		System.out.println("Host Name: " + ia.getHostName());
//		System.out.println("Canonical Host Name: " + ia.getCanonicalHostName());
//		System.out.println("Host Address: " + ia.getHostAddress());
		
		InputStream  is = service.getInputStream();
		OutputStream os = service.getOutputStream();
		
		byte input[] = new byte[256];
		is.read(input);
		char message[] = new char[256];
		for(int n = 0; n < 256; n++)
		{
			char next = (char)input[n];
			if(next == '|') break;
			message[n] = next;
		}
		String str = new String(message);
		StringTokenizer st = new StringTokenizer(str, " ");
		String spritenum = st.nextToken();
		String xpos = st.nextToken();
		String ypos = st.nextToken();
		String frame = st.nextToken();
		System.out.println(spritenum + " " +  xpos + " " + ypos + " " + frame);
		byte output[] = (spritenum + " " + xpos + " " + ypos + " " + frame + "|").getBytes();
		os.write(output);
		is.close();
		os.close();
		service.close();
	}
}
