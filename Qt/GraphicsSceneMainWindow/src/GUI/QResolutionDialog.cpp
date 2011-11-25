/*
 * QResolutionDialog.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "QResolutionDialog.h"

#include "ui_ResolutionDialog.h"

#include <QTextStream>

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
