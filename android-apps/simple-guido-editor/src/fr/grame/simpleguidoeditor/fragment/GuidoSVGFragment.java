package fr.grame.simpleguidoeditor.fragment;

import android.view.inputmethod.InputMethodManager;
import android.content.Context;

 
import fr.grame.simpleguidoeditor.GuidoWebView;
import fr.grame.simpleguidoeditor.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import android.util.Log;
 
public class GuidoSVGFragment extends Fragment {

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        final InputMethodManager imm = (InputMethodManager) getActivity().getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(getView().getWindowToken(), 0);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_guido_svg, container, false);
        GuidoWebView wv = (GuidoWebView) rootView.findViewById(R.id.guido_svg);
        wv.do_loading();
        wv.invalidate();
        return rootView;
    }
}