/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___TagParametersMaps___
#define ___TagParametersMaps___

#include "TagParameterStrings.h"
#include "TagParameterMap.h"

typedef struct TagParametersMaps {
	const TagParameterMap sARAccelerandoMap;
	const TagParameterMap sARAccidentalMap;
	const TagParameterMap sARAccoladeMap;
	const TagParameterMap sARAlterMap;
	const TagParameterMap sARArticulationMap;
	const TagParameterMap sARArpeggioMap;
	const TagParameterMap sARAutoMap;
	const TagParameterMap sARBarMap;
	const TagParameterMap sARBarFormatMap;
	const TagParameterMap sARBeamMap;
	const TagParameterMap sARBembelMap;
	const TagParameterMap sARBowingMap;
	const TagParameterMap sARClefMap;
	const TagParameterMap sARClusterMap;
	const TagParameterMap sARColorMap;
	const TagParameterMap sARComposerMap;
	const TagParameterMap sARCueMap;
	const TagParameterMap sARDisplayDurationMap;
	const TagParameterMap sARDrHoosMap;
	const TagParameterMap sARDrRenzMap;
	const TagParameterMap sARDynamicMap;
	const TagParameterMap sARFeatheredBeamMap;
	const TagParameterMap sARFermataMap;
	const TagParameterMap sARFontAbleMap;
	const TagParameterMap sARGlissandoMap;
	const TagParameterMap sARGraceMap;
	const TagParameterMap sARInstrumentMap;
	const TagParameterMap sARIntensMap;
	const TagParameterMap sARJumpMap;
	const TagParameterMap sARKeyMap;
	const TagParameterMap sARLyricsMap;
	const TagParameterMap sARMarcatoMap;
	const TagParameterMap sARMarkMap;
	const TagParameterMap sARMeterMap;
	const TagParameterMap sARMusicalTagMap;
	const TagParameterMap sARNoteFormatMap;
	const TagParameterMap sAROctavaMap;
	const TagParameterMap sARPageFormatMap;
	const TagParameterMap sARPizzicatoMap;
	const TagParameterMap sARRitardandoMap;
	const TagParameterMap sARSpaceMap;
	const TagParameterMap sARSpecialMap;
	const TagParameterMap sARStaccatoMap;
	const TagParameterMap sARStaffMap;
	const TagParameterMap sARStaffFormatMap;
	const TagParameterMap sARSymbolMap;
	const TagParameterMap sARTStemMap;
	const TagParameterMap sARTempoMap;
	const TagParameterMap sARTextMap;
	const TagParameterMap sARTextHarmonyMap;
	const TagParameterMap sARTitleMap;
	const TagParameterMap sARTremoloMap;
	const TagParameterMap sARTrillMap;
	const TagParameterMap sARTupletMap;
	const TagParameterMap sARUnitsMap;
	const TagParameterMap sARVoltaMap;
	
	TagParametersMaps() :
			sARAccelerandoMap (kARAccelerandoParams),
			sARAccidentalMap (kARAccidentalParams),
			sARAccoladeMap (kARAccoladeParams),
			sARAlterMap (kARAlterParams),
			sARArticulationMap (kARArticulationParams),
			sARArpeggioMap (kARArpeggioParams),
			sARAutoMap (kARAutoParams),
			sARBarMap (kARBarParams),
			sARBarFormatMap (kARBarFormatParams),
			sARBeamMap (kARBeamParams),
			sARBembelMap (kARBembelParams),
			sARBowingMap (kARBowingParams),
			sARClefMap (kARClefParams),
			sARClusterMap (kARClusterParams),
			sARColorMap (kARColorParams),
			sARComposerMap (kARComposerParams),
			sARCueMap (kARCueParams),
			sARDisplayDurationMap (kARDisplayDurationParams),
			sARDrHoosMap (kARDrHoosParams),
			sARDrRenzMap (kARDrRenzParams),
			sARDynamicMap (kARDynamicParams),
			sARFeatheredBeamMap (kARFeatheredBeamParams),
			sARFermataMap (kARFermataParams),
			sARFontAbleMap (kARFontAbleParams),
			sARGlissandoMap (kARGlissandoParams),
			sARGraceMap (kARGraceParams),
			sARInstrumentMap (kARInstrumentParams),
			sARIntensMap (kARIntensParams),
			sARJumpMap (kARJumpParams),
			sARKeyMap (kARKeyParams),
			sARLyricsMap (kARLyricsParams),
			sARMarcatoMap (kARMarcatoParams),
			sARMarkMap (kARMarkParams),
			sARMeterMap (kARMeterParams),
			sARMusicalTagMap (kCommonParams),
			sARNoteFormatMap (kARNoteFormatParams),
			sAROctavaMap (kAROctavaParams),
			sARPageFormatMap (kARPageFormatParams),
			sARPizzicatoMap (kARPizzicatoParams),
			sARRitardandoMap (kARRitardandoParams),
			sARSpaceMap (kARSpaceParams),
			sARSpecialMap (kARSpecialParams),
			sARStaccatoMap (kARStaccatoParams),
			sARStaffMap (kARStaffParams),
			sARStaffFormatMap (kARStaffFormatParams),
			sARSymbolMap (kARSymbolParams),
			sARTStemMap (kARTStemParams),
			sARTempoMap (kARTempoParams),
			sARTextMap (kARTextParams),
			sARTextHarmonyMap (kARTextHarmonyParams),
			sARTitleMap (kARTitleParams),
			sARTremoloMap (kARTremoloParams),
			sARTrillMap (kARTrillParams),
			sARTupletMap (kARTupletParams),
			sARUnitsMap (kARUnitsParams),
			sARVoltaMap (kARVoltaParams) {}
	
} TagParametersMaps;

extern const TagParametersMaps* gMaps;

#endif
