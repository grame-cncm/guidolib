/****************************************************************************
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
****************************************************************************/

#include <QtGui>

#include "CodeEditor.h"

//-------------------------------------------------------------------------
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));

    updateLineNumberAreaWidth(0);
#ifdef QTTABSTOPDISTANCE
	setTabStopDistance (20);
#else
	setTabStopWidth (20);
#endif
}

#ifdef QTFONTMETRICS_511
#define WIDTH(m, s) m.horizontalAdvance(s)
#else
#define WIDTH(m, s) m.width(s)
#endif

//-------------------------------------------------------------------------
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
//    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    int space = 3 + WIDTH(fontMetrics(), QLatin1Char('9')) * digits;
    return space;
}

//-------------------------------------------------------------------------
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{ 
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//-------------------------------------------------------------------------
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//-------------------------------------------------------------------------
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//-------------------------------------------------------------------------
void CodeEditor::highlightErrorLine(int line)
{
	if ( line < 1 )
	{
		setExtraSelections( QList<QTextEdit::ExtraSelection>() );
		return;
	}
	
    QList<QTextEdit::ExtraSelection> extraSelections;

	QColor lineColor = QColor(Qt::red).lighter(160);

	QTextBlock textBlock = document()->findBlockByNumber( line-1 );
	QTextCursor textCursor( textBlock );

	for ( int i = 0 ; i < textBlock.lineCount() ; i++ )
	{
		QTextEdit::ExtraSelection selection;
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor;
		for ( int down = 0 ; down < i ; down++ )
			selection.cursor.movePosition( QTextCursor::Down );

		selection.cursor.select( QTextCursor::LineUnderCursor );
		extraSelections.append(selection);
	}

    setExtraSelections(extraSelections);
}

//-------------------------------------------------------------------------
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
	painter.setPen(Qt::gray);
	QFont lineFont = document()->defaultFont();
	lineFont.setPointSize( lineFont.pointSize()*2/3 );
	painter.setFont( lineFont );

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top() + fontMetrics().leading();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignBottom, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
