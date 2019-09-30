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


class ARBow;
class ARMusicalTag;
class GREvent;
class GRStaff;


/** \brief Articulations: staccato, accent, marcato,
			tenuto, fermata, breath-mark, harmonics
*/

class GRArticulation : public GRTagARNotationElement
{
  public:
		enum {  kFlagStaccato = 1, kFlagStaccmo = 2, kFlagTenuto = 4,
				kFlagAccent = 8, kFlagMarcato = 16, kFlagMarcatoUp = 32,
                kFlagMarcatoDown = 64, kFlagFermataUp = 128, kFlagFermataDown = 256,
                kFlagBreathMark = 512, kFlagPizz = 1024, kFlagHarmonic = 2048, kFlagBow = 4096 };


				 GRArticulation( const ARMusicalTag * inTag, float curLSPACE, bool ownsar = false );
		virtual ~GRArticulation() {}

		virtual const NVPoint & getReferencePosition() const;
		virtual void setPosition(const NVPoint & point);

		// this will be done by the event to which
		// the staccato belongs ....
		virtual void tellPosition(GObject * caller, const NVPoint & inPos);

		virtual void accept (GRVisitor& visitor);
		virtual void OnDraw( VGDevice & hdc ) const;

		int	 	getArticulationType() const		{ return mArticulationFlag; }
		void 	print(std::ostream& os) const;
	
		static bool  compare (GRArticulation* i, GRArticulation* j) 	{ return (i->getArticulationOrder() < j->getArticulationOrder()); }

	protected:
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
		void 	placeBow		 ( const GREvent * inParent, NVPoint & ioPos );
		void 	placeBowBelow	 ( const GREvent * inParent, NVPoint & ioPos );
		void 	placeBowAbove	 ( const GREvent * inParent, NVPoint & ioPos );

		int		getPlacement( const GREvent * inParent ) const;		// gives an ARArticulation placement

		int		mArticulationFlag;

		static std::map<int,int> sOrdering;

	private:
		unsigned int 	getSymbol				( const ARMusicalTag * inTag ) const;
		int 			getArticulationFlag		( const ARMusicalTag * inTag ) const;
		NVPoint 		getReferencePosition 	( unsigned int symbol) const;
		float 			getSymbolHeight			( unsigned int symbol) const;
		void			setArticulationSymbol	( unsigned int symbol );
		int	 			getArticulationOrder() const	{ return sOrdering[mArticulationFlag]; }
		int	 			getARPlacement() const;			// gives the ARArticulation position

		double	resolveCollisionAbove ( const GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	resolveCollisionBelow ( const GREvent * inParent, double currentpos, float minspace, int skiptypes) const;
		double	staffBottom (const GRStaff * staff) const;
		bool	onStaffLine (const GRStaff * staff, double pos) const;
		void	initOrder ();
	
		NVPoint fRefPos;
};

#endif
