public class hitbox
{
	public double cx, cy, radius, kx, ky;
	public int frame, damage, length, priority;
	boolean isatkbox;	
	public hitbox(boolean atk) {
		frame = 0;
		cx = 0;
		cy = 0;
		radius = 0;
		damage = 0;
		kx = 0;
		ky = 0;
		length = 0;
		priority = 0;
		isatkbox = atk;
	}
	public hitbox(int f, double x, double y, double r) {
		frame = f;
		cx = x;
		cy = y;
		radius = r;
		isatkbox = false;
	}
	public hitbox(int f, double x, double y, double r, int d, double x2, double y2, int l, int p) {
		frame = f;
		cx = x;
		cy = y;
		radius = r;
		damage = d;
		kx = x2;
		ky = y2;
		length = l;
		priority = p;
		isatkbox = true;
	}
	public String toString() {
		if(isatkbox) return frame + "\t" + cx + "\t" + cy + "\t" + radius;
		return frame + "\t" + cx + "\t" + cy + "\t" + radius + "\t" + damage + "\t" + kx + "\t" + ky + "\t" + length + "\t" + priority;
	}
	public int compareTo(Object o) {
		hitbox other = (hitbox)o;
		if (other.frame != frame) {
			if (frame > other.frame) return 1;
			else return -1;
		}
		else if (cx != other.cx) {
			if (cx > other.cx) return 1;
			else return -1;
		}
		else if (cy != other.cy) {
			if (cy > other.cy) return 1;
			else return -1;
		}
		else if (radius != other.radius) {
			if (radius > other.radius) return 1;
			else return -1;
		}
		else return 0;
	}
}
