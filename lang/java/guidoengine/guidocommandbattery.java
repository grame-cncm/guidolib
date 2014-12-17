package guidoengine;

import java.util.List;

public interface guidocommandbattery {
    public void storeBeginDrawCommand();
    public void storeEndDrawCommand();
    public void storeLineCommand(float x1, float y1, float x2, float y2);
    public void storePolygonCommand(List<Float> xCoords, List<Float> yCoords, int count);
    public void storeRectangleCommand(float left, float top, float right, float bottom);
    public void storeSetMusicFontCommand(String name, int size, int properties);
    public void storeGetMusicFontCommand();
    public void storeSetTextFontCommand(String name, int size, int properties);
    public void storeGetTextFontCommand();
    public void storeSetScaleCommand(float x, float y);
    public void storeSetOriginCommand(float x, float y);
    public void storeOffsetOriginCommand(float x, float y);
    public void storeNotifySizeCommand(int width, int height);
    public void storeDrawMusicSymbolCommand(float x, float y, int inSymbolID);
    public void storeDrawStringCommand(float x, float y, String s, int inCharCount);
    public void storeSetFontColorCommand(char alpha, char red, char green, char blue);
    public void storeGetFontColorCommand();
    public void storeSetFontAlignCommand(int inAlign);
    public void storeSelectPenColorCommand(char alpha, char red, char green, char blue);
    public void storePopPenColorCommand();
    public void storePushPenWidthCommand(float width);
    public void storePopPenWidthCommand();
}