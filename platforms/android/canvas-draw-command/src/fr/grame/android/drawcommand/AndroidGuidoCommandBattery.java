package fr.grame.android.drawcommand;

import fr.grame.android.drawcommand.command.AndroidCanvasBeginDrawCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasDrawMusicSymbolCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasDrawStringCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasEndDrawCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasGetFontColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasGetMusicFontCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasGetTextFontCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasLineCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasNotifySizeCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasOffsetOriginCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPolygonCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPopFillColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPopPenColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPopPenCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPopPenWidthCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPushFillColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPushPenColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPushPenCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasPushPenWidthCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasRectangleCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSelectPenColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetFontAlignCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetFontColorCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetMusicFontCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetOriginCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetScaleCommand;
import fr.grame.android.drawcommand.command.AndroidCanvasSetTextFontCommand;
import guidoengine.guidocommandbattery;

import java.util.ArrayList;
import java.util.List;

/**
 * Specific container of draw command for android device.
 *
 */
public class AndroidGuidoCommandBattery implements guidocommandbattery {

	public List<DrawToCanvas> drawCommands;

	public AndroidGuidoCommandBattery() {
		drawCommands = new ArrayList<DrawToCanvas>();
	}

	public void flush() {
		drawCommands = new ArrayList<DrawToCanvas>();
	}

	@Override
	public void storeBeginDrawCommand() {
		drawCommands.add(new AndroidCanvasBeginDrawCommand());
	}

	@Override
	public void storeEndDrawCommand() {
		drawCommands.add(new AndroidCanvasEndDrawCommand());
	}

	@Override
	public void storeLineCommand(float x1, float y1, float x2, float y2) {
		drawCommands.add(new AndroidCanvasLineCommand(x1, y1, x2, y2));
	}

	@Override
	public void storePolygonCommand(List<Float> xCoords, List<Float> yCoords, int count) {
		drawCommands.add(new AndroidCanvasPolygonCommand(xCoords, yCoords, count));
	}

	@Override
	public void storeRectangleCommand(float left, float top, float right, float bottom) {
		drawCommands.add(new AndroidCanvasRectangleCommand(left, top, right, bottom));
	}

	@Override
	public void storeSetMusicFontCommand(String name, int size, int properties) {
		drawCommands.add(new AndroidCanvasSetMusicFontCommand(name, size, properties));
	}

	@Override
	public void storeGetMusicFontCommand() {
		drawCommands.add(new AndroidCanvasGetMusicFontCommand());
	}

	@Override
	public void storeSetTextFontCommand(String name, int size, int properties) {
		drawCommands.add(new AndroidCanvasSetTextFontCommand(name, size, properties));
	}

	@Override
	public void storeGetTextFontCommand() {
		drawCommands.add(new AndroidCanvasGetTextFontCommand());
	}

	@Override
	public void storeSetScaleCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasSetScaleCommand(x, y));
	}

	@Override
	public void storeSetOriginCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasSetOriginCommand(x, y));
	}

	@Override
	public void storeOffsetOriginCommand(float x, float y) {
		drawCommands.add(new AndroidCanvasOffsetOriginCommand(x, y));
	}

	@Override
	public void storeNotifySizeCommand(int width, int height) {
		drawCommands.add(new AndroidCanvasNotifySizeCommand(width, height));
	}

	@Override
	public void storeDrawMusicSymbolCommand(float x, float y, int inSymbolID) {
		drawCommands.add(new AndroidCanvasDrawMusicSymbolCommand(x, y, inSymbolID));
	}

	@Override
	public void storeDrawStringCommand(float x, float y, String s, int inCharCount) {
		drawCommands.add(new AndroidCanvasDrawStringCommand(x, y, s, inCharCount));
	}

	@Override
	public void storeSetFontColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasSetFontColorCommand(alpha, red, green, blue));
	}

	@Override
	public void storeGetFontColorCommand() {
		drawCommands.add(new AndroidCanvasGetFontColorCommand());
	}

	@Override
	public void storeSetFontAlignCommand(int inAlign) {
		drawCommands.add(new AndroidCanvasSetFontAlignCommand(inAlign));
	}

	@Override
	public void storeSelectPenColorCommand(char alpha, char red, char green, char blue) {
		drawCommands.add(new AndroidCanvasSelectPenColorCommand(alpha, red, green, blue));
	}

	@Override
	public void storePopPenColorCommand() {
		drawCommands.add(new AndroidCanvasPopPenColorCommand());
	}

	@Override
	public void storePushPenWidthCommand(float width) {
		drawCommands.add(new AndroidCanvasPushPenWidthCommand(width));
	}

	@Override
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