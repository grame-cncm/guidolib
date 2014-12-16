package fr.grame.simpleguidoeditor;

import fr.grame.simpleguidoeditor.drawcommand.*;

import guidoengine.guidocommandbattery;

import java.util.*;

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

}