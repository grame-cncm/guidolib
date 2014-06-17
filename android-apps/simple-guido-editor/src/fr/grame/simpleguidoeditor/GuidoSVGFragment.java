package fr.grame.simpleguidoeditor;
 
import fr.grame.simpleguidoeditor.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import android.widget.EditText;
import android.webkit.WebView;
 
/******* io *******/
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
 
import guidoengine.*;

public class GuidoSVGFragment extends Fragment {
 
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

    public void generateGuidoSVG(EditText et, WebView wv) {
      String svg = gmntosvg(et.getText().toString(), getFont());
      if (svg == null) {
        String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
        wv.loadData(html, "text/html", null);
      } else {
        wv.loadData(svg, "image/svg+xml", null);
      }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
 
        View rootView = inflater.inflate(R.layout.fragment_guido_svg, container, false);
         
        return rootView;
    }
}