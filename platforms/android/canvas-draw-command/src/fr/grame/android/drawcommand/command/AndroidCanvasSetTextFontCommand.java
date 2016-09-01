package fr.grame.android.drawcommand.command;

import drawcommand.SetTextFontCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;
import android.graphics.Typeface;

public class AndroidCanvasSetTextFontCommand extends SetTextFontCommand implements DrawToCanvas {

	public AndroidCanvasSetTextFontCommand(String name, int size, int properties) {
		super(name, size, properties);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		// If text font is not found, default text font is used
		Typeface font;
		try {
			font = Typeface.createFromAsset(view.getContext().getAssets(), "fonts/" + _name + ".ttf");
		} catch (RuntimeException e) {
			font = Typeface.DEFAULT;
		}
		view.setTextFont(font);
		view.setTextFontSize(_size);
	}
}
