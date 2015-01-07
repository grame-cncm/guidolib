package fr.grame.simpleguidoeditor.fragment;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;
import fr.grame.simpleguidoeditor.R;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

/**
 * Fragment to draw score in a android canvas.
 * Implements Updatable to be able to update the fragment with new gmncode
 */
public class GuidoCanvasFragment extends Fragment implements Updatable {
	public GuidoCanvasView cv;

	private String gmnCode = "";

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		final InputMethodManager imm = (InputMethodManager) getActivity()
				.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(getView().getWindowToken(), 0);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

		RelativeLayout rootView = (RelativeLayout) inflater.inflate(R.layout.fragment_guido_canvas, container, false);
		cv = (GuidoCanvasView) rootView.getChildAt(0);
		cv.generateGuidoCanvas(this.gmnCode);
		cv.invalidate();
		return rootView;
	}

	@Override
	public void update(String aGmnCode) {
		if (cv != null) {
			// Update score if gmn code has changed.
			if (!this.gmnCode.equals(aGmnCode)) {
				cv.generateGuidoCanvas(aGmnCode);
				cv.invalidate();
				this.gmnCode = aGmnCode;
			}
		}
	}
}