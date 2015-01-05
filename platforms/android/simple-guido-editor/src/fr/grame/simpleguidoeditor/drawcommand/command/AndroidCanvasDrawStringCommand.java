package fr.grame.simpleguidoeditor.drawcommand.command;

import drawcommand.DrawStringCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import fr.grame.simpleguidoeditor.drawcommand.GuidoCanvasView;

import android.graphics.Canvas;

import android.graphics.Paint;

public class AndroidCanvasDrawStringCommand extends DrawStringCommand implements DrawToCanvas {

	public AndroidCanvasDrawStringCommand(float x, float y, String s, int inCharCount) {
		super(x, y, s, inCharCount);
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.correctTransformMatrix(canvas);

		Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
		paint.setTypeface(view.getTextFont());
		paint.setColor(view.getFontColor());
		paint.setTextSize(view.getTextFontSize());

		canvas.drawText(_s, _x, _y, paint);
		view.resetTransformMatrix(canvas);
	}

}
