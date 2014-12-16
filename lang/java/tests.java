
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import guidoengine.*;

public class tests extends Canvas implements timemapcollector, mapcollector
{
	static {
        try {
			System.loadLibrary("jniGUIDOEngine");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native code library failed to load.\n" + e);
		}
	}
	                                                                                               
	public guidoscore fScore;
    private Graphics fCurrGraphics;
	
	public tests() {
		fScore = makeScore("[ \\key<\"G\"> a b f g e ]", false);
//		fScore = makeScore("/Users/fober/Desktop/repeats.gmn", true);
//		fScore = makeScore("/Users/fober/src/guido/svn/guidosf/branches/mapping/gmn-examples/complex_files/bach.gmn", true);
	}

	public void Time2TimeMap( guidosegment start, guidosegment end) {
        System.out.println("Time2TimeMap : [" + start.start.fNum + "/" + start.start.fDenum + "," 
			+ start.end.fNum + "/" + start.end.fDenum + "[ -> [" + end.start.fNum + "/" + end.start.fDenum + "," 
			+ end.end.fNum + "/" + end.end.fDenum + "[");
	}

	public void Graph2TimeMap( guidorect box, guidosegment time, guidoelementinfo infos ) {
      //  System.out.println("Graph2TimeMap box: [" + box.left + ", " +  box.top + ", " + box.width() + ", " + box.height() + "]");
		fCurrGraphics.fillRect (box.left, box.top, box.width(), box.height());
	}

	public void misc() {
        System.out.println("\n-- misc ---");
        System.out.println("GetErrorString guidoErrParse       : " + guido.GetErrorString (guido.guidoErrParse));
        System.out.println("GetErrorString guidoErrNoMusicFont : " + guido.GetErrorString (guido.guidoErrNoMusicFont));
        System.out.println("GetParseErrorLine : " + guido.GetParseErrorLine ());
	}

	public void measure() {
        System.out.println("\n-- measure ---");
        System.out.println("GetLineSpace : " + guido.GetLineSpace());
        System.out.println("Unit2CM      : " + guido.Unit2CM(1.f));
        System.out.println("CM2Unit      : " + guido.CM2Unit(1.f));
        System.out.println("Unit2Inches  : " + guido.Unit2Inches(1.f));
        System.out.println("Inches2Unit  : " + guido.Inches2Unit(1.f));
	}

	public void pageformat() {
        System.out.println("\n-- pageformat ---");
		guidopageformat pf = new guidopageformat();
        System.out.println("new guidopageformat()");
		pf.print();
        System.out.println("get default pageformat");
		pf.GetDefault();
        System.out.println("width in cm     : " + guido.Unit2CM(pf.fWidth));
        System.out.println("width in Inches : " + guido.Unit2Inches(pf.fWidth));
		pf.print();
	}

	public guidoscore makeScore(String gmn, boolean file) {
		guidoscore score = new guidoscore();
		int err = file ? score.ParseFile(gmn) : score.ParseString(gmn);
		if (err != guido.guidoNoErr) {
			System.out.println("score.ParseString error  : " + guido.GetErrorString (err));
			return score;
		}
		err = score.AR2GR();
		if (err != guido.guidoNoErr)
			System.out.println("score.AR2GR error  : " + guido.GetErrorString (err));
		return score;
	}

	public void score() {
        System.out.println("\n-- score ---");
		guidoscore score = makeScore("[ a b f g e ]", false);

		System.out.println("score.GetPageCount  : " + score.GetPageCount ());
		guidodate dur = new guidodate();
		score.GetDuration (dur);
		System.out.println("score.GetDuration   : " + dur.fNum + "/" + dur.fDenum);
		
		guidodate date = new guidodate(-1,-1);
		score.GetPageDate (0, date);
		System.out.println("\nscore.GetPageDate 0  : " + date.fNum + "/" + date.fDenum);
		date = new guidodate(-1,-1); score.GetPageDate (1, date);
		System.out.println("score.GetPageDate 1  : " + date.fNum + "/" + date.fDenum);
		date = new guidodate(-1,-1); score.GetPageDate (2, date);
		System.out.println("score.GetPageDate 2  : " + date.fNum + "/" + date.fDenum);

		System.out.println("\nscore.FindPageAt 1/4  : " + score.FindPageAt (new guidodate(1,4)));
		System.out.println("score.FindPageAt 15/1  : " + score.FindPageAt (new guidodate(15,1)));

		System.out.println("\nscore.FindEventPage 1/4  : " + score.FindEventPage (new guidodate(1,4)));
		System.out.println("score.FindEventPage 15/1  : " + score.FindEventPage (new guidodate(15,1)));

		score.DrawBoundingBoxes (score.kSystemsBB + score.kStavesBB);
		System.out.println("\nscore.GetDrawBoundingBoxes (10 expected): " + score.GetDrawBoundingBoxes ());
		score.DrawBoundingBoxes (0);
		System.out.println("score.GetDrawBoundingBoxes  (0 expected): " + score.GetDrawBoundingBoxes ());
	}

    public	void paint(Graphics g) {
		fCurrGraphics = g;
		Toolkit.getDefaultToolkit().sync();
		Dimension d = getSize();
		g.setColor(Color.white);
		g.fillRect (0, 0, d.width, d.height);
		g.setColor(Color.blue);
		g.fillRect (0, 0, 20, 20);

		guidodrawdesc desc = new guidodrawdesc();
		desc.fSizex = d.width;
		desc.fSizey = d.height;
		
		int m = (int)guido.Inches2Unit(30.f/72);
		guidopageformat format = new guidopageformat(guido.Inches2Unit(d.width/72), guido.Inches2Unit(d.height/72), m, (int)guido.Inches2Unit(1.f),m,m);
		format.SetDefault();
		fScore.UpdateGR();
		
//		guidopaint area = new guidopaint(0,0, desc.fSizex, desc.fSizey,false);
		guidopaint area = new guidopaint(0,0, 10000, 10000,false);
//		guidopaint area = new guidopaint(0,0, (int)format.fWidth, (int)format.fHeight, false);
		int ret = fScore.Draw (this, g, desc, area);
		if (ret != guido.guidoNoErr)
			System.out.println("error drawing score: " + guido.GetErrorString(ret));
		
//		fScore.GetTimeMap (this);
//		Color c = new Color(100, 0, 0, 100);
//		fCurrGraphics.setColor(c);
//		fScore.GetMap (1, d.width, d.height, guidoscoremap.kGuidoEvent, this);
//		System.gc();
	}

	
    public static void main (String args[]) {
        System.out.println("Test GUIDOEngine started... ");
		int err = guido.Init("Guido2", "Times");
		if (err != guido.guidoNoErr)
			System.out.println("guido.Init error  : " + guido.GetErrorString (err));
		tests t = new tests();

//		t.misc();
//		t.pageformat();
//		t.measure();
//		t.score();
//		System.gc();

		Frame f = new Frame("GuidoEngine Java Tests");
        f.setBounds(10, 30, 550, 700);
        f.add( t );
        f.setVisible(true);
		f.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				System.exit(0);
			}
		});
    }
}
