package fr.grame.simpleguidoeditor;

import fr.grame.simpleguidoeditor.fragment.Updatable;
import guidoengine.guido;
import android.app.ActionBar;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.widget.Toast;

/**
 * Main activity class. It's a FragmentActivity to manage each fragment (the
 * different tab). Implements OnGmnCodeChangeListener to have the change on the
 * gmn code.
 */
public class SimpleGuidoEditor extends FragmentActivity implements OnGmnCodeChangeListener {

	/** The viewPager is used to have the name of each tab on top of them. */
	private ViewPager viewPager;
	/** Used to manage tab and fragment */
	private TabsPagerAdapter mAdapter;
	/** Main action bar of the activity */
	private ActionBar actionBar;

	/**
	 * Gmn code typed in the application (default gmn code is provided by
	 * /SimpleGuidoEditor/res/layout/fragment_gmn_text.xml
	 */
	private static String gmnCode;

	static {
		// Load library and init guido Engine
		System.loadLibrary("GUIDOEngine-android");
		guido.Init("Guido2", "Times");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// The main view
		setContentView(R.layout.main);

		// Initilization
		viewPager = (ViewPager) findViewById(R.id.pager);
		actionBar = getActionBar();
		mAdapter = new TabsPagerAdapter(getSupportFragmentManager(), this.getApplicationContext());

		viewPager.setAdapter(mAdapter);

		actionBar.setHomeButtonEnabled(false);

		// Attach the page change listener inside the activity
		viewPager.setOnPageChangeListener(new OnPageChangeListener() {

			// This method will be invoked when a new page becomes selected.
			@Override
			public void onPageSelected(int position) {
				// Popup with the page position.
				Toast.makeText(SimpleGuidoEditor.this.getApplicationContext(), "Selected page position: " + position,
						Toast.LENGTH_SHORT).show();

				TabsPagerAdapter.Tab tabSelected = TabsPagerAdapter.Tab.getFromInt(position);

				if (tabSelected == TabsPagerAdapter.Tab.SVG || tabSelected == TabsPagerAdapter.Tab.CANVAS) {
					// This fragment must implement updatable interface
					Updatable f = (Updatable) mAdapter.getItem(position);
					// Update the view with the gmnCode.
					f.update(getGmnCode());
				}
			}

			// This method will be invoked when the current page is scrolled
			@Override
			public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
				// Code goes here
			}

			// Called when the scroll state changes:
			// SCROLL_STATE_IDLE, SCROLL_STATE_DRAGGING, SCROLL_STATE_SETTLING
			@Override
			public void onPageScrollStateChanged(int state) {
				// Code goes here
			}
		});
	}

	@Override
	public void setGmnCode(String gmncode) {
		gmnCode = gmncode;
	}

	public String getGmnCode() {
		return gmnCode;
	}
}
