package fr.grame.simpleguidoeditor.fragment;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import fr.grame.simpleguidoeditor.GuidoWebView;
import fr.grame.simpleguidoeditor.R;

/**
 * Fragment to draw score in svg format.
 * Implements Updatable to be able to update the fragment with new gmncode.
 */
public class GuidoSVGFragment extends Fragment implements Updatable {
	private GuidoWebView wv;
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
		View rootView = inflater.inflate(R.layout.fragment_guido_svg, container, false);
		wv = (GuidoWebView) rootView.findViewById(R.id.guido_svg);
		wv.loading(this.gmnCode);
		wv.invalidate();
		return rootView;
	}

	@Override
	public void update(String aGmnCode) {
		if (wv != null) {
			// Update score if gmn code has changed.
			if (!this.gmnCode.equals(aGmnCode)) {
				wv.loading(aGmnCode);
				wv.invalidate();
				this.gmnCode = aGmnCode;
			}
		}
	}
}