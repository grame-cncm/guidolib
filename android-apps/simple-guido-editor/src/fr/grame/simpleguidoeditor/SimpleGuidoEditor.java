package fr.grame.simpleguidoeditor;
 
import fr.grame.simpleguidoeditor.TabsPagerAdapter;
import fr.grame.simpleguidoeditor.R;
import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.view.Menu;
import android.widget.EditText;
import android.webkit.WebView;

import android.content.Context;
import android.view.inputmethod.InputMethodManager;
import android.util.Log;

import guidoengine.*;

public class SimpleGuidoEditor extends FragmentActivity implements
        ActionBar.TabListener {
 
    private ViewPager viewPager;
    private TabsPagerAdapter mAdapter;
    private ActionBar actionBar;
    // Tab titles
    private String[] tabs = { "GMN", "SVG", "Canvas" };
 
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.loadLibrary("GUIDOEngine-android");
        guido.Init("Guido2", "Times");

        setContentView(R.layout.main);
 
        // Initilization
        viewPager = (ViewPager) findViewById(R.id.pager);
        actionBar = getActionBar();
        mAdapter = new TabsPagerAdapter(getSupportFragmentManager());
 
        viewPager.setAdapter(mAdapter);
        actionBar.setHomeButtonEnabled(false);
        actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);        
 
        // Adding Tabs
        for (String tab_name : tabs) {
            actionBar.addTab(actionBar.newTab().setText(tab_name)
                    .setTabListener(this));
        }
 
        /**
         * on swiping the viewpager make respective tab selected
         * */
        viewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
 
            @Override
            public void onPageSelected(int position) {
                // on changing the page
                // make respected tab selected
                actionBar.setSelectedNavigationItem(position);
            }
 
            @Override
            public void onPageScrolled(int arg0, float arg1, int arg2) {
            }
 
            @Override
            public void onPageScrollStateChanged(int arg0) {
            }
        });
    }
 
    @Override
    public void onTabReselected(Tab tab, FragmentTransaction ft) {
    }

    @Override
    public void onTabSelected(Tab tab, FragmentTransaction ft) {
        // on tab selected
        // show respected fragment view
        Log.w("SimpleGuidoEditor", "Tab position");
        Log.w("SimpleGuidoEditor", Integer.toString(tab.getPosition()));
        int tabPosition = tab.getPosition();
        if (tabPosition == 1) {
          // ugh...kludgy in how it passes data around
          ((GuidoSVGFragment) (mAdapter.getItem(tabPosition))).generateGuidoSVG((EditText) findViewById(R.id.gmn_text),
                                                                                (WebView) findViewById(R.id.guido_svg));
        }
        viewPager.setCurrentItem(tabPosition);
        
        if (tabPosition != 0) {
          final InputMethodManager imm = (InputMethodManager) this.getSystemService(Context.INPUT_METHOD_SERVICE);
          imm.hideSoftInputFromWindow(mAdapter.getItem(tabPosition).getView().getWindowToken(), 0);
        }
        
    }
 
    @Override
    public void onTabUnselected(Tab tab, FragmentTransaction ft) {
    }
 
}