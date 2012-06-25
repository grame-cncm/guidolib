
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import guidoengine.*;

public class factorysample extends Canvas
{
	static {
        try {
			String arch = System.getProperty("os.arch");
			if (arch.equals("x86"))
				System.loadLibrary("jniGUIDOEngine");
			else
				System.loadLibrary("jniGUIDOEngine64");
			guido.Init("Guido2", "Times");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native code library failed to load.\n" + e);
		}
	}
	                                                                                               
	public guidoscore fScore;
	
	public factorysample() {
		fScore = makeScore();
	}

	public void addEvent(guidofactory f, String note, int n, int d, int accident, int octave, int dots) {
		f.OpenEvent(note);
		f.SetDuration(n, d);
		f.SetOctave(octave);
		f.SetEventDots(dots);
		f.SetEventAccidentals(accident);
		f.CloseEvent();
	}

	public void addTag(guidofactory f, String name) {
		f.OpenTag (name, 0);
		f.CloseTag();
		f.EndTag();
	}

	public void addTag(guidofactory f, String name, int param) {
		f.OpenTag (name, 0);
		f.AddTagParameterInt(param);
		f.CloseTag();
		f.EndTag();
	}

	public void addTag(guidofactory f, String name, String param) {
		f.OpenTag (name, 0);
		f.AddTagParameterString(param);
		f.CloseTag();
		f.EndTag();
	}

	public guidoscore makeScore() {
		guidofactory f = new guidofactory();
		f.Open();
		f.OpenMusic();

		f.OpenVoice();
		addTag (f, "key", "A");
		addTag (f, "meter", "C");
		addEvent(f, "g", 1, 4, 0, 1, 0);
		addEvent(f, "a", 1, 8, 0, 1, 0);

		f.OpenTag ("slur", 0);
		f.CloseTag();
		f.IsRangeTag ();
		addEvent(f, "g", 1, 4, 0, 1, 0);
		addEvent(f, "f", 1, 4, 1, 1, 1);
		addEvent(f, "c", 1, 8, 0, 2, 0);
		f.EndTag();
		f.CloseVoice();

		f.OpenVoice();
		addTag (f, "staff", 1);
		addTag (f, "stemsDown");
		addEvent(f, "c", 1, 4, 0, 1, 0);
		addEvent(f, "d", 1, 4, 0, 1, 0);
		addEvent(f, "d", 1, 4, 0, 1, 0);
		addEvent(f, "c", 1, 4, 0, 1, 0);
		f.CloseVoice();

		f.OpenVoice();
		addTag (f, "key", -4);
		addTag (f, "meter", "6/8");
		f.OpenEvent("d");
		f.AddFlat();
		f.SetDuration(1, 2);
		f.SetEventDots(3);
		f.CloseEvent();
		f.CloseVoice();
		
		guidoscore score = new guidoscore(f.CloseMusic());
		int err = score.AR2GR();
		if (err != guido.guidoNoErr)
			System.out.println("score.AR2GR error  : " + guido.GetErrorString (err));
		score.ResizePageToMusic();
		f.Close();
		return score;
	}

    public	void background_paint(Graphics g) {
		Color c1 = new Color (200,200,240);
		Color c2 = new Color (255,255,255);
		int n = 7, border=10;
		int w = (getSize().width-(2*border)) / n;
		int h = (getSize().height-(2*border)) / n;
		int x = border, y = border;
		for (int i=0; i<n*n; i++) {
			if ( (i%n) == 0 ) {
				x = border;
				y = border + (h * i/n);
			}
			g.setColor( (i & 1) == 0 ? c1 : c2 );
			g.fillRect (x, y, w, h);
			x += w;
		}
	}

    public	void paint(Graphics g) {
		background_paint (g);
		guidodrawdesc desc = new guidodrawdesc(getSize().width, getSize().height);
		int ret = fScore.Draw (g, getSize().width, getSize().height, desc, new guidopaint(), new Color(0,255,0,70));
		if (ret != guido.guidoNoErr)
			System.err.println("error drawing score: " + guido.GetErrorString(ret));
		g.setColor(new Color (255,0,0,80));
		g.fillOval (getSize().width/2-40, getSize().height/2-40, 80, 80);
	}

	
    public static void main (String args[]) {
		factorysample t = new factorysample();
		JFrame win = new JFrame("GuidoEngine Java Factory");
        win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		win.setBounds(10, 30, 500, 400);
        win.add( t );
        win.setVisible(true);
    }
}
