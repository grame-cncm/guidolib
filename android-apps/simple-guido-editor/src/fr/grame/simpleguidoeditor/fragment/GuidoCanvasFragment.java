package fr.grame.simpleguidoeditor.fragment;
 
import fr.grame.simpleguidoeditor.R;

import android.view.inputmethod.InputMethodManager;
import android.content.Context;

import fr.grame.simpleguidoeditor.GuidoCanvasView;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import android.widget.RelativeLayout;

public class GuidoCanvasFragment extends Fragment {

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        final InputMethodManager imm = (InputMethodManager) getActivity().getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(getView().getWindowToken(), 0);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
 
        RelativeLayout rootView = (RelativeLayout) inflater.inflate(R.layout.fragment_guido_canvas, container, false);
        GuidoCanvasView cv = (GuidoCanvasView) rootView.getChildAt(0);
        cv.generateGuidoCanvas();
        cv.invalidate();
        return rootView;
    }
}