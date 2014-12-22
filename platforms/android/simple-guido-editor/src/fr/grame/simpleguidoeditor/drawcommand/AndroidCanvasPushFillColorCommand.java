package fr.grame.simpleguidoeditor.drawcommand;

import android.graphics.Canvas;
import android.graphics.Color;
import drawcommand.PushFillColorCommand;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasPushFillColorCommand extends PushFillColorCommand implements DrawToCanvas {

	public AndroidCanvasPushFillColorCommand(char alpha, char red, char green, char blue) {
		super(alpha, red, green, blue);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushFillColor(Color.argb(alpha, red, green, blue));
	}
}
