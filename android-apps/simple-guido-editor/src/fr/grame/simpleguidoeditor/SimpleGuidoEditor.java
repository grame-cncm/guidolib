package fr.grame.simpleguidoeditor;

//import android.util.Log;

import android.app.TabActivity;
import android.os.Bundle;
import android.content.Context;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;
import android.widget.TabHost.OnTabChangeListener;
import android.text.TextWatcher;
import android.text.Editable;
import android.view.inputmethod.InputMethodManager;
import android.view.LayoutInflater;
import android.widget.EditText;
import android.webkit.WebView;
import android.content.res.Resources;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;

import guidoengine.*;

public class SimpleGuidoEditor extends TabActivity {

    // ugh...need to learn how to make this faster...
    // it is doing a font lookup every time
    public static String gmntosvg(String gmn, String font) {
        guidoscore gmnscore = new guidoscore();
        int err = gmnscore.ParseString(gmn);
        err = gmnscore.AR2GR();
        err = gmnscore.ResizePageToMusic();
        return gmnscore.SVGExportWithFontSpec(1, "", font);
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
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.loadLibrary("GUIDOEngine-android");
        //init();
        /********/
        guido.Init("Guido2", "Times");
        /********/

        TabHost tabHost = getTabHost();
        LayoutInflater.from(this).inflate(R.layout.main, tabHost.getTabContentView(), true);

        tabHost.addTab(tabHost.newTabSpec("tab1")
                .setIndicator("GMN")
                .setContent(R.id.view1));
        tabHost.addTab(tabHost.newTabSpec("tab2")
                .setIndicator("SCORE")
                .setContent(R.id.view2));

        EditText et = (EditText) findViewById(R.id.view1);
        et.setText("[a g b a]");
        WebView wv = (WebView) findViewById(R.id.view2);
        String svg = gmntosvg("[a g b a]", getFont());
        if (svg == null) {
          String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
          wv.loadData(html, "text/html", null);
        } else {
          wv.loadData(svg, "image/svg+xml", null);
        }
        et.addTextChangedListener(new TextWatcher() {
                    // ugh...code dup...
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
                    @Override
                    public void afterTextChanged(Editable s) {}

                    @Override
                    public void beforeTextChanged(CharSequence s, int start, int count,
							int after) {}

                    @Override
                    public void onTextChanged(CharSequence s, int start, int before,
							int count) {
                      EditText et = (EditText) findViewById(R.id.view1);
	              WebView wv = (WebView) findViewById(R.id.view2);
	              getResources();
	              String svg = SimpleGuidoEditor.gmntosvg(et.getText().toString(), getFont());
	              if (svg == null) {
                        String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
                        wv.loadData(html, "text/html", null);
	              } else {
	                wv.loadData(svg, "image/svg+xml", null);
                      }
		    }

        });
        tabHost.setOnTabChangedListener(new OnTabChangeListener() {
                  @Override
                  public void onTabChanged(String tabId) {
                    if (tabId.equals("tab2")) {
                        EditText et = (EditText) findViewById(R.id.view1);
                        InputMethodManager mgr = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                        mgr.hideSoftInputFromWindow(et.getWindowToken(), 0);
                    }
                  }});
    }
    //public static native String gmntosvg(String java_gmn);
    //public static native int init();
    //public static native int shutdown();
}