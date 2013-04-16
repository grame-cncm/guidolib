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
	
private:
	void get(GuidoLayoutSettings& gsl, int& bbmap);
	void set(const GuidoLayoutSettings& gls, int bbmap);

	MainWindow *		mMainWindow;
	GuidoLayoutSettings mSavedSettings;
	int					mSavedBBMap;
};

#endif
