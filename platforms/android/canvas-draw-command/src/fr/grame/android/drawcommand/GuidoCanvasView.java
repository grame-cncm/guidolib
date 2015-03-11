package fr.grame.android.drawcommand;

import guidoengine.guido;
import guidoengine.guidobinaryparser;
import guidoengine.guidoscorebase;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

/**
 * A view which contains a canvas to draw the score.
 * The gmn code is parsed by the engine and exported to binary. the binary is parsed to created android 
 * draw command object and draw the score in the canvas. This class use an android Canvas.
 * It override the default method void onDraw(Canvas canvas) to draw the score. 
 * 
 */
public class GuidoCanvasView extends View {
	/** draw command container */
	private AndroidGuidoCommandBattery commandBattery;

	/** Scale use to draw in the canvas */
	private double globalRescaleFactor;
	
	/** Current font color */
	private int fontColor;
	
	/** Font use to draw music symbols */
	private Typeface musicFont;
	
	/** Size of music font */
	private int musicFontSize;
	
	/** Font use to draw text */
	private Typeface textFont;
	
	/** Size of text font */
	private int textFontSize;

	/** Colors to fill forms */
	private List<Integer> fillColors;
	
	/** Current color used to fill form */
	private int currentFillColor;

	/** List of pens */
	private List<Pen> pens;

	/** list of pen width*/
	private List<Float> penWidths;
	
	/** Current pen width */
	private float currentPenWidth;

	/** List of Pen colors */
	private List<Integer> penColors;
	
	/** Current pen color */
	private int currentPenColor;

	public GuidoCanvasView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
		doInits();
	}

	public void correctTransformMatrix(Canvas canvas) {
		canvas.save();
		canvas.scale((float) globalRescaleFactor, (float) globalRescaleFactor);
	}

	public void resetTransformMatrix(Canvas canvas) {
		canvas.restore();
	}

	/**
	 * initialize the view to draw the score.
	 */
	public void doInits() {
		commandBattery = new AndroidGuidoCommandBattery();
		globalRescaleFactor = 1.0;
		penColors = new ArrayList<Integer>();
		pushPenColor(Color.BLACK);
		// Init to default font color.
		setFontColor(Color.BLACK);
		penWidths = new ArrayList<Float>();
		pushPenWidth(1);
		setMusicFont(Typeface.create("Guido2", Typeface.NORMAL));
		setMusicFontSize(12);
		setTextFont(Typeface.create("Times", Typeface.NORMAL));
		setTextFontSize(12);
		fillColors = new ArrayList<Integer>();
		pushFillColor(Color.BLACK);
		pens = new ArrayList<Pen>();
		pushPen(new Pen(Color.BLACK, 1));
	}

	/**
	 * Print matrix in information level log
	 */
	public void printMatrixCoords() {
		float[] matrix = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		this.getMatrix().getValues(matrix);
		StringBuilder sb = new StringBuilder();
		sb.append("current matrix:");
		for (int i = 0; i < 9; i++) {
			sb.append(" ");
			sb.append(matrix[i]);
		}
		Log.i("SimpleGuidoEditor", sb.toString());
	}

	/**
	 * Parse guido code and export it to binary.
	 * @param gmn guido code.
	 * @return binary export
	 */
	private static byte[] gmnToBinary(String gmn) {
		guidoscorebase gmnscore = new guidoscorebase();
		gmnscore.OpenParser();
		int err = gmnscore.String2AR(gmn);
		gmnscore.CloseParser();
		if (err == guido.guidoNoErr) {
			err = gmnscore.AR2GR();
		}
		if (err == guido.guidoNoErr) {
			err = gmnscore.ResizePageToMusic();
		}
		byte[] out = gmnscore.BinaryExport(1);
		gmnscore.close();
		return out;
	}

	/**
	 * Draw guido code into a canvas
	 * @param gmn a guido code
	 */
	public void generateGuidoCanvas(String gmn) {
		if (gmn != null) {
			byte[] binary = gmnToBinary(gmn);
			if (binary.length > 0) {
				commandBattery.flush();
				guidobinaryparser.parseIntoDrawCommands(binary, commandBattery);
			}
		}
	}

	@Override
	protected void onDraw(Canvas canvas) {
		for (int i = 0; i < commandBattery.drawCommands.size(); i++) {
			commandBattery.drawCommands.get(i).drawToCanvas(canvas, this);
		}
	}

	// ********************** //
	// Stack gestion
	public void popFillColor() {
		this.fillColors.remove(this.fillColors.size() - 1);
		this.currentFillColor = this.fillColors.get(this.fillColors.size() - 1);
	}

	public void pushFillColor(int color) {
		this.fillColors.add(color);
		this.currentFillColor = color;
	}

	public void popPenColor() {
		this.penColors.remove(this.penColors.size() - 1);
		this.currentPenColor = this.penColors.get(this.penColors.size() - 1);
	}

	public void pushPenColor(int color) {
		this.penColors.add(color);
		this.currentPenColor = color;
	}

	public void popPenWidth() {
		this.penWidths.remove(this.penWidths.size() - 1);
		this.currentPenWidth = this.penWidths.get(penWidths.size() - 1);
	}

	public void pushPenWidth(float width) {
		this.penWidths.add(width);
		this.currentPenWidth = width;
	}

	public void pushPen(Pen pen) {
		this.currentPenColor = pen.getColor();
		this.currentPenWidth = pen.getWidth();
		this.pens.add(pen);
	}

	public void popPen() {
		this.pens.remove(this.pens.size() - 1);
		Pen pen = this.pens.get(this.pens.size() - 1);
		this.currentPenColor = pen.getColor();
		this.currentPenWidth = pen.getWidth();
	}

	// ***************************************** //
	// GETTERS AND SETTERS
	
	public float getCurrentPenWidth() {
		Log.i(VIEW_LOG_TAG, "CurrentPenWidth = " + currentPenWidth);
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

	/**
	 * @return the globalRescaleFactor
	 */
	public double getGlobalRescaleFactor() {
		return globalRescaleFactor;
	}

	/**
	 * @param globalRescaleFactor the globalRescaleFactor to set
	 */
	public void setGlobalRescaleFactor(double globalRescaleFactor) {
		this.globalRescaleFactor = globalRescaleFactor;
	}

	/**
	 * @return the fontColor
	 */
	public int getFontColor() {
		return fontColor;
	}

	/**
	 * @param fontColor the fontColor to set
	 */
	public void setFontColor(int fontColor) {
		this.fontColor = fontColor;
	}

	/**
	 * @return the musicFont
	 */
	public Typeface getMusicFont() {
		return musicFont;
	}

	/**
	 * @param musicFont the musicFont to set
	 */
	public void setMusicFont(Typeface musicFont) {
		this.musicFont = musicFont;
	}

	/**
	 * @return the musicFontSize
	 */
	public int getMusicFontSize() {
		return musicFontSize;
	}

	/**
	 * @param musicFontSize the musicFontSize to set
	 */
	public void setMusicFontSize(int musicFontSize) {
		this.musicFontSize = musicFontSize;
	}

	/**
	 * @return the textFont
	 */
	public Typeface getTextFont() {
		return textFont;
	}

	/**
	 * @param textFont the textFont to set
	 */
	public void setTextFont(Typeface textFont) {
		this.textFont = textFont;
	}

	/**
	 * @return the textFontSize
	 */
	public int getTextFontSize() {
		return textFontSize;
	}

	/**
	 * @param textFontSize the textFontSize to set
	 */
	public void setTextFontSize(int textFontSize) {
		this.textFontSize = textFontSize;
	}
}
