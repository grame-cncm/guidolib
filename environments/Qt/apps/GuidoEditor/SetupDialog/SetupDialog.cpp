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
#include <QSettings>
#include <QColorDialog>
#include <QFont>
#include <QtDebug>

#include "SetupDialog.h"
#include "GuidoHighlighter.h"

#define SETUP_DIALOG_POS_SETTING	"SetupDialogPosSetting"
#define SETUP_DIALOG_SIZE_SETTING	"SetupDialogSizeSetting"

#define SYNTAX_ELT_ID		"syntaxElementId"
#define BUTTON_COLOR		"buttonColor"
#define DIALOG_ELEMENT_ID	"dialogEltId"

QString weightToString( QFont::Weight w )
{
	switch (w)
	{
		case QFont::Normal:
			return "Normal";
		case QFont::Bold:
			return "Bold";
		default:
			return "";
	}
	return "";
}

QString weightToString(int weight) { return weightToString( QFont::Weight(weight) ); }
//QColorDialog* SetupDialog::mColorDialog = 0;
ColorDialog* SetupDialog::mColorDialog = 0;
//-------------------------------------------------------------------------
SetupDialog::SetupDialog(MainWindow *parent) 
 : QDialog(parent), mMainWindow(parent)
{
	setupUi(this);

	QObject::connect (fDefaultButton, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect (fSysDistBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fMaxDistBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fSpringBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fForceBox, SIGNAL(valueChanged(int)), this, SLOT(setup()));
	QObject::connect (fSysDistrMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(setup()));
	QObject::connect (fOPFcheckBox, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fNSpacingcheckBox, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fPropRendercheckBox, SIGNAL(clicked()), this, SLOT(setup()));
    QObject::connect (fResizePage2MusiccheckBox, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fColorButton, SIGNAL(clicked()) , this, SLOT(changeColor()));

    QObject::connect (fCheckLyrics, SIGNAL(clicked()), this, SLOT(setup()));
	
	QObject::connect (fMapping, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fRawMapping, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fBoundingBoxes, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fPageBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fSystemBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fSystemSliceBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fStaffBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fMeasureBB, SIGNAL(clicked()), this, SLOT(setup()));
	QObject::connect (fEventBB, SIGNAL(clicked()), this, SLOT(setup()));

	QObject::connect (fVoiceNumEdit, SIGNAL(valueChanged(int)), this, SLOT(voiceStaffSetup(int)));
	QObject::connect (fStaffNumEdit, SIGNAL(valueChanged(int)), this, SLOT(voiceStaffSetup(int)));
	QObject::connect (fShowAllStaffsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(voiceStaffSetup(int)));
	QObject::connect (fShowAllVoicesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(voiceStaffSetup(int)));

    QObject::connect (fHideSlurs, SIGNAL(clicked()), this, SLOT(showhide()));
    QObject::connect (fHideDynamics, SIGNAL(clicked()), this, SLOT(showhide()));
    QObject::connect (fHideArticulations, SIGNAL(clicked()), this, SLOT(showhide()));
    QObject::connect (fHideText, SIGNAL(clicked()), this, SLOT(showhide()));
    QObject::connect (fHideLyrics, SIGNAL(clicked()), this, SLOT(showhide()));


	mSavedSettings = mMainWindow->getEngineSettings();
	mSavedBBMap = mMainWindow->getBBMap();
	mSavedShowMapping = mMainWindow->getShowMapping();
	mSavedRawMapping = mMainWindow->getRawMapping();
	mSavedShowBoxes = mMainWindow->getShowBoxes();
	mSavedVoiceNum = mMainWindow->getVoiceNum();
	mSavedStaffNum = mMainWindow->getStaffNum();
	scoreColorChanged( mMainWindow->getScoreColor() );
	setState (mSavedSettings, mSavedBBMap, mSavedShowMapping, mSavedRawMapping, mSavedShowBoxes, mSavedVoiceNum, mSavedStaffNum);
	setDisplayState (parent->getDisplayState());

	mFontColorMap[ GuidoHighlighter::VOICE_SEPARATOR_ELT ]	= fVoiceSeparatorColorButton;
	mFontColorMap[ GuidoHighlighter::SCORE_SEPARATOR_ELT ]	= fScoreSeparatorColorButton;
	mFontColorMap[ GuidoHighlighter::DURATION_ELT ]			= fDurationsColorButton;
	mFontColorMap[ GuidoHighlighter::NOTE_ELT ]				= fNotesColorButton;
	mFontColorMap[ GuidoHighlighter::TAG_PARAM_ELT ]		= fTagsParametersColorButton;
	mFontColorMap[ GuidoHighlighter::TAG_ELT ]				= fTagsColorButton;
	mFontColorMap[ GuidoHighlighter::COMMENT_ELT ]			= fCommentsColorButton;
						
	mFontWeightMap[ GuidoHighlighter::VOICE_SEPARATOR_ELT ]	= fVoiceSeparatorWeight;
	mFontWeightMap[ GuidoHighlighter::SCORE_SEPARATOR_ELT ]	= fScoreSeparatorWeight;
	mFontWeightMap[ GuidoHighlighter::DURATION_ELT ]		= fDurationsWeight;
	mFontWeightMap[ GuidoHighlighter::NOTE_ELT ]			= fNotesWeight;
	mFontWeightMap[ GuidoHighlighter::TAG_PARAM_ELT ]		= fTagsParametersWeight;
	mFontWeightMap[ GuidoHighlighter::TAG_ELT ]				= fTagsWeight;
	mFontWeightMap[ GuidoHighlighter::COMMENT_ELT ]			= fCommentsWeight;

	fTabWidget->setCurrentIndex(0);

	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
	{
		// Set widgets properties with syntax-element id
		mFontColorMap[ i ]->setProperty( SYNTAX_ELT_ID , i );
		mFontWeightMap[ i ]->setProperty( SYNTAX_ELT_ID , i );
		
		// Set the color of the font-color button
		QColor c = mMainWindow->getHighlighter()->color( i );
		QPixmap pixmap(30 , 30);
		pixmap.fill( c );
		mFontColorMap[ i ]->setIcon( QIcon(pixmap) );
		mFontColorMap[ i ]->setProperty( BUTTON_COLOR , c );
				
		// Adds font-weight items in the combobox
		for ( int w = 0 ; w <= QFont::Black ; w++ )
		{
			QString weightString = weightToString( w );
			if ( weightString.size() )
				mFontWeightMap[i]->addItem( weightString , w );
		}
		// Set the current combobox item
		QFont::Weight weight = (QFont::Weight)mMainWindow->getHighlighter()->weight( i );
		mFontWeightMap[i]->setCurrentIndex( mFontWeightMap[i]->findData( weight ) );
		
		// Connect the font widgets to their methods
		connect( mFontColorMap[i] , SIGNAL(clicked()) , this , SLOT(fontColorButtonClicked()) );
		connect( mFontWeightMap[i] , SIGNAL(currentIndexChanged(int)) , this , SLOT(fontWeightChanged(int)) );

		mSavedColors[i] = c;
		mSavedWeights[i]= int(weight);
	}

	if (!mColorDialog) {
		mColorDialog = new ColorDialog( this);
		mColorDialog->setWindowTitle("Choose score color");
		mColorDialog->setOption (QColorDialog::NoButtons);
	}
	
	QSettings settings;
    QPoint pos = settings.value(SETUP_DIALOG_POS_SETTING, QPoint(300, 300)).toPoint();
    QSize winSize = settings.value(SETUP_DIALOG_SIZE_SETTING, sizeHint() ).toSize();
    resize(winSize);
    move(pos);
}

//-------------------------------------------------------------------------
SetupDialog::~SetupDialog()
{
	QSettings settings;
	settings.setValue(SETUP_DIALOG_POS_SETTING, pos());
    settings.setValue(SETUP_DIALOG_SIZE_SETTING, size());
}

//-------------------------------------------------------------------------
void SetupDialog::setup()
{
	GuidoLayoutSettings gls;
	int bbmap,voiceNum,staffNum;
	bool showBoxes, showMapping, rawMapping;
	getState (gls, bbmap , showMapping , rawMapping, showBoxes , voiceNum,staffNum);
	mMainWindow->setEngineSettings (gls, bbmap, showMapping, rawMapping, showBoxes, voiceNum, staffNum);
}

//-------------------------------------------------------------------------
void SetupDialog::showhide()
{
	THideState state = getDisplayState ();
	mMainWindow->setDisplayState (state);
}

//-------------------------------------------------------------------------
void SetupDialog::resetDisplayState ()
{
	fHideSlurs->setCheckState(Qt::Unchecked);
	fHideArticulations->setCheckState(Qt::Unchecked);
	fHideDynamics->setCheckState(Qt::Unchecked);
	fHideText->setCheckState(Qt::Unchecked);
	fHideLyrics->setCheckState(Qt::Unchecked);
}

//-------------------------------------------------------------------------
THideState SetupDialog::getDisplayState()
{
	THideState state;
	state.slurs			= (fHideSlurs->checkState() == Qt::Checked);
	state.articulations = (fHideArticulations->checkState() == Qt::Checked);
	state.dynamics		= (fHideDynamics->checkState() == Qt::Checked);
	state.text			= (fHideText->checkState() == Qt::Checked);
	state.lyrics		= (fHideLyrics->checkState() == Qt::Checked);
	return state;
}

//-------------------------------------------------------------------------
void SetupDialog::setDisplayState(const THideState& state)
{
	fHideSlurs->setCheckState(state.slurs ? Qt::Checked : Qt::Unchecked);
	fHideArticulations->setCheckState(state.articulations ? Qt::Checked : Qt::Unchecked);
	fHideDynamics->setCheckState(state.dynamics ? Qt::Checked : Qt::Unchecked);
	fHideText->setCheckState(state.text ? Qt::Checked : Qt::Unchecked);
	fHideLyrics->setCheckState(state.lyrics ? Qt::Checked : Qt::Unchecked);
}

//-------------------------------------------------------------------------
void SetupDialog::changeColor()
{
//	mColorDialog->disconnect();
	mColorDialog->setCurrentColor (mSavedColor);
//	connect( mColorDialog , SIGNAL( currentColorChanged(const QColor&)) , this , SLOT( scoreColorChanged(const QColor&) ) );
//	mColorDialog->open();
}

//-------------------------------------------------------------------------
void SetupDialog::scoreColorChanged(const QColor& c)
{
	if ( c.isValid() )
	{
		mMainWindow->setScoreColor( c );
		QPixmap pixmap(30 , 30);
		pixmap.fill( c );
		fColorButton->setIcon( QIcon(pixmap) );
	}
}

//-------------------------------------------------------------------------
void SetupDialog::reject()
{
	mMainWindow->setEngineSettings (mSavedSettings, mSavedBBMap, mSavedShowMapping, mSavedRawMapping, mSavedShowBoxes, mSavedVoiceNum, mSavedStaffNum);
	scoreColorChanged(mSavedColor);

	GuidoHighlighter * highlighter = new GuidoHighlighter();
	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
		highlighter->addRule( i , mSavedColors[i] , QFont::Weight(mSavedWeights[i]) );
	mMainWindow->setHighlighter(highlighter);

	QDialog::reject();
}
	
//-------------------------------------------------------------------------
void SetupDialog::setTimings (long ar, long ar2gr, long draw)
{
	QString str;
	fARTime->setText(str.setNum(ar));
	fAR2GRTime->setText(str.setNum(ar2gr));
	fDrawTime->setText(str.setNum(draw));
	fTotalTime->setText(str.setNum(ar + ar2gr + draw));
}
	
//-------------------------------------------------------------------------
void SetupDialog::getState (GuidoLayoutSettings& gls, int& bbmap, bool& showMapping, bool& rawMapping, bool& showBoxes, int&voiceNum, int&staffNum)
{
	gls.systemsDistance		  = fSysDistBox->value();
	gls.systemsDistribLimit   = float(fMaxDistBox->value()) / 100;
	gls.spring				  = float(fSpringBox->value()) / 100;
	gls.force				  = fForceBox->value();
	gls.systemsDistribution	  = fSysDistrMenu->currentIndex() + 1 ;
	gls.optimalPageFill		  = fOPFcheckBox->checkState() == Qt::Checked ? 1 : 0;
	gls.neighborhoodSpacing   = fNSpacingcheckBox->checkState() == Qt::Checked ? 1 : 0;
    gls.resizePage2Music	  = fResizePage2MusiccheckBox->checkState() == Qt::Checked ? 1 : 0;
    gls.checkLyricsCollisions = fCheckLyrics->checkState() == Qt::Checked;
    gls.proportionalRenderingForceMultiplicator = (fPropRendercheckBox->checkState() == Qt::Checked ? fForceBox->value() : 0);
	
	bbmap = kNoBB;
	if (fPageBB->checkState() == Qt::Checked)			bbmap |= kPageBB;
	if (fSystemBB->checkState() == Qt::Checked)			bbmap |= kSystemsBB;
	if (fSystemSliceBB->checkState() == Qt::Checked)	bbmap |= kSystemsSliceBB;
	if (fStaffBB->checkState() == Qt::Checked)			bbmap |= kStavesBB;
	if (fMeasureBB->checkState() == Qt::Checked)		bbmap |= kMeasureBB;
	if (fEventBB->checkState() == Qt::Checked)			bbmap |= kEventsBB;
	showMapping = fMapping->checkState();
	rawMapping = fRawMapping->checkState();
	showBoxes = fBoundingBoxes->checkState();
	voiceNum = this->voiceNum();
	staffNum = this->staffNum();
}
	
//-------------------------------------------------------------------------
void SetupDialog::setState (const GuidoLayoutSettings& gls, int bbmap, bool showMapping, bool rawMapping, bool showBoxes, int voiceNum, int staffNum)
{
	fSysDistBox->setValue	(gls.systemsDistance);
	fMaxDistBox->setValue	(gls.systemsDistribLimit*100);
	fSpringBox->setValue	(gls.spring*100);
	fForceBox->setValue		(gls.force);
	fSysDistrMenu->setCurrentIndex	(gls.systemsDistribution - 1);
	fOPFcheckBox->setCheckState		(gls.optimalPageFill ? Qt::Checked : Qt::Unchecked);
	fNSpacingcheckBox->setCheckState(gls.neighborhoodSpacing ? Qt::Checked : Qt::Unchecked);
    fResizePage2MusiccheckBox->setCheckState(gls.resizePage2Music ? Qt::Checked : Qt::Unchecked);
	fPropRendercheckBox->setCheckState		(gls.proportionalRenderingForceMultiplicator ? Qt::Checked : Qt::Unchecked);
	fCheckLyrics->setCheckState				(gls.checkLyricsCollisions ? Qt::Checked : Qt::Unchecked);

	fMapping->setCheckState			(showMapping ? Qt::Checked : Qt::Unchecked);
	fRawMapping->setCheckState		(rawMapping ? Qt::Checked : Qt::Unchecked);
	fBoundingBoxes->setCheckState	(showBoxes ? Qt::Checked : Qt::Unchecked);

	fPageBB->setCheckState			(bbmap & kPageBB ? Qt::Checked : Qt::Unchecked);
	fSystemBB->setCheckState		(bbmap & kSystemsBB ? Qt::Checked : Qt::Unchecked);
	fSystemSliceBB->setCheckState(	bbmap & kSystemsSliceBB ? Qt::Checked : Qt::Unchecked);
	fStaffBB->setCheckState			(bbmap & kStavesBB ? Qt::Checked : Qt::Unchecked);
	fMeasureBB->setCheckState		(bbmap & kMeasureBB ? Qt::Checked : Qt::Unchecked);
	fEventBB->setCheckState			(bbmap & kEventsBB ? Qt::Checked : Qt::Unchecked);
	
	fShowAllVoicesCheckBox->setCheckState( voiceNum==ALL_VOICE ? Qt::Checked : Qt::Unchecked );
	if ( (voiceNum!=ALL_VOICE) )
		fVoiceNumEdit->setValue(voiceNum);

	fShowAllStaffsCheckBox->setCheckState( staffNum==ALL_STAFF ? Qt::Checked : Qt::Unchecked );
	if ( (staffNum!=ALL_STAFF) )
		fStaffNumEdit->setValue(staffNum);
}

//-------------------------------------------------------------------------
int SetupDialog::voiceNum() const
{
	return ( fShowAllVoicesCheckBox->checkState() ) ? ALL_VOICE : fVoiceNumEdit->value();
}

//-------------------------------------------------------------------------
int SetupDialog::staffNum() const
{
	return ( fShowAllStaffsCheckBox->checkState() ) ? ALL_STAFF : fStaffNumEdit->value();
}
	
//-------------------------------------------------------------------------
void SetupDialog::reset()
{
	GuidoLayoutSettings gls;
	GuidoGetDefaultLayoutSettings (&gls);
	setState (gls, kNoBB , false , true, false , ALL_VOICE , ALL_STAFF );
	setup();
	scoreColorChanged( Qt::black );
	
	GuidoHighlighter * highlighter = new GuidoHighlighter();
	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
		highlighter->addRule( i , mMainWindow->mDefaultFontColors[i] , QFont::Weight(mMainWindow->mDefaultFontWeights[i]) );
	mMainWindow->setHighlighter(highlighter);
}

//-------------------------------------------------------------------------
void SetupDialog::fontColorButtonClicked()
{
	QPushButton * button = (QPushButton *)sender();
	mColorDialog->disconnect();
	mColorDialog->setCurrentColor (button->property( BUTTON_COLOR ).value<QColor>());
	connect( mColorDialog , SIGNAL( currentColorChanged(const QColor&)) , this , SLOT( fontColorChanged(const QColor&) ) );
	mColorDialog->setProperty( DIALOG_ELEMENT_ID , button->property( SYNTAX_ELT_ID ).toInt() );
	mColorDialog->exec();
}

void ColorDialog::closeEvent(QCloseEvent *event){
	parentWidget()->activateWindow();
	parentWidget()->raise();
}

void ColorDialog::reject()
{
	close();
}


//-------------------------------------------------------------------------
void SetupDialog::fontColorChanged(const QColor& color)
{
	QColorDialog * dialog = (QColorDialog *)sender();
	int syntaxEltId = dialog->property( DIALOG_ELEMENT_ID ).toInt();

	if ( color.isValid() )
	{
		QPixmap pixmap(30 , 30);
		pixmap.fill( color );
		mFontColorMap[syntaxEltId]->setIcon( QIcon(pixmap) );
		mFontColorMap[syntaxEltId]->setProperty( BUTTON_COLOR , color );
		mMainWindow->setHighlighter( syntaxEltId , color , mFontWeightMap[syntaxEltId]->itemData( mFontWeightMap[syntaxEltId]->currentIndex() ).toInt() );
	}
}

//-------------------------------------------------------------------------
void SetupDialog::fontWeightChanged(int index)
{
	QComboBox * combobox = (QComboBox *)sender();
	int id = combobox->property( SYNTAX_ELT_ID ).toInt();
	
	int weight = combobox->itemData( index ).toInt();
	QColor color = mFontColorMap[id]->property( BUTTON_COLOR ).value<QColor>();
	
	mMainWindow->setHighlighter( id , color , weight );
}

//-------------------------------------------------------------------------
void SetupDialog::voiceStaffSetup(int)
{
	fStaffNumEdit->setEnabled( !fShowAllStaffsCheckBox->checkState() );
	fVoiceNumEdit->setEnabled( !fShowAllVoicesCheckBox->checkState() );

	setup();
}
