package fr.grame.simpleguidoeditor;

import guidoengine.*;

import java.util.*;
import android.util.Log;

import android.graphics.Rect;

import android.graphics.drawable.ShapeDrawable;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;

import android.webkit.WebView;

public class GuidoWebView extends WebView {

  private static String gmntosvg(String gmn, String font) {
       guidoscore gmnscore = new guidoscore();
       int err = gmnscore.ParseString(gmn);
       err = gmnscore.AR2GR();
       err = gmnscore.ResizePageToMusic();
       String out = gmnscore.SVGExportWithFontSpec(1, "", font);
       gmnscore.close();
       return out;
   }

   private String getFont() {
       BufferedReader temp_br = new BufferedReader(new InputStreamReader(getResources().openRawResource(R.raw.guido2)));
       StringBuilder temp_total = new StringBuilder();
       String temp_line;
       try {
         while ((temp_line = temp_br.readLine()) != null) {
             temp_total.append(temp_line);
         }
       }
       catch (IOException e) { // do nothing
       }
       return temp_total.toString();
   }

  public void do_loading() {
    if (SimpleGuidoEditor._gmn != null) {
      String svg = gmntosvg(SimpleGuidoEditor._gmn, getFont());
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
