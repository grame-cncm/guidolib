#ifndef GRMusic_H
#define GRMusic_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>

#include "GREvent.h"
#include "GUIDOEngine.h"		// for GuidoOnDrawDesc
#include "TCollisions.h"
#include "GRVisitor.h"

class ARMusic;
class ARPageFormat;
class ARMusicalVoice;

class GRVoice;
class GRSystem;
class GRPage;
class GRMusic;
class TCollisions;

typedef std::vector<GRPage *> PageList;
typedef std::vector<GRVoice *> VoiceList;

/** \brief Graphical representation of the music. It handles a list of voices
	and a list of pages.
*/
class GRMusic : public GREvent
{
	public:
		using GObject::GetMap;
		
				GRMusic(const ARMusic * inARMusic, const ARPageFormat * inFormat, const GuidoLayoutSettings *settings, bool ownsAR );
		virtual ~GRMusic();

				void 		startNewSystem( GRSystem * grsystem );
	 	 		void 		setName( const char * in )	 { mName = in; }
	  			const 		NVstring & getName();

				void 		createGR (const ARPageFormat * inPageFormat = 0 , const GuidoLayoutSettings *settings = 0);

				const ARMusic * getARMusic() const;
	     		const ARMusic * getconstARMusic() const;

		// - Page related
	  	virtual const GRPage * getPage( int inPageNum ) const;
	  	virtual GRPage *	getPage( int inPageNum );
	  	virtual	int			getPageIndex( const GRPage * inPage ) const;
		virtual	bool 		getRTPofPage( int inPageNum, int * num, int * denom ) const;
		virtual	int 		getPageNum( int num, int denom ) const;		// must match an existing element
		virtual	int 		getPageNumForTimePos( int num, int denom ) const;

		virtual int 		getNumPages() const;
		virtual int 		getNumSystems(int page) const;

		// - Voice related
		void MarkVoice(int voicenum,int numfrom,int denomfrom,
			int numlength,int denomlength, unsigned char red, unsigned char green, unsigned char blue);
		virtual void rememberVoiceNLinePosition		(const ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp);
		virtual void setPossibleVoiceNLinePosition	(const ARMusicalVoice * arv, const TYPE_TIMEPOSITION & tp);
		
		void	removeVoiceElement(ARMusicalVoice * arv,GRNotationElement * el);
		int		getNumVoices() const;
		void	setSpringParameter(float npar);

	  	GRVoice * getVoice( int num );
	 	GRVoice * getVoice( const ARMusicalVoice * arv );
	  
	  	void addVoiceElement( int num, GRNotationElement * el );
	  	void addVoiceElement( const ARMusicalVoice * arv, GRNotationElement * el );

		// - Graphic and sizes
		virtual void	updateBoundingBox();
				void	adjustPageSize();
		virtual float	getPageWidth( int inPageNum ) const;
		virtual float	getPageHeight( int inPageNum ) const;

		virtual void	DrawMusic( VGDevice & hdc, const GuidoOnDrawDesc & inDrawInfos );
		virtual void	GetMap( int inPage, float w, float h, GuidoElementSelector sel, MapCollector& f ) const;

		virtual void	print(std::ostream& os) const;
		virtual void	trace(VGDevice & hdc);
		virtual void	pagetrace(VGDevice & hdc);
		virtual void	voicetrace(VGDevice & hdc);

				int 	getVoiceNum( ARMusicalVoice * arvoice ) const; 
				void 	addPage( GRPage * newPage );
                
                void    setAR2GRTime(long time) { mAR2GRTime = time; }
                long    getAR2GRTime()          { return mAR2GRTime; }
                void    setDrawTime(long time)  { mDrawTime = time; }
                long    getDrawTime()           { return mDrawTime; }

				float	getNotesDensity () const;
				bool	checkLyricsCollisions ();		// checks lyrics collisions and resolves the collisions if any
				bool	collides () const					{ return !fCollisions.list().empty(); }
				bool	lyricsChecked () const				{ return fLyricsChecked; }
				void	removeAutoSpace (ARMusic * arm);// removes space tags inserted by checkLyricsCollisions()

				void	printVoices (std::ostream& os) const;
    
                void    setStaffSize(int staffNum, float size);
                float   getStaffSize(int staffNum);

		std::vector<TCollisionInfo> getCollisions() const	{ return fCollisions.list(); }

		virtual void accept   (GRVisitor& visitor);

	protected:
				void 	addVoiceElement( GRVoice * voice, GRNotationElement * el );

	  	NVstring 	mName;
		VoiceList 	mVoiceList;

        long        mAR2GRTime;
        long        mDrawTime;

	private:
				size_t		checkCollisions (bool lyrics);
				void		resolveCollisions (std::vector<TCollisionInfo> list);

		ARMusicalVoice*		getARVoice (int n);
		std::vector<TCollisionInfo> strip (const std::vector<TCollisionInfo>& list) const;

		PageList			mPages;
		ARPageFormat *		fInFormat;
        std::map<int,float> fStaffSizes;
		GuidoLayoutSettings fSettings;
		TCollisions			fCollisions;
		bool				fLyricsChecked;		// true when lyrics collisions have been solved
};


#endif


