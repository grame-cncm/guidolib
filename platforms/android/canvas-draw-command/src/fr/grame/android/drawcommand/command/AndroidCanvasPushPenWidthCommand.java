package fr.grame.android.drawcommand.command;

import drawcommand.PushPenWidthCommand;
import fr.grame.android.drawcommand.DrawToCanvas;
import fr.grame.android.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;

public class AndroidCanvasPushPenWidthCommand extends PushPenWidthCommand implements DrawToCanvas {

	public AndroidCanvasPushPenWidthCommand(float width) {
		super(width);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.pushPenWidth(_width);
	}
}
