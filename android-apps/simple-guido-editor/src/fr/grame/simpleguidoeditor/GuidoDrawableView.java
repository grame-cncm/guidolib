package fr.grame.simpleguidoeditor;

import android.graphics.drawable.ShapeDrawable;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.shapes.OvalShape;
import android.util.AttributeSet;

public class GuidoDrawableView extends View {
  private ShapeDrawable mDrawable;

  public GuidoDrawableView(Context context, AttributeSet attributeSet) {
    super(context);

    int x = 10;
    int y = 10;
    int width = 300;
    int height = 50;

    mDrawable = new ShapeDrawable(new OvalShape());
    mDrawable.getPaint().setColor(0xff74AC23);
    mDrawable.setBounds(x, y, x + width, y + height);
  }

  protected void onDraw(Canvas canvas) {
    mDrawable.draw(canvas);
  }
}
