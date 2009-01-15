	public class hitbox
	{
		public double cx, cy, radius;
		public int frame;
		public hitbox()
		{
			frame = -1;
			cx = -1;
			cy = -1;
			radius = -1;
		}
		public hitbox(int f, double x, double y, double r)
		{
			frame = f;
			cx = x;
			cy = y;
			radius = r;
		}
		public String toString()
		{
			return frame + "\t" + cx + "\t" + cy + "\t" + radius;
		}	
		public int compareTo(Object o)
		{
			hitbox other = (hitbox)o;
			if(other.frame != frame)
			{
				if(frame > other.frame) return 1;
				else return -1;
			}
			else if(cx != other.cx)
			{
				if(cx > other.cx) return 1;
				else return -1;
			}
			else if(cy != other.cy)
			{
				if(cy > other.cy) return 1;
				else return -1;
			}
			else if(radius != other.radius)
			{
				if(radius > other.radius) return 1;
				else return -1;
			}
			else return 0;
		}
	}
