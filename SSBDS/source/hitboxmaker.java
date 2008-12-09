   import javax.swing.*;
   import java.awt.*;
   import java.awt.event.*;	
   import java.awt.image.*;
   import java.math.*;
   import java.util.*;
   import java.io.*;
			
    public class hitboxmaker extends JPanel 
   {
      public BufferedImage myImage;
      public Graphics2D buffer; 
      public javax.swing.Timer t = new javax.swing.Timer(30, new Listener());
      public boolean[] mouse = new boolean[1];
      final static int CLICKED = 0;
      public boolean[] keys = new boolean[2];
      final static int DELETE = 0, ENTER = 1; 
      public ImageIcon img;
      public int scroll = 0;
      public double cx = -1, cy = -1, radius = -1;
      public int frame = -1;
      public ArrayList<hitbox> hitboxes = new ArrayList<hitbox>();
      public String charname;
		public int selected = -1;
  		public String atkdef = "";   	
 	
       public hitboxmaker()
      {
         charname = JOptionPane.showInputDialog("What character?");
         String imgloc = "gfx/images/" + charname + "/" + charname + ".gif";
         img = new ImageIcon(imgloc);
			while(!atkdef.equals("atk") && !atkdef.equals("def"))
			{
				atkdef = JOptionPane.showInputDialog("To edit attack boxes type 'atk'; to edit defense boxes type 'def'.");
			}
			readFile();
			
         myImage = new BufferedImage(img.getImage().getWidth(null)*2,img.getImage().getHeight(null)*2,BufferedImage.TYPE_INT_RGB);
         buffer = (Graphics2D)myImage.getGraphics();  
         addKeyListener(new KeyListener());
         addMouseListener(new MouseListener());
         addMouseWheelListener(new WheelListener());
         setFocusable(true);
         t.start();		
      }       
     
       public void draw()
      {
			boolean flag = false;
			if(keys[DELETE] && hitboxes.size() > 0 && selected != -1) 
			{
				hitboxes.remove(selected);
				selected = -1;
				keys[DELETE] = false;
			}
         if(keys[ENTER]) 
			{
				writeFile();
				keys[ENTER] = false;
			}
         if(scroll < 0) scroll = 0;
         if(scroll > img.getImage().getHeight(null)*2 - 600) scroll = img.getImage().getHeight(null)*2 - 600;
         buffer.drawImage(img.getImage(),0, 0-scroll, img.getImage().getWidth(null)*2, img.getImage().getHeight(null)*2, null);
         if(mouse[CLICKED] && cx == -1 && cy == -1) 
         {
				double clickedx = (MouseInfo.getPointerInfo().getLocation().getX() - (1280/2 -64)) /2;
           	double clickedy = ((MouseInfo.getPointerInfo().getLocation().getY() + scroll - (800/2 -300)) /2);
  				for(int n = 0; n < hitboxes.size(); n++)
				{
					hitbox nextbox = hitboxes.get(n);
					double x = clickedx - nextbox.cx;
					double y = clickedy - nextbox.cy;
					if(Math.sqrt(x*x + y*y) < nextbox.radius)
					{
						selected = n;
						flag = true;
						mouse[CLICKED] = false;
						break;
					}
				}
				if(!flag)
				{
					selected = -1;
				}
				if(mouse[CLICKED] && selected == -1)
				{
     	      	cx = (MouseInfo.getPointerInfo().getLocation().getX() - (1280/2 -64)) /2;
      	     	cy = ((MouseInfo.getPointerInfo().getLocation().getY() + scroll - (800/2 -300)) /2);
       	    	frame = (int)(((MouseInfo.getPointerInfo().getLocation().getY() + scroll - (800/2 -300)) /2) /64);
        		//				System.out.println(frame + " " + cx + " " + cy);
         	  	mouse[CLICKED] = false;
				}
			}
         if(cx != -1 && cy != -1)
         {
            buffer.setColor(Color.black);
            double dx = cx - ((MouseInfo.getPointerInfo().getLocation().getX() - (1280/2 -64)) /2);
            double dy = cy - (((MouseInfo.getPointerInfo().getLocation().getY() + scroll - (800/2 -300)) /2));
            radius = Math.sqrt(dx*dx + dy*dy);
            buffer.drawOval(2*(int)(cx - radius), 2*(int)(cy - radius)-scroll, 2*(int)(2*radius), 2*(int)(2*radius));
            if(mouse[CLICKED])
            {
               hitbox next = new hitbox(frame, cx, cy, radius);
               hitboxes.add(next);
               frame = -1;
               cx = -1;
               cy = -1;
               radius = -1;
               mouse[CLICKED] = false;
            }
         }
         repaint();
      }   
   	
       public void placeOthers()
      {
   		int n = 0;   	
			for(hitbox hit : hitboxes)
			{
				if(n != selected)
				{
					buffer.setColor(Color.black);
					buffer.fillOval(2*(int)(hit.cx -hit.radius), 2*(int)(hit.cy -hit.radius)-scroll, 2*(int)(2*hit.radius), 2*(int)(2*hit.radius));
				}
				else
				{
					buffer.setColor(Color.red);
					buffer.fillOval(2*(int)(hit.cx -hit.radius), 2*(int)(hit.cy -hit.radius)-scroll, 2*(int)(2*hit.radius), 2*(int)(2*hit.radius));
				}
				n++;
			}
      }   
      
       public void writeFile()
      {
         try
         {
            PrintWriter out = new PrintWriter(new FileWriter("gfx/Hitboxes/" + charname + "." + atkdef + ".hit"));
				PrintWriter simpleout = new PrintWriter(new FileWriter("gfx/Hitboxes/" + charname + "." + atkdef));
				for(int n = 0; n < hitboxes.size(); n++)
					for(int m = 0; m < hitboxes.size(); m++)
						if(hitboxes.get(n).compareTo(hitboxes.get(m)) < 0)
						{
							int index = m;
							hitbox temp = hitboxes.get(n);
							hitboxes.set(n, hitboxes.get(m));
							hitboxes.set(index, temp);
						}
				int lastframe = -1;
				for(int n = 0; n < hitboxes.size(); n++)
				{
					hitbox next = hitboxes.get(n);
					simpleout.println(next.frame + "\t" + next.cx + "\t" + next.cy + "\t" + next.radius);
					if(next.frame != lastframe)
					{
						if(lastframe != -1) out.print("\t\t\t\t}\n");
						out.print("\t\t\t\tif(frame == " + next.frame + ") {\n");
						lastframe = next.frame;
					}
					out.print("\t\t\t\t\ttempbox.addCircle(Circle(" + next.cx + ", " + (int)next.cy + "%64, " + next.radius + "));\n");
				}
				out.print("\t\t\t\t}\n");
            out.close();
				simpleout.close();
				JOptionPane.showMessageDialog(null, "Hitboxes saved!");
         }
             catch(Exception e) 
				 { 
				 	System.out.println("File failed to write");
				 	JOptionPane.showMessageDialog(null, "Hitboxes failed to save.");
				 }    
      }
		public void readFile()
		{
			try
			{
				Scanner sc = new Scanner(new File("gfx/Hitboxes/" + charname + "." + atkdef));
				while(sc.hasNext())
				{
					String line = sc.nextLine();
					StringTokenizer st = new StringTokenizer(line, "\t");
					int f = Integer.parseInt(st.nextToken());
					double x = Double.parseDouble(st.nextToken());
					double y = Double.parseDouble(st.nextToken());
					double r = Double.parseDouble(st.nextToken());
					hitboxes.add(new hitbox(f, x, y, r));
				}
				sc.close();
				System.out.println("File read!");
			}
			catch(Exception e) { System.out.println("File failed to read"); }
   	}
		
       private class Listener implements ActionListener
      {      
          public void actionPerformed(ActionEvent e)
         {             
            draw();
            placeOthers();
         }
      }     
   	
       public class KeyListener extends KeyAdapter
      {
          public void keyPressed(KeyEvent e)
         {
            if(e.getKeyCode()==KeyEvent.VK_BACK_SPACE)
            {
               keys[DELETE]=true;
            }
            if(e.getKeyCode()==KeyEvent.VK_ENTER)
            {
               keys[ENTER]=true;
            }
         }
      	
          public void keyReleased(KeyEvent e)
         {
            if(e.getKeyCode()==KeyEvent.VK_BACK_SPACE)
            {
               keys[DELETE]=false;
            }
            if(e.getKeyCode()==KeyEvent.VK_ENTER)
            {
               keys[ENTER]=false;
            }	       
         }
      } 
       public class MouseListener extends MouseAdapter // mouse stuff
      {
          public void mouseClicked(MouseEvent e)
         {			
            mouse[CLICKED]=true;
         }
      }
   	
       private class WheelListener implements MouseWheelListener
      {
          public void mouseWheelMoved(MouseWheelEvent e)
         {
            if(e.getWheelRotation() < 0) scroll -= 32;
            if(e.getWheelRotation() > 0) scroll += 32;
         }
      }
   
       public void paintComponent(Graphics g)
      {	
         g.drawImage(myImage, 0, 0, img.getImage().getWidth(null)*2, img.getImage().getHeight(null)*2, null);
      }		
   			
       public static void main(String[] args)
      {
         JFrame frame = new JFrame("Click to make a hitbox.");
         frame.setSize(128,600);
         frame.setLocationRelativeTo(null); 
         frame.setUndecorated(true);
         frame.setResizable(false);
         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);     
         frame.setContentPane(new hitboxmaker());
         frame.setVisible(true);
      }
   }