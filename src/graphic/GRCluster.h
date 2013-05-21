#ifndef GRCluster_H
#define GRCluster_H

/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "GRARCompositeNotationElement.h"
#include "GRPositionTag.h"

class ARCluster;
class ARNoteFormat;
class GRStaff;
class GRSingleNote;

/** \brief Graphical representation of a cluster.
*/
class GRCluster : public GRARCompositeNotationElement,
	              public GRPositionTag
{
public:

				 GRCluster(GRStaff * stf, ARCluster * arcls, GRSingleNote *sngNote, ARNoteFormat * curnoteformat);
	virtual	 	~GRCluster();

	virtual void OnDraw(VGDevice &hdc) const;
	virtual void tellPosition( GObject * caller, const NVPoint & np );

	virtual void addAssociation( GRNotationElement * p );
    virtual GuidoPos AddTail( GRNotationElement * el );

    void setSecondNoteYPosition();

    virtual const unsigned char * getColRef() const { return mColRef; }

    int getNoteCount() const { return gNoteCount;}

protected:

    float gdx;
    float gdy;
    float ghdx;
    float ghdy;
    float gSize;

    float gFirstNoteYPosition;
    float gSecondNoteYPosition;
    GRStaff *gStaff;
    ARCluster *gARCluster;
    TYPE_DURATION gDuration;

    unsigned char *gClusterColor;

    int gNoteCount;
};

#endif


