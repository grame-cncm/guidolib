package fr.grame.simpleguidoeditor;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import fr.grame.simpleguidoeditor.drawcommand.Pen;
import guidoengine.guidobinaryparser;
import guidoengine.guidoscore;

public class GuidoCanvasView extends View {
	private AndroidGuidoCommandBattery _commandBattery;
	public double _CHAR_MAX;
	public double _GLOBAL_RESCALE_FACTOR;
	public Matrix _CURRENT_TRANSFORM_MATRIX;
	public int _FONT_COLOR;
	public Typeface _MUSIC_FONT;
	public int _MUSIC_FONT_SIZE;
	public Typeface _TEXT_FONT;
	public int _TEXT_FONT_SIZE;

	private List<Integer> _FILL_COLORS;
	private int currentFillColor;

	private List<Pen> _PENS;

	private List<Float> _PEN_WIDTHS;
	private float currentPenWidth;

	private List<Integer> _PEN_COLORS;
	private int currentPenColor;

	public void correctTransformMatrix(Canvas canvas) {
		canvas.save();
		canvas.scale((float) _GLOBAL_RESCALE_FACTOR, (float) _GLOBAL_RESCALE_FACTOR);
	}

	public void resetTransformMatrix(Canvas canvas) {
		canvas.restore();
	}

	public void do_inits() {
		_commandBattery = new AndroidGuidoCommandBattery();
		_CHAR_MAX = 255.0;
		_GLOBAL_RESCALE_FACTOR = 1.0;
		_CURRENT_TRANSFORM_MATRIX = new Matrix();
		_PEN_COLORS = new ArrayList<Integer>();
		pushPenColor(Color.BLACK);
		_FONT_COLOR = Color.BLACK;
		_PEN_WIDTHS = new ArrayList<Float>();
		pushPenWidth(1);
		_MUSIC_FONT = Typeface.create("Guido2", Typeface.NORMAL);
		_MUSIC_FONT_SIZE = 12;
		_TEXT_FONT = Typeface.create("Times", Typeface.NORMAL);
		_TEXT_FONT_SIZE = 12;
		_FILL_COLORS = new ArrayList<Integer>();
		pushFillColor(Color.BLACK);
		_PENS = new ArrayList<Pen>();
		pushPen(new Pen(Color.BLACK, 1));
	}

	public void print_matrix_coords(Canvas canvas) {
		float[] matrix = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		canvas.getMatrix().getValues(matrix);
		StringBuilder sb = new StringBuilder();
		sb.append("current matrix:");
		for (int i = 0; i < 9; i++) {
			sb.append(" ");
			sb.append(matrix[i]);
		}
		Log.i("SimpleGuidoEditor", sb.toString());
	}

	private static byte[] gmntobinary(String gmn) {
		guidoscore gmnscore = new guidoscore();
		gmnscore.OpenParser();
		int err = gmnscore.String2AR(gmn);
		gmnscore.CloseParser();
		err = gmnscore.AR2GR();
		err = gmnscore.ResizePageToMusic();
		byte[] out = gmnscore.BinaryExport(1);
		gmnscore.close();
		return out;
	}

	public void generateGuidoCanvas() {
		if (SimpleGuidoEditor._gmn != null) {
			byte[] binary = gmntobinary(SimpleGuidoEditor._gmn);
			if (binary.length > 0) {
				_commandBattery.flush();
				guidobinaryparser.parseIntoDrawCommands(binary, _commandBattery);
			}
		}
	}

	public GuidoCanvasView(Context context, AttributeSet attributeSet) {
		super(context);
		do_inits();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		for (int i = 0; i < _commandBattery.drawCommands.size(); i++) {
			// Log.i("SimpleGuidoEditor", _commandBattery.get(i).asString());
			_commandBattery.drawCommands.get(i).drawToCanvas(canvas, this);
		}
	}

	public void popFillColor() {
		this._FILL_COLORS.remove(this._FILL_COLORS.size() - 1);
		this.currentFillColor = this._FILL_COLORS.get(this._FILL_COLORS.size() - 1);
	}

	public void pushFillColor(int color) {
		this._FILL_COLORS.add(color);
		this.currentFillColor = color;
	}

	public void popPenColor() {
		this._PEN_COLORS.remove(this._PEN_COLORS.size() - 1);
		this.currentPenColor = this._PEN_COLORS.get(this._PEN_COLORS.size() - 1);
	}

	public void pushPenColor(int color) {
		this._PEN_COLORS.add(color);
		this.currentPenColor = color;
	}

	public void popPenWidth() {
		this._PEN_WIDTHS.remove(this._PEN_WIDTHS.size() - 1);
		this.currentPenWidth = this._PEN_WIDTHS.get(_PEN_WIDTHS.size() - 1);
	}

	public void pushPenWidth(float width) {
		this._PEN_WIDTHS.add(width);
		this.currentPenWidth = width;
	}

	public void pushPen(Pen pen) {
		this.currentPenColor = pen.getColor();
		this.currentPenWidth = pen.getWidth();
		this._PENS.add(pen);
	}

	public void popPen() {
		this._PENS.remove(this._PENS.size() - 1);
		Pen pen = this._PENS.get(this._PENS.size() - 1);
		this.currentPenColor = pen.getColor();
		this.currentPenWidth = pen.getWidth();
	}

	public float getCurrentPenWidth() {
		return currentPenWidth;
	}

	public void setCurrentPenWidth(float width) {
		this.currentPenWidth = width;
	}

	public int getCurrentPenColor() {
		return currentPenColor;
	}

	public void setCurrentPenColor(int color) {
		this.currentPenColor = color;
	}

	public int getCurrentFillColor() {
		return this.currentFillColor;
	}
}
