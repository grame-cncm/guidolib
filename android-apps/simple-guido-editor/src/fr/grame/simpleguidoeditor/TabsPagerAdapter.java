package fr.grame.simpleguidoeditor;
 
import fr.grame.simpleguidoeditor.GMNTextFragment;
import fr.grame.simpleguidoeditor.GuidoSVGFragment;
import fr.grame.simpleguidoeditor.GuidoCanvasFragment;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;

import android.content.Context;
 
public class TabsPagerAdapter extends FragmentPagerAdapter {

    private GMNTextFragment cached_gmn_text_fragment;
    private GuidoSVGFragment cached_guido_svg_fragment;
    private GuidoCanvasFragment cached_guido_canvas_fragment;

    public TabsPagerAdapter(FragmentManager fm) {
        super(fm);
        //Log.w("SimpleGuidoEditor", "pager adapter constructor");
        // GMN text fragment activity
        cached_gmn_text_fragment = new GMNTextFragment();
        // Guido SVG activity
        cached_guido_svg_fragment =  new GuidoSVGFragment();
        // GuidoCanvas activity
        cached_guido_canvas_fragment = new GuidoCanvasFragment();
        
    }
 
    @Override
    public Fragment getItem(int index) {
 
        switch (index) {
            case 0: {
                return cached_gmn_text_fragment;
            }
            case 1: {
                return cached_guido_svg_fragment;
            }
            case 2: {
                return cached_guido_canvas_fragment;
            }
        }
 
        return null;
    }
 
    @Override
    public int getCount() {
        // get item count - equal to number of tabs
        return 3;
    }
 
}