package fr.grame.simpleguidoeditor.fragment;
 
import fr.grame.simpleguidoeditor.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
 
/**
 * Fragment to type gmn code.
 */
public class GMNTextFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_gmn_text, container, false);
        return rootView;
    }
}