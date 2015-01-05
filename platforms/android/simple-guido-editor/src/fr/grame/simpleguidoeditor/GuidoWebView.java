package fr.grame.simpleguidoeditor;

import guidoengine.guidoscore;
import android.content.Context;
import android.util.AttributeSet;
import android.webkit.WebView;

/**
 * View to raw the score in svg format.
 */
public class GuidoWebView extends WebView {

  private static String gmntosvg(String gmn) {
       guidoscore gmnscore = new guidoscore();
       gmnscore.OpenParser();
       int err = gmnscore.String2AR(gmn);
       gmnscore.CloseParser();
       err = gmnscore.AR2GR();
       err = gmnscore.ResizePageToMusic();
       String out = null;
       if(err == 0)
    	   out = gmnscore.GR2SVG(1, true, "", 0);
       gmnscore.close();
       return out;
   }

  public void do_loading() {
    if (SimpleGuidoEditor._gmn != null) {
      String svg = gmntosvg(SimpleGuidoEditor._gmn);
      if (svg != null) {
        loadData(svg, "image/svg+xml", null);
        return;
      }
    }
    String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
    loadData(html, "text/html", null);
  }

  public GuidoWebView(Context context, AttributeSet attributeSet) {
    super(context, attributeSet);
  }
}
