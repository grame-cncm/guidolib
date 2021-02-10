/*
 * QLanguageCommandPaletteLayout.h
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
 
#ifndef LANGUAGE_COMMAND_PALETTE_LAYOUT_H
#define LANGUAGE_COMMAND_PALETTE_LAYOUT_H

#include <QLayout>
#include <QRect>
#include <QWidgetItem>

class QLanguageCommandPaletteLayout : public QLayout
{
public:
    QLanguageCommandPaletteLayout(QWidget *parent, int margin = 0, int spacing = -1);
    QLanguageCommandPaletteLayout(int spacing = -1);
    ~QLanguageCommandPaletteLayout();

    void addItem(QLayoutItem *item);
//    Qt::Orientations expandingDirections() const;

    int				count() const;
    QLayoutItem *	itemAt(int index) const;
    QSize			minimumSize() const;
    void			setGeometry(const QRect &rect);
    QSize			sizeHint() const;
    QLayoutItem *	takeAt(int index);

private:
    int doLayout(const QRect &rect, bool testOnly) const;

    QList<QLayoutItem *> mItemList;
};

#endif //GUIDO_COMMAND_PALETTE_LAYOUT_H
