package fr.grame.android.drawcommand.command;

import android.graphics.Canvas;
import android.graphics.Color;
import drawcommand.PushPenCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;
import fr.grame.android.drawcommand.Pen;

public class AndroidCanvasPushPenCommand extends PushPenCommand implements DrawToCanvas {

	public AndroidCanvasPushPenCommand(char alpha, char red, char green, char blue, float width) {
		super(alpha, red, green, blue, width);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushPen(new Pen(Color.argb(alpha, red, green, blue), width));
	}

}
