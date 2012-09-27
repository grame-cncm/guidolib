#ifndef GRMusic_H
#define GRMusic_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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

#include <vector>

#include "GREvent.h"
#include "GUIDOEngine.h"		// for GuidoOnDrawDesc

class ARMusic;
class ARPageFormat;
class ARMusicalVoice;

class GRVoice;
class GRSystem;
class GRPage;
class GRMusic;

typedef std::vector<GRPage *> PageList;
typedef std::vector<GRVoice *> VoiceList;

extern GRMusic * gCurMusic;
extern long ggsoffsetx;
extern long ggsoffsety;

/** \brief Graphical representation of the music. It handles a list of voices
	and a list of pages.
*/
class GRMusic : public GREvent
{
	public:
		using GObject::GetMap;
		
				GRMusic( ARMusic * inARMusic, ARPageFormat * inFormat, bool ownsAR ); 
		virtual ~GRMusic();

				void 		startNewSystem( GRSystem * grsystem );
		virtual void 		getGuido() const;
	 	 		void 		setName( const char * in )	 { mName = in; }
	  			const 		NVstring & getName();

	  			void 		createGR(ARPageFormat * inPageFormat = 0 );

				ARMusic * 	getARMusic();
	     		const ARMusic * getconstARMusic() const;

		// - Page related
	  	virtual const GRPage * getPage( int inPageNum ) const;
	  	virtual GRPage *	getPage( int inPageNum );
	  	virtual	int			getPageIndex( const GRPage * inPage ) const;
		virtual	bool 		getRTPofPage( int inPageNum, int * num, int * denom ) const;
		virtual	int 		getPageNum( int num, int denom ) const;		// must match an existing element
		virtual	int 		getPageNumForTimePos( int num, int denom ) const;

		virtual int 		getNumPages() const;

		// - Voice related
		void MarkVoice(int voicenum,int numfrom,int denomfrom,
			int numlength,int denomlength);
		virtual void rememberVoiceNLinePosition(ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp);
		virtual void setPossibleVoiceNLinePosition(ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp);
		
		void	removeVoiceElement(ARMusicalVoice * arv,GRNotationElement * el);
		int		getNumVoices() const;
		void	setSpringParameter(float npar);

	  	GRVoice * getVoice( int num );
	 	GRVoice * getVoice( ARMusicalVoice * arv );
	  
	  	void addVoiceElement( int num, GRNotationElement * el );
	  	void addVoiceElement( ARMusicalVoice * arv, GRNotationElement * el );

		// - Graphic and sizes
		virtual void	updateBoundingBox();
				void	adjustPageSize();
		virtual float	getPageWidth( int inPageNum ) const;
		virtual float	getPageHeight( int inPageNum ) const;

		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual void	OnDraw( VGDevice & hdc, const GuidoOnDrawDesc & inDrawInfos );
		virtual void	GetMap( int inPage, float w, float h, GuidoeElementSelector sel, MapCollector& f ) const;

		virtual void	GGSOutputPage( int inPageNum ) const;
		virtual int		GGSInputPage( int inPageNum, const char * str);
		virtual char *	getGGSInfo( int infotype ) const;

		virtual void	print() const;
		virtual void	trace(VGDevice & hdc);
		virtual void	voicetrace(VGDevice & hdc);

				void 	setOptForce( float newoptforce );
				float 	getOptForce();
				int 	getVoiceNum( ARMusicalVoice * arvoice ) const; 
				void 	addPage( GRPage * newPage );

	protected:
				void 	addVoiceElement( GRVoice * voice, GRNotationElement * el );

	  	NVstring 	mName;
		VoiceList 	mVoiceList;

	private:  
		PageList 	mPages;
};


#endif


