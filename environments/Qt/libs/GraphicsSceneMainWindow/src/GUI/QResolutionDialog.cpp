/*
 * QResolutionDialog.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */

#include <QTextStream>
 
#include "QResolutionDialog.h"
#include "ui_ResolutionDialog.h"

//-------------------------------------------------------------------------
QResolutionDialog::QResolutionDialog(const QRectF& baseRect , int min , int max , int def , float convertor ) 
: QDialog()
{
	mUI = new Ui::ResolutionDialog();
	mUI->setupUi(this);
	QObject::connect( mUI->buttonBox, SIGNAL(accepted()), this, SLOT(accepted()));
	QObject::connect( mUI->scaleSlider , SIGNAL(valueChanged(int)) , this , SLOT(updateResolution(int)) );
	mBaseRect = QRectF(baseRect);
	mConvertor = convertor;
	mUI->scaleSlider->setRange( min , max );
	mUI->scaleSlider->setValue( def );
}

//-------------------------------------------------------------------------
void QResolutionDialog::accepted()	
{ 
	done( mUI->scaleSlider->value() ); 
}

//-------------------------------------------------------------------------
void QResolutionDialog::updateResolution(int v) 
{ 
	int width = v * mBaseRect.width();
	int height = v * mBaseRect.height();
	QString text;
	QTextStream stream(&text);
	stream << int(width*mConvertor) << "x" << int(height*mConvertor);
	mUI->resolutionLabel->setText( text );
}
