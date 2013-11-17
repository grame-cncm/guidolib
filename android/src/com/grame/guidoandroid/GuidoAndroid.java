package com.grame.guidoandroid;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;

public class GuidoAndroid extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        WebView webview = new WebView(this);

        System.loadLibrary("GUIDOEngine-android");
        init();
        String svg = gmntosvg("[a]");
        /*
        String svg = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"
           + "<circle cx=\"100\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />"
           + "</svg>";
        */
        if (svg == null) {
          svg = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"
           + "<circle cx=\"100\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />"
           + "</svg>";
        }
        webview.loadData(svg, "image/svg+xml", null);
        setContentView(webview);
    }
    public native String gmntosvg(String java_gmn);
    public native int init();
    public native int shutdown();
}
