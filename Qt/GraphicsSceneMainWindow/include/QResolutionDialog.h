/*
 * QResolutionDialog.h
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
#ifndef RESOLUTION_DIALOG_H
#define RESOLUTION_DIALOG_H

#include <QDialog>

namespace Ui
{
	class ResolutionDialog;
}

//------------------------------------------------------------------------------------------------------------------------
class QResolutionDialog : public QDialog
{
	Q_OBJECT

	public:
		QResolutionDialog(const QRectF& baseRect , int min , int max , int def , float convertor );

	private Q_SLOTS:
		void accepted();
		void updateResolution(int v);

	private:
		QRectF mBaseRect;
		float mConvertor;
		Ui::ResolutionDialog * mUI;
};

#endif
