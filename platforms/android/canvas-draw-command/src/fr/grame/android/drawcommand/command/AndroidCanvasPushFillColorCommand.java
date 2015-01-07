package fr.grame.android.drawcommand.command;

import android.graphics.Canvas;
import android.graphics.Color;
import drawcommand.PushFillColorCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

public class AndroidCanvasPushFillColorCommand extends PushFillColorCommand implements DrawToCanvas {

	public AndroidCanvasPushFillColorCommand(char alpha, char red, char green, char blue) {
		super(alpha, red, green, blue);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushFillColor(Color.argb(alpha, red, green, blue));
	}
}
