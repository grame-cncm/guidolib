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

#include <QWidget>
#include "MainWindow.h"
#include "SetupDialog.h"


//-------------------------------------------------------------------------
SetupDialog::SetupDialog(MainWindow *parent) 
 : QDialog(parent), mMainWindow(parent)
{
	setupUi(this);
	mSavedSettings = mMainWindow->getEngineSettings ();
	mSavedBBMap = GuidoGetDrawBoundingBoxes();
	set (mSavedSettings, mSavedBBMap);
	QObject::connect (fDefaultButton, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect (fSysDistBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fMaxDistBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fSpringBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fForceBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fSysDistrMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(setup()));
	QObject::connect (fOPFcheckBox, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fNSpacingcheckBox, SIGNAL(clicked()), this, SLOT(setup()));
//	QObject::connect (fOPFcheckBox, SIGNAL(stateChanged()), this, SLOT(setup()));
//	QObject::connect (fNSpacingcheckBox, SIGNAL(stateChanged()), this, SLOT(setup()));

	QObject::connect (fMapping, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fNewMapping, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fPageBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fSystemBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fSystemSliceBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fStaffBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fMeasureBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fEventBB, SIGNAL(clicked()), this, SLOT(setup()));
}

//-------------------------------------------------------------------------
SetupDialog::~SetupDialog()
{
}
	
//-------------------------------------------------------------------------
void SetupDialog::setup()
{
	GuidoLayoutSettings gls;
	int bbmap;
	get (gls, bbmap);
	mMainWindow->setEngineSettings (gls, bbmap);	
}
	
//-------------------------------------------------------------------------
void SetupDialog::reject()
{
	mMainWindow->setEngineSettings (mSavedSettings, mSavedBBMap);
	QDialog::reject();
}
	
//-------------------------------------------------------------------------
void SetupDialog::get (GuidoLayoutSettings& gls, int& bbmap)
{
	gls.systemsDistance		= fSysDistBox->value();
	gls.systemsDistribLimit = float(fMaxDistBox->value()) / 100;
	gls.spring				= float(fSpringBox->value()) / 100;
	gls.force				= fForceBox->value();
	gls.systemsDistribution	= fSysDistrMenu->currentIndex() + 1 ;
	gls.optimalPageFill		= fOPFcheckBox->checkState() == Qt::Checked ? 1 : 0;
	gls.neighborhoodSpacing	= fNSpacingcheckBox->checkState() == Qt::Checked ? 1 : 0;
	
	bbmap = kNoBB;
	if (fPageBB->checkState() == Qt::Checked)		bbmap |= kPageBB;
	if (fSystemBB->checkState() == Qt::Checked)		bbmap |= kSystemsBB;
	if (fSystemSliceBB->checkState() == Qt::Checked) bbmap |= kSystemsSliceBB;
	if (fStaffBB->checkState() == Qt::Checked)		bbmap |= kStavesBB;
	if (fMeasureBB->checkState() == Qt::Checked)	bbmap |= kMeasureBB;
	if (fEventBB->checkState() == Qt::Checked)		bbmap |= kEventsBB;
//	if (fMapping->checkState() == Qt::Checked)		bbmap |= kDrawMap;
//	if (fNewMapping->checkState() == Qt::Checked)	bbmap |= kNewMap;
}
	
//-------------------------------------------------------------------------
void SetupDialog::set (const GuidoLayoutSettings& gls, int bbmap)
{
	fSysDistBox->setValue(gls.systemsDistance);
	fMaxDistBox->setValue(gls.systemsDistribLimit*100);
	fSpringBox->setValue(gls.spring*100);
	fForceBox->setValue(gls.force);
	fSysDistrMenu->setCurrentIndex(gls.systemsDistribution - 1);
	fOPFcheckBox->setCheckState(gls.optimalPageFill ? Qt::Checked : Qt::Unchecked);
	fNSpacingcheckBox->setCheckState(gls.neighborhoodSpacing ? Qt::Checked : Qt::Unchecked);

//	fMapping->setCheckState(bbmap & kDrawMap ? Qt::Checked : Qt::Unchecked);
//	fNewMapping->setCheckState(bbmap & kNewMap ? Qt::Checked : Qt::Unchecked);
	fPageBB->setCheckState(bbmap & kPageBB ? Qt::Checked : Qt::Unchecked);
	fSystemBB->setCheckState(bbmap & kSystemsBB ? Qt::Checked : Qt::Unchecked);
	fSystemSliceBB->setCheckState(bbmap & kSystemsSliceBB ? Qt::Checked : Qt::Unchecked);
	fStaffBB->setCheckState(bbmap & kStavesBB ? Qt::Checked : Qt::Unchecked);
	fMeasureBB->setCheckState(bbmap & kMeasureBB ? Qt::Checked : Qt::Unchecked);
	fEventBB->setCheckState(bbmap & kEventsBB ? Qt::Checked : Qt::Unchecked);
}

//-------------------------------------------------------------------------
void SetupDialog::reset()
{
	GuidoLayoutSettings gls;
	GuidoGetDefaultLayoutSettings (&gls);
	set (gls, kNoBB);
	setup();
}

