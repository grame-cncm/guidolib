#ifndef GRArticulation_H
#define GRArticulation_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <map>

#include "GRTagARNotationElement.h"
#include "NVPoint.h"


class GRStaff;
class ARMusicalTag;
class GREvent;


/** \brief Articulations: staccato, accent, marcato,
			tenuto, fermata, breath-mark, harmonics
*/

class GRArticulation : public GRTagARNotationElement
{
  public:
	//  enum { STACCATO, ACCENT, MARCATO, TENUTO, FERMATA, BREATHMARK };
	
		enum {  kFlagStaccato = 1, kFlagStaccmo = 2, kFlagTenuto = 4,
				kFlagAccent = 8, kFlagMarcato = 16, kFlagMarcatoUp = 32,
                kFlagMarcatoDown = 64, kFlagFermataUp = 128, kFlagFermataDown = 256,
                kFlagBreathMark = 512, kFlagPizz = 1024, kFlagHarmonic = 2048 };


				GRArticulation( ARMusicalTag * inTag,
								float curLSPACE,	
								bool ownsar = false );

		virtual ~GRArticulation();

		virtual const NVPoint & getReferencePosition() const;

		// ignore positions 
		virtual void setPosition(const NVPoint & point);

		// this will be done by the event to which
		// the staccato belongs ....
		virtual void tellPosition(GObject * caller, const NVPoint & inPos);

		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;

		
		int	 getArticulationType() const	{ return mArticulationFlag; }
		int	 getArticulationOrder() const	{ return sOrdering[mArticulationFlag]; }
		int	 getARPlacement() const;		// gives the ARArticulation position
		void print(std::ostream& os) const;

	protected:
	
		void	setupStaccato();
		void	setupStaccmo();
		void	setupLeftHPizz();
		void	setupSnapPizz();
		void	setupBuzzPizz();
		void	setupFingernailPizz();
		void	setupAccent();
		void	setupMarcato();
        void	setupMarcatoBelow();
        void	setupMarcatoAbove();
		void	setupTenuto();
		void	setupFermataUp();
		void	setupFermataDown();
		void	setupShortFermataUp();
		void	setupShortFermataDown();
		void	setupLongFermataUp();
		void	setupLongFermataDown();
		void	setupBreathMark();
		void	setupHarmonic();
		void	setArticulationSymbol( unsigned int inSymbolID );

		void	placeStaccato( GREvent * inParent, NVPoint & ioPos );
		void	placeStaccmo (GREvent * inParent, NVPoint & ioPos);
		void	placePizz(GREvent * inParent, NVPoint & ioPos);

		void	placeAccent( GREvent * inParent, NVPoint & ioPos );
		void	placeAccentAbove( GREvent * inParent, NVPoint & ioPos );
		void	placeAccentBelow( GREvent * inParent, NVPoint & ioPos );

		void	placeMarcato( GREvent * inParent, NVPoint & ioPos );
        void	placeMarcatoAbove( GREvent * inParent, NVPoint & ioPos );
        void	placeMarcatoBelow( GREvent * inParent, NVPoint & ioPos );

		void	placeTenuto( GREvent * inParent, NVPoint & ioPos );
		void	placeFermataAbove( GREvent * inParent, NVPoint & ioPos );
		void	placeFermataBelow( GREvent * inParent, NVPoint & ioPos );
		void	placeHarmonic (GREvent * inParent, NVPoint & ioPos);
		void	placeBreathMark( GREvent * inParent, NVPoint & ioPos );


		int		getPlacement( GREvent * inParent ) const;		// gives an ARArticulation placement

		void	setMarcatoDirection( bool upward );
		void	setStaccmoDirection( bool upward);

		int		mArticulationFlag;

		static NVPoint sRefposStacc;
		static NVPoint sRefposStaccmoUp;
		static NVPoint sRefposStaccmoDown;
		static NVPoint sRefposLeftHPizz;
		static NVPoint sRefposSnapPizz;
		static NVPoint sRefposBuzzPizz;
		static NVPoint sRefposFingernailPizz;
		static NVPoint sRefposAccent;
		static NVPoint sRefposMarcatoUp;
		static NVPoint sRefposMarcatoDown;
		static NVPoint sRefposTenuto;
		static NVPoint sRefposFermataUp;
		static NVPoint sRefposFermataDown;
		static NVPoint sRefposShortFermataUp;
		static NVPoint sRefposShortFermataDown;
		static NVPoint sRefposLongFermataUp;
		static NVPoint sRefposLongFermataDown;
		static NVPoint sRefposBreathMark;
		static NVPoint sRefposHarmonic;

		static std::map<int,int> sOrdering;

	private:
		double	resolveCollisionAbove (GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	resolveCollisionBelow (GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	staffBottom (const GRStaff * staff) const;
		bool	onStaffLine (const GRStaff * staff, double pos) const;

		void	initOrder ();
};

#endif
