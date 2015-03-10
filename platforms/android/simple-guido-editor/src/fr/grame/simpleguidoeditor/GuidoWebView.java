package fr.grame.simpleguidoeditor;

import guidoengine.guido;
import guidoengine.guidoscorebase;
import android.content.Context;
import android.util.AttributeSet;
import android.webkit.WebView;

/**
 * View to raw the score in svg format.
 */
public class GuidoWebView extends WebView {

	public GuidoWebView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	/**
	 * Transform gmn code into svg string
	 * 
	 * @param gmn
	 *            the gmn code
	 * @return the svg code.
	 */
	private static String gmnToSvg(String gmn) {
		guidoscorebase gmnscore = new guidoscorebase();
		// Create an Abstract representation with gmn code
		gmnscore.OpenParser();
		int err = gmnscore.String2AR(gmn);
		gmnscore.CloseParser();
		// Create a graphic representation with Abstract representation
		if (err == guido.guidoNoErr)
			err = gmnscore.AR2GR();
		if (err == guido.guidoNoErr)
			err = gmnscore.ResizePageToMusic();
		String out = null;
		if (err == guido.guidoNoErr)
			// Create SVG
			out = gmnscore.GR2SVG(1, true, "", 0);
		// Free all resource (abstract and graphic representation)
		gmnscore.close();
		return out;
	}

	/**
	 * Convert gmncode to svg and add error message if the svg is not generated.
	 * 
	 * @param gmnCode
	 */
	public void loading(String gmnCode) {
		if (gmnCode != null) {
			String svg = gmnToSvg(gmnCode);
			if (svg != null) {
				loadData(svg, "image/svg+xml", null);
				return;
			}
		}
		String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
		loadData(html, "text/html", null);
	}
}
