/*
 * QLanguageCommandPaletteLayout.h
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
    Qt::Orientations expandingDirections() const;

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
