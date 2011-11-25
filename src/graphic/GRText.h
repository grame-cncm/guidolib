#ifndef GRText_H
#define GRText_H

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

#include <string>

#include "GRPTagARNotationElement.h"

class ARText;
class GRStaff;
class GRRod;

/** \brief A text string.

	Can represent standard text, lyrics, fingering, label, cue-text, marks...

*/
class GRText : public GRPTagARNotationElement
{	
	public:
		class GRTextSaveStruct : public GRPositionTag::GRSaveStruct
		{
			public:
				
						GRTextSaveStruct()  {}
				virtual ~GRTextSaveStruct() {};
				
				NVPoint position;
				NVRect boundingBox;
				std::string text;
		};

						GRText( GRStaff *, ARText * abstractRepresentationOfText );
		virtual 		~GRText();

		virtual void 	removeAssociation( GRNotationElement * el );
		virtual void 	tellPosition( GObject * caller, const NVPoint & inPosition );
		virtual void 	addAssociation( GRNotationElement * el );
		
		
		virtual void 	OnDraw( VGDevice & hdc ) const;
		
		virtual void 	print() const;
		
		const ARText * 	getARText() const;
		
		virtual unsigned int getTextAlign() const { return mTextAlign; }

		virtual float 	getLeftSpace() const;
		virtual float 	getRightSpace() const;

		virtual void 	setPosition(const NVPoint & inPosition );
		virtual void 	setHPosition( float nx );
				void	mustFollowPitch( bool flag ) { mMustFollowPitch = flag; }

	protected:

		virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()
			{ return new GRTextSaveStruct; }	

		unsigned int mTextAlign;
		bool	mMustFollowPitch; // (when the text tag has a range)
};

#endif

