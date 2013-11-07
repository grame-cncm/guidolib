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

