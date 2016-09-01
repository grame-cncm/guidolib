package fr.grame.simpleguidoeditor;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import fr.grame.simpleguidoeditor.fragment.GMNTextFragment;
import fr.grame.simpleguidoeditor.fragment.GuidoCanvasFragment;
import fr.grame.simpleguidoeditor.fragment.GuidoSVGFragment;

/**
 * Manage Fragment and tab.
 *
 */
public class TabsPagerAdapter extends FragmentPagerAdapter {

	/**
	 * Enum to manage tab.
	 */
	public enum Tab {
		GMNCODE(0), SVG(1), CANVAS(2);

		@SuppressWarnings("unused")
		private int value;

		private Tab(int value) {
			this.value = value;
		}

		public static Tab getFromInt(int value) {
			// here return the appropriate enum constant
			if (value < Tab.values().length)
				return Tab.values()[value];
			return null;
		}
	}

	private Context context;
	private FragmentManager fragmentManager;

	public TabsPagerAdapter(FragmentManager fm, Context context) {
		super(fm);
		this.context = context;
		fragmentManager = fm;
	}

	@Override
	public Fragment getItem(int index) {
		// Try to find an existing instance of the fragment
		Fragment f = this.fragmentManager.findFragmentByTag(makeFragmentName(index));
		if (f != null)
			return f;

		// Create a new fragment
		Tab tab = Tab.getFromInt(index);
		switch (tab) {
		case GMNCODE:
			return new GMNTextFragment();
		case SVG:
			return new GuidoSVGFragment();
		case CANVAS:
			return new GuidoCanvasFragment();
		}

		return null;
	}

	/**
	 * Get tag name of a fragment. With a FragmentPagerAdapter, the fragment
	 * have not this id but all the fragment have the id of the pager.
	 * @param index index of the tab.
	 * @return the fragment tag name.
	 */
	private static String makeFragmentName(int index) {
		return "android:switcher:" + R.id.pager + ":" + index;
	}

	@Override
	public int getCount() {
		// get item count - equal to number of tabs
		return 3;
	}

	@Override
	public CharSequence getPageTitle(int position) {
		switch (Tab.getFromInt(position)) {
		case GMNCODE:
			return context.getString(R.string.tab_1);
		case SVG:
			return context.getString(R.string.tab_2);
		case CANVAS:
			return context.getString(R.string.tab_3);
		}
		return null;
	}
}