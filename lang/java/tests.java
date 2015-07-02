import guidoengine.guido;
import guidoengine.guidodate;
import guidoengine.guidodrawdesc;
import guidoengine.guidoelementinfo;
import guidoengine.guidopageformat;
import guidoengine.guidopaint;
import guidoengine.guidopianoroll;
import guidoengine.guidorect;
import guidoengine.guidoscore;
import guidoengine.guidosegment;
import guidoengine.mapcollector;
import guidoengine.parserError;
import guidoengine.timemapcollector;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class tests extends Canvas implements timemapcollector, mapcollector {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6919682223871582540L;

	static {
		try {
			String jnilib = "jniGUIDOEngine";
			String os = System.getProperty("os.name").toLowerCase();
			if (os.indexOf("mac") < 0) {
				String arch = System.getProperty("os.arch");
				if (!arch.equals("x86"))
					jnilib = "jniGUIDOEngine64";
			}
			System.loadLibrary(jnilib);
			guido.Init("Guido2", "Times"); // guido engine initailization is
											// required before calling any
											// guidoscore API
			if (guido.xml2gmn()) // check if musicxml support is available
				System.out.println("libMusicXML v." + guido.musicxmlversion() + " with GMN converter v."
						+ guido.musicxml2guidoversion());
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native code library failed to load.\n" + e);
		}
	}

	public guidoscore fScore;
	guidoscore testStream;
	guidopianoroll pianoRoll;
	private Graphics fCurrGraphics;

	public tests() {
		fScore = makeScore("[ \\key<\"G\"> a b f g e ]", false);
		// Piano roll Tests
		pianoRoll = new guidopianoroll();
		pianoRoll.setARHandler(fScore.getARHandler());
		pianoRoll.AR2PianoRoll(guidopianoroll.kSimplePianoRoll);
		System.out.println("pianoRoll.GetKeyboardWidth(100) = " + pianoRoll.GetKeyboardWidth(100));
		
		// Test stream
		testStream = new guidoscore();
		testStream.OpenStream();
		testStream.WriteStream("{[a b c#]");
		testStream.OpenParser();
		testStream.Stream2AR();
		testStream.CloseParser();
		testStream.AR2GR();
		String stream = testStream.GetStream();
		System.out.println("stream = " + stream);
		testStream.CloseStream();
	}

	public void Time2TimeMap(guidosegment start, guidosegment end) {
		System.out.println("Time2TimeMap : [" + start.start.fNum + "/" + start.start.fDenum + "," + start.end.fNum
				+ "/" + start.end.fDenum + "[ -> [" + end.start.fNum + "/" + end.start.fDenum + "," + end.end.fNum
				+ "/" + end.end.fDenum + "[");
	}

	public void Graph2TimeMap(guidorect box, guidosegment time, guidoelementinfo infos) {
		// System.out.println("Graph2TimeMap box: [" + box.left + ", " + box.top
		// + ", " + box.width() + ", " + box.height() + "]");
		fCurrGraphics.fillRect(box.left, box.top, box.width(), box.height());
	}

	public void misc() {
		System.out.println("\n-- misc ---");
		System.out.println("GetErrorString guidoErrParse       : " + guido.GetErrorString(guido.guidoErrParse));
		System.out.println("GetErrorString guidoErrNoMusicFont : " + guido.GetErrorString(guido.guidoErrNoMusicFont));

		// Test error on parser
		guidoscore score = new guidoscore();
		score.OpenParser();
		int err = score.String2AR("[ a b g }");
		parserError myErr = score.ParserGetErrorCode();
		System.out.println("Parser Error " + err + " on ligne " + myErr.getLigne() + " and column " + myErr.getCol()
				+ " message : " + myErr.getMessage());
		score.CloseParser();
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
		score.OpenParser();
		int err = file ? score.File2AR(gmn) : score.String2AR(gmn);
		score.CloseParser();
		if (err != guido.guidoNoErr) {
			System.out.println("score.ParseString error  : " + guido.GetErrorString(err));
			return score;
		}
		err = score.AR2GR();
		if (err != guido.guidoNoErr)
			System.out.println("score.AR2GR error  : " + guido.GetErrorString(err));
		return score;
	}

	public void score() {
		System.out.println("\n-- score ---");
		guidoscore score = makeScore("[ a b f g e ]", false);

		System.out.println("score.GetPageCount  : " + score.GetPageCount());
		guidodate dur = new guidodate();
		score.GetDuration(dur);
		System.out.println("score.GetDuration   : " + dur.fNum + "/" + dur.fDenum);

		System.out.println("score.GetSystemCount : " + score.GetSystemCount(1));

		guidodate date = new guidodate(-1, -1);
		score.GetPageDate(0, date);
		System.out.println("\nscore.GetPageDate 0  : " + date.fNum + "/" + date.fDenum);
		date = new guidodate(-1, -1);
		score.GetPageDate(1, date);
		System.out.println("score.GetPageDate 1  : " + date.fNum + "/" + date.fDenum);
		date = new guidodate(-1, -1);
		score.GetPageDate(2, date);
		System.out.println("score.GetPageDate 2  : " + date.fNum + "/" + date.fDenum);

		System.out.println("\nscore.FindPageAt 1/4  : " + score.FindPageAt(new guidodate(1, 4)));
		System.out.println("score.FindPageAt 15/1  : " + score.FindPageAt(new guidodate(15, 1)));

		System.out.println("\nscore.FindEventPage 1/4  : " + score.FindEventPage(new guidodate(1, 4)));
		System.out.println("score.FindEventPage 15/1  : " + score.FindEventPage(new guidodate(15, 1)));

		score.DrawBoundingBoxes(guidoscore.kSystemsBB + guidoscore.kStavesBB);
		System.out.println("\nscore.GetDrawBoundingBoxes (10 expected): " + score.GetDrawBoundingBoxes());

		score.DrawBoundingBoxes(0);
		System.out.println("score.GetDrawBoundingBoxes  (0 expected): " + score.GetDrawBoundingBoxes());

		System.out.println("SVG Export of : [ a b f g e ] with font :");
		String str = score.GR2SVG(1, true, null, 0);
		System.out.println(str);
		System.out.println("Abstract Export of : [ a b f g e ] with font :");
		System.out.println(score.AbstractExport(1));
		score.FreeGR();
		score.FreeAR();
	}

	public void paint(Graphics g) {
		fCurrGraphics = g;
		Toolkit.getDefaultToolkit().sync();
		Dimension d = getSize();
		// Draw a white screen
		g.setColor(Color.white);
		g.fillRect(0, 0, d.width, d.height);

		// Draw a little blue rectangle in upper left corner.
		g.setColor(Color.blue);
		g.fillRect(0, 0, 20, 20);

		guidodrawdesc desc = new guidodrawdesc();
		desc.fWidth = d.width;
		desc.fHeight = d.height;

		int m = (int) guido.Inches2Unit(30.f / 72);
		guidopageformat format = new guidopageformat(guido.Inches2Unit(d.width / 72), guido.Inches2Unit(d.height / 72),
				m, (int) guido.Inches2Unit(1.f), m, m);
		format.SetDefault();
		fScore.UpdateGR();

		// guidopaint area = new guidopaint(0,0, desc.fSizex,
		// desc.fSizey,false);
		guidopaint area = new guidopaint(0, 0, 10000, 10000);
		// guidopaint area = new guidopaint(0,0, (int)format.fWidth,
		// (int)format.fHeight, false);
		int ret = fScore.Draw(g, d.width, d.height, desc, area);
		if (ret != guido.guidoNoErr)
			System.out.println("error drawing score: " + guido.GetErrorString(ret));

		// Piano roll Tests
		pianoRoll.Draw(g, 200, 0, 400, 200, area, Color.black);

		// Test Stream
		testStream.Draw(g, 400, 0, 400, 200, desc, area, Color.black);
	}

	public void freeResources() {
		pianoRoll.DestroyPianoRoll();
		fScore.FreeGR();
		fScore.FreeAR();
		testStream.FreeGR();
		testStream.FreeAR();
		guido.shutdown();
	}

	public static void main(String args[]) {
		System.out.println("Test GUIDOEngine started... ");
		int err = guido.Init("Guido2", "Times");
		if (err != guido.guidoNoErr)
			System.out.println("guido.Init error  : " + guido.GetErrorString(err));
		final tests t = new tests();

		t.misc();
		t.pageformat();
		t.measure();
		t.score();
		// System.gc();

		Frame f = new Frame("GuidoEngine Java Tests");
		f.setBounds(10, 30, 550, 700);
		f.add(t);
		f.setVisible(true);
		f.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				t.freeResources();
				System.exit(0);
			}
		});
	}
}
