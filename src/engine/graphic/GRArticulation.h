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
#include "GRVisitor.h"
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
		enum {  kFlagStaccato = 1, kFlagStaccmo = 2, kFlagTenuto = 4,
				kFlagAccent = 8, kFlagMarcato = 16, kFlagMarcatoUp = 32,
                kFlagMarcatoDown = 64, kFlagFermataUp = 128, kFlagFermataDown = 256,
                kFlagBreathMark = 512, kFlagPizz = 1024, kFlagHarmonic = 2048 };


				 GRArticulation( const ARMusicalTag * inTag, float curLSPACE, bool ownsar = false );
		virtual ~GRArticulation() {}

		virtual const NVPoint & getReferencePosition() const;

		// ignore positions 
		virtual void setPosition(const NVPoint & point);

		// this will be done by the event to which
		// the staccato belongs ....
		virtual void tellPosition(GObject * caller, const NVPoint & inPos);

		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void accept (GRVisitor& visitor);

		
		int	 getArticulationType() const	{ return mArticulationFlag; }
		int	 getArticulationOrder() const	{ return sOrdering[mArticulationFlag]; }
		int	 getARPlacement() const;		// gives the ARArticulation position
		void print(std::ostream& os) const;
	
		static bool  compare (GRArticulation* i, GRArticulation* j) 	{ return (i->getArticulationOrder() < j->getArticulationOrder()); }

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

		void	placeStaccato( const GREvent * inParent, NVPoint & ioPos );
		void	placeStaccmo ( const GREvent * inParent, NVPoint & ioPos);
		void	placePizz	 ( const GREvent * inParent, NVPoint & ioPos);

		void	placeAccent		( const GREvent * inParent, NVPoint & ioPos );
		void	placeAccentAbove( const GREvent * inParent, NVPoint & ioPos );
		void	placeAccentBelow( const GREvent * inParent, NVPoint & ioPos );

		void	placeMarcato	 ( const GREvent * inParent, NVPoint & ioPos );
        void	placeMarcatoAbove( const GREvent * inParent, NVPoint & ioPos );
        void	placeMarcatoBelow( const GREvent * inParent, NVPoint & ioPos );

		void	placeTenuto		 ( const GREvent * inParent, NVPoint & ioPos );
		void	placeFermataAbove( const GREvent * inParent, NVPoint & ioPos );
		void	placeFermataBelow( const GREvent * inParent, NVPoint & ioPos );
		void	placeHarmonic	 ( const GREvent * inParent, NVPoint & ioPos);
		void	placeBreathMark	 ( const GREvent * inParent, NVPoint & ioPos );


		int		getPlacement( const GREvent * inParent ) const;		// gives an ARArticulation placement

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
		double	resolveCollisionAbove ( const GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	resolveCollisionBelow ( const GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	staffBottom (const GRStaff * staff) const;
		bool	onStaffLine (const GRStaff * staff, double pos) const;

		void	initOrder ();
};

#endif
