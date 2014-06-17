package fr.grame.simpleguidoeditor;
 
import fr.grame.simpleguidoeditor.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
 
public class GuidoCanvasFragment extends Fragment {
 
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
 
        View rootView = inflater.inflate(R.layout.fragment_guido_svg, container, false);
         
        return rootView;
    }
}