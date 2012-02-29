/*
 * SetupDialog.h
 *
 * Created by Christophe Daudin on 17/04/08.
 * Copyright 2008 Grame. All rights reserved.
 *
 * This file may be used under the terms of the GNU General Public
 * License version 2.0 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of
 * this file.  Please review the following information to ensure GNU
 * General Public Licensing requirements will be met:
 * http://trolltech.com/products/qt/licenses/licensing/opensource/
 *
 * If you are unsure which license is appropriate for your use, please
 * review the following information:
 * http://trolltech.com/products/qt/licenses/licensing/licensingoverview
 * or contact the sales department at sales@trolltech.com.
 *
 * In addition, as a special exception, Trolltech gives you certain
 * additional rights. These rights are described in the Trolltech GPL
 * Exception version 1.0, which can be found at
 * http://www.trolltech.com/products/qt/gplexception/ and in the file
 * GPL_EXCEPTION.txt in this package.
 *
 * In addition, as a special exception, Trolltech, as the sole copyright
 * holder for Qt Designer, grants users of the Qt/Eclipse Integration
 * plug-in the right for the Qt/Eclipse Integration to link to
 * functionality provided by Qt Designer and its related libraries.
 *
 * Trolltech reserves all rights not expressly granted herein.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
 
#ifndef __SetupDialog__
#define __SetupDialog__

#include <QColorDialog>
#include <QDialog>
#include <QObject>
#include "ui_setupDialog.h"
#include "GUIDOEngine.h"

class QGuidoWidget;
class MainWindow;


class SetupDialog : public QDialog, public Ui::Setup
{
    Q_OBJECT

public:
	SetupDialog(MainWindow *parent);
	virtual ~SetupDialog();

private slots:
	void reject();
	void reset();
	void setup();
	void changeColor();
	void scoreColorChanged(const QColor& c);
	void fontColorButtonClicked();
	void fontColorChanged(const QColor& color);
	void fontWeightChanged(int index);
	void voiceStaffSetup(int index);

private:
	void get (GuidoLayoutSettings& gls, int& bbmap, bool& showMapping, bool& rawMapping, bool& showBoxes, int&voiceNum, int&staffNum);
	void set(const GuidoLayoutSettings& gls, int bbmap , bool showMapping, bool rawMapping, bool showBoxes , int voiceNum , int staffNum );
	
	int voiceNum() const;
	int staffNum() const;

	MainWindow *		mMainWindow;
	GuidoLayoutSettings mSavedSettings;
	int					mSavedBBMap;
	bool				mSavedShowMapping, mSavedRawMapping, mSavedShowBoxes;
	QColor				mSavedColor;
	int					mSavedVoiceNum, mSavedStaffNum;
	QMap<int, QPushButton*> mFontColorMap;
	QMap<int, QComboBox*>	mFontWeightMap;
	QMap<int, QColor>		mSavedColors;
	QMap<int, int>			mSavedWeights;
static QColorDialog *		mColorDialog;
};

#endif
