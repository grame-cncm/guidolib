package com.grame.simpleguidoeditor;

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

public class SimpleGuidoEditor extends TabActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.loadLibrary("GUIDOEngine-android");
        init();
        TabHost tabHost = getTabHost();

        LayoutInflater.from(this).inflate(R.layout.main, tabHost.getTabContentView(), true);

        tabHost.addTab(tabHost.newTabSpec("tab1")
                .setIndicator("tab1")
                .setContent(R.id.view1));
        tabHost.addTab(tabHost.newTabSpec("tab2")
                .setIndicator("tab2")
                .setContent(R.id.view2));

        EditText et = (EditText) findViewById(R.id.view1);
        et.setText("[a g b a]");
        WebView wv = (WebView) findViewById(R.id.view2);
        String svg = gmntosvg("[a g b a]");
        if (svg == null) {
          String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
          wv.loadData(html, "text/html", null);
        } else {
          wv.loadData(svg, "image/svg+xml", null);
        }
        et.addTextChangedListener(new TextWatcher() {
                    @Override
                    public void afterTextChanged(Editable s) {
                      EditText et = (EditText) findViewById(R.id.view1);
	              WebView wv = (WebView) findViewById(R.id.view2);
	              String svg = SimpleGuidoEditor.gmntosvg(et.getText().toString());
	              if (svg == null) {
                        String html = "<html><body>You have entered invalid GMN code. Please retype.</body></html>";
                        wv.loadData(html, "text/html", null);
	              } else {
	                wv.loadData(svg, "image/svg+xml", null);
                      }
		    }

                    @Override
                    public void beforeTextChanged(CharSequence s, int start, int count,
							int after) {}

                    @Override
                    public void onTextChanged(CharSequence s, int start, int before,
							int count) {}

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
    public static native String gmntosvg(String java_gmn);
    public static native int init();
    public static native int shutdown();
}