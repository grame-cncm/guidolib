package fr.grame.simpleguidoeditor;

import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasBeginDrawCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasDrawMusicSymbolCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasDrawStringCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasEndDrawCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasGetFontColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasGetMusicFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasGetTextFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasLineCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasNotifySizeCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasOffsetOriginCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPolygonCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPopFillColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPopPenColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPopPenCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPopPenWidthCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPushFillColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPushPenColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPushPenCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasPushPenWidthCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasRectangleCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSelectPenColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetFontAlignCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetFontColorCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetMusicFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetOriginCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetScaleCommand;
import fr.grame.simpleguidoeditor.drawcommand.AndroidCanvasSetTextFontCommand;
import fr.grame.simpleguidoeditor.drawcommand.DrawToCanvas;
import guidoengine.guidocommandbattery;

import java.util.ArrayList;
import java.util.List;

public class AndroidGuidoCommandBattery implements guidocommandbattery {

	public List<DrawToCanvas> drawCommands;

	public AndroidGuidoCommandBattery() {
		drawCommands = new ArrayList<DrawToCanvas>();
	}

	public void flush() {
		drawCommands = new ArrayList<DrawToCanvas>();
	}

	public void storeBeginDrawCommand() {
		drawCommands.add(new AndroidCanvasBeginDrawCommand());
	}

	public void storeEndDrawCommand() {
		drawCommands.add(new AndroidCanvasEndDrawCommand());
	}

	public void storeLineCommand(float x1, float y1, float x2, float y2) {
		drawCommands.add(new AndroidCanvasLineCommand(x1, y1, x2, y2));
	}

	public void storePolygonCommand(List<Float> xCoords, List<Float> yCoords, int count) {
		drawCommands.add(new AndroidCanvasPolygonCommand(xCoords, yCoords, count));
	}

	public void storeRectangleCommand(float left, float top, float right, float bottom) {
		drawCommands.add(new AndroidCanvasRectangleCommand(left, top, right, bottom));
	}

	public void storeSetMusicFontCommand(String name, int size, int properties) {
		drawCommands.add(new AndroidCanvasSetMusicFontCommand(name, size, properties));
	}

	public void storeGetMusicFontCommand() {
		drawCommands.add(new AndroidCanvasGetMusicFontCommand());
	}

	public void storeSetTextFontCommand(String name, int size, int properties) {
		drawCommands.add(new AndroidCanvasSetTextFontCommand(name, size, properties));
	}

	public void storeGetTextFontCommand() {
		drawCommands.add(new AndroidCanvasGetTextFontCommand());
	}

	public void storeSetScaleCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasSetScaleCommand(x, y));
	}

	public void storeSetOriginCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasSetOriginCommand(x, y));
	}

	public void storeOffsetOriginCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasOffsetOriginCommand(x, y));
	}

	public void storeNotifySizeCommand(int width, int height) {
		drawCommands.add(new AndroidCanvasNotifySizeCommand(width, height));
	}

	public void storeDrawMusicSymbolCommand(float x, float y, int inSymbolID) {
		drawCommands.add(new AndroidCanvasDrawMusicSymbolCommand(x, y, inSymbolID));
	}

	public void storeDrawStringCommand(float x, float y, String s, int inCharCount) {
		drawCommands.add(new AndroidCanvasDrawStringCommand(x, y, s, inCharCount));
	}

	public void storeSetFontColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasSetFontColorCommand(alpha, red, green, blue));
	}

	public void storeGetFontColorCommand() {
		drawCommands.add(new AndroidCanvasGetFontColorCommand());
	}

	public void storeSetFontAlignCommand(int inAlign) {
		drawCommands.add(new AndroidCanvasSetFontAlignCommand(inAlign));
	}

	public void storeSelectPenColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasSelectPenColorCommand(alpha, red, green, blue));
	}

	public void storePopPenColorCommand() {
		drawCommands.add(new AndroidCanvasPopPenColorCommand());
	}

	public void storePushPenWidthCommand(float width) {
		drawCommands.add(new AndroidCanvasPushPenWidthCommand(width));
	}

	public void storePopPenWidthCommand() {
		drawCommands.add(new AndroidCanvasPopPenWidthCommand());
	}

	@Override
	public void storePopFillColorCommand() {
		drawCommands.add(new AndroidCanvasPopFillColorCommand());
	}

	@Override
	public void storePopPenCommand() {
		drawCommands.add(new AndroidCanvasPopPenCommand());
	}

	@Override
	public void storePushFillColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasPushFillColorCommand(alpha, red, green, blue));
	}

	@Override
	public void storePushPenColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasPushPenColorCommand(alpha, red, green, blue));
	}

	@Override
	public void storePushPenCommand(char alpha, char red, char green, char blue, float width) {
		drawCommands.add(new AndroidCanvasPushPenCommand(alpha, red, green, blue, width));
	}

}