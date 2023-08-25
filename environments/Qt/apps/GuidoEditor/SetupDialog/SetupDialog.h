/*
 * SetupDialog.h
 *
 * Created by Christophe Daudin on 17/04/08.
 * Copyright 2008 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
 
#ifndef __SetupDialog__
#define __SetupDialog__

#include <QColorDialog>
#include <QDialog>
#include <QObject>
#include "ui_setupDialog.h"
#include "GUIDOEngine.h"
#include "MainWindow.h"

class QGuidoWidget;
class MainWindow;

class ColorDialog : public QColorDialog
{
    Q_OBJECT

	public :
		ColorDialog (QWidget *parent) : QColorDialog(parent) { setAttribute(Qt::WA_QuitOnClose); }
		~ColorDialog() {}
	
	protected:
	virtual void	reject() override;
	
	private:
	virtual void closeEvent(QCloseEvent *event);

};

class SetupDialog : public QDialog, public Ui::Setup
{
    Q_OBJECT

public:
			 SetupDialog(MainWindow *parent);
	virtual ~SetupDialog();

	void	setDisplayState(const THideState& state);
	void	setTimings (long ar, long ar2gr, long draw);

private slots:
	void reject();
	void reset();
	void setup();
	void showhide();
	void changeColor();
	void scoreColorChanged(const QColor& c);
	void fontColorButtonClicked();
	void fontColorChanged(const QColor& color);
	void fontWeightChanged(int index);
	void voiceStaffSetup(int index);

private:
	THideState	getDisplayState ();
	void		resetDisplayState ();

	void getState (GuidoLayoutSettings& gls, int& bbmap, bool& showMapping, bool& rawMapping, bool& showBoxes, int&voiceNum, int&staffNum);
	void setState (const GuidoLayoutSettings& gls, int bbmap , bool showMapping, bool rawMapping, bool showBoxes , int voiceNum , int staffNum );
	
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
static ColorDialog *		mColorDialog;
};

#endif
