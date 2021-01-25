/*
 * QPRollGraphicsItem
 *
 * Copyright 2015 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */

#ifndef QPRollGraphicsItem_H
#define QPRollGraphicsItem_H

#include <QGraphicsRectItem>

#include "QGuidoPainter.h"
#include "GUIDOPianoRoll.h"

namespace inscore
{

/**
*	\brief A QGraphicsItem displaying a Guido piano roll.
*/
class QPRollGraphicsItem : public QGraphicsRectItem
{
	public:
				 QPRollGraphicsItem(QGraphicsItem * parent = 0);
		virtual ~QPRollGraphicsItem();
		
		virtual void		paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget=0);
	
				void		setPianoRoll(PianoRoll * pRoll);
        const PianoRoll*	getPianoRoll() const { return fPianoRoll; }
				QString		gmnCode() const;
				bool		setGMNCode(const QString& gmnCode);
				bool		setGMNStream(GuidoStream * gmnStream);
				QString		getLastErrorMessage() const;

	protected:
		PianoRoll *		fPianoRoll;
		QGuidoPainter * fGuidoPainter;
};
}

#endif
