package fr.grame.simpleguidoeditor;
 
import fr.grame.simpleguidoeditor.fragment.GMNTextFragment;
import fr.grame.simpleguidoeditor.fragment.GuidoSVGFragment;
import fr.grame.simpleguidoeditor.fragment.GuidoCanvasFragment;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
 
public class TabsPagerAdapter extends FragmentPagerAdapter {

    //public GMNTextFragment _cached_gmn_text_fragment;
    //public GuidoSVGFragment _cached_guido_svg_fragment;
    //public GuidoCanvasFragment _cached_guido_canvas_fragment;

    public TabsPagerAdapter(FragmentManager fm) {
        super(fm);
        //_cached_gmn_text_fragment = new GMNTextFragment();
        //_cached_guido_svg_fragment = new GuidoSVGFragment();
        //_cached_guido_canvas_fragment = new GuidoCanvasFragment();
    }
 
    @Override
    public Fragment getItem(int index) {
 
        switch (index) {
            case 0: {
                //return _cached_gmn_text_fragment;
                return new GMNTextFragment();
            }
            case 1: {
                //return _cached_guido_svg_fragment;
                return new GuidoSVGFragment();
            }
            case 2: {
                //return _cached_guido_canvas_fragment;
                return new GuidoCanvasFragment();
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