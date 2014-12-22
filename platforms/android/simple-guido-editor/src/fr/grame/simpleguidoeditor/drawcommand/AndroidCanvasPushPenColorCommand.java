package fr.grame.simpleguidoeditor.drawcommand;

import android.graphics.Canvas;
import android.graphics.Color;
import drawcommand.PushPenColorCommand;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasPushPenColorCommand extends PushPenColorCommand implements DrawToCanvas {

	public AndroidCanvasPushPenColorCommand(char alpha, char red, char green, char blue) {
		super(alpha, red, green, blue);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushPenColor(Color.argb(alpha, red, green, blue));
	}
}
