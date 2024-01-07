/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "TagParameterStrings.h"

//---------------------------------------------
// the tags parameters strings
//---------------------------------------------
// common tags parameters strings
const char* kCommonParams		= "S,color,black,o;U,dx,0,o;U,dy,0,o;F,size,1.0,o";
const char* kARFontAbleParams	= "S,textformat,rc,o;S,font,Times,o;U,fsize,9pt,o;S,fattrib,,o";

//---------------------------------------------
// specific tags parameters strings
const char* kARAutoParams		= "S,endBar,on,o;S,pageBreak,on,o;S,systemBreak,on,o;S,clefKeyMeterOrder,on,o;S,stretchLastLine,off,o;S,stretchFirstLine,off,o;S,lyricsAutoPos,off,o;S,instrAutoPos,off,o;S,intensAutoPos,off,o;S,autoEndBar,on,o;S,autoPageBreak,on,o;S,autoSystemBreak,on,o;S,autoClefKeyMeterOrder,on,o;S,autoStretchLastLine,off,o;S,autoStretchFirstLine,off,o;S,autoInstrPos,off,o;S,autoLyricsPos,off,o;S,autoIntensPos,off,o;S,fingeringPos,,o;F,fingeringSize,,o;S,harmonyPos,,o;S,autoHideTiedAccidentals,on,o;S,resolveMultiVoiceCollisions,off,o";

const char* kARDrHoosParams		= "I,inverse,0,o";
const char* kARDrRenzParams		= "I,inverse,0,o";
const char* kARBembelParams		= "I,inverse,0,o";

const char* kARAccelerandoParams= "S,before,,o;S,after,,o;U,dx2,0hs,o;S,font,Times New Roman,o;U,fsize,10pt,o;S,textformat,lc,o";
const char* kARAccidentalParams	= "S,style,,o";
const char* kARAccoladeParams	= "I,id,,r;S,range,,r;S,type,standard,o";
const char* kARAlterParams		= "F,detune,0.0,r;S,text,,o";
const char* kARArpeggioParams	= "S,direction,,o";
const char* kARArticulationParams="S,position,,o";
const char* kARBarFormatParams	= "S,style,staff,o;S,range,,o";
const char* kARBarParams		= "S,displayMeasNum,false,o;I,measNum,,o;S,hidden,false,o;U,numDx,0,o;U,numDy,0,o";
const char* kARBeamParams		= "U,dy,0,o;U,dx1,0hs,o;U,dy1,0hs,o;U,dx2,0hs,o;U,dy2,1hs,o;U,dx3,0hs,o;U,dy3,0hs,o;U,dx4,0hs,o;U,dy4,1hs,o";
const char* kARBowParams		= "S,type,,r";
const char* kARBowingParams		= "S,curve,down,o;U,dx1,2hs,o;U,dy1,1hs,o;U,dx2,-2hs,o;U,dy2,1hs,o;F,r3,0.5,o;U,h,2hs,o";
const char* kARClefParams		= "S,type,treble,r";
const char* kARClusterParams	= "U,hdx,0hs,o;U,hdy,0hs,o";
const char* kARColorParams		= "F,red,0.0,r;F,green,0.0,r;F,blue,0.0,r;F,alpha,0.0,o";
const char* kARComposerParams	= "S,name,,r;S,pageformat,53,o;S,textformat,rb,o;U,fsize,14pt,o";
const char* kARCueParams		= "S,name,,o;U,fsize,9pt,o";
const char* kARDisplayDurationParams ="I,n,,r;I,d,,r;I,ndots,0,o";
const char* kARDynamicParams	= "U,dx1,0,o;U,dx2,0,o;U,deltaY,3,o;U,thickness,0.16,o;S,autopos,off,o";
const char* kARFeatheredBeamParams= "S,durations,,o;S,drawDuration,false,o";
const char* kARFermataParams	= "S,type,,o;S,position,above,o";
const char* kARFingeringParams	= "S,position,,o;U,fsize,10pt,o;";
const char* kARFooterParams		= "S,text,,r;S,pageformat,c6,o;S,font,Times,o;U,fsize,10pt,o;S,textformat,cc,o";
const char* kARGlissandoParams	= "U,dx1,0,o;U,dy1,0,o;U,dx2,0,o;U,dy2,0,o;S,fill,false,o;U,thickness,0.3,o";
const char* kARGraceParams		= "I,i,,o";
const char* kARHarmonyParams	= "S,text,,r;U,dy,-1,o;S,textformat,lt,o;S,font,Arial,o;U,fsize,18pt,o";
const char* kARInstrumentParams	= "S,name,,r;S,transp,,o;S,autopos,off,o;S,repeat,off,o";
const char* kARIntensParams		= "S,type,,r;S,before,,o;S,after,,o;S,font,Times,o;U,fsize,10pt,o;S,fattrib,i,o;S,autopos,off,o";
const char* kARJumpParams		= "S,m,,o;I,id,0,o";
const char* kARKeyParams		= "S,key,,r;S,hideNaturals,false,o;S,free,,o";
const char* kARLyricsParams		= "S,text,,r;U,dy,-3,o;S,textformat,ct,o;U,fsize,12pt,o;S,autopos,off,o";
const char* kARMarkParams		= "S,text,,r;S,enclosure,none,o;U,dy,0,o";
const char* kARMeterParams		= "S,type,4/4,r;S,autoBarlines,on,o;S,autoMeasuresNum,off,o;S,group,off,o;S,hidden,off,o";
const char* kARMMRestParams		= "I,count,,r";
const char* kARNoteFormatParams	= "S,style,standard,o";
const char* kAROctavaParams		= "I,i,,r;S,hidden,off,o";
const char* kARPageFormatParams	= "S,type,,r;U,w,,r;U,h,,r;U,lm,2cm,o;U,tm,5cm,o;U,rm,2cm,o;U,bm,3cm,o";
const char* kARPizzicatoParams	= "S,type,lefthand,o;S,position,,o";
const char* kARRepeatParams		= "S,hidden,false,o";
const char* kARRitardandoParams	= "S,before,,o;S,after,,o;U,dx2,0hs,o;S,font,Times New Roman,o;U,fsize,10pt,o;S,textformat,lc,o";
const char* kARSpaceParams		= "U,dd,,r";
const char* kARSpecialParams	= "S,char,,r";
const char* kARStaccatoParams	= "S,type,,o;S,position,,o";
const char* kARStaffFormatParams= "S,style,standard,o;U,size,3pt,o;F,lineThickness,0.08,o;U,distance,0hs,o";
const char* kARStaffParams		= "I,id,,r";
const char* kARSymbolParams		= "S,file,,r;S,position,mid,o;I,w,,o;I,h,,o";
const char* kARTempoParams		= "S,tempo,,r;S,bpm,,o;S,font,Times New Roman,o;S,textformat,lc,o;U,fsize,11pt,o";
const char* kARTextParams		= "S,text,,r;U,dy,-1,o;S,textformat,lt,o;U,fsize,12pt,o";
const char* kARTitleParams		= "S,name,,r;S,pageformat,c2,o;U,dy,0,o;S,font,Times,o;U,fsize,24pt,o;S,textformat,cc,o";
const char* kARTremoloParams	= "S,style,///,o;I,speed,32,o;S,pitch,,o;U,thickness,0.75,o;S,text,,o";
const char* kARTrillParams		= "S,note,,o;S,type,prall,o;F,detune,0.0,o;S,accidental,,o;I,dur,32,o;S,begin,on,o;U,adx,0hs,o;U,ady,0hs,o;S,tr,true,o;S,wavy,true,o;S,position,above,o;S,repeat,true,o";
const char* kARTStemParams		= "U,length,7.0,o";
const char* kARTupletParams		= "S,format,,r;S,position,above,o;U,dy1,0,o;U,dy2,0,o;F,lineThickness,4,o;S,bold,,o;F,textSize,1,o;S,dispNote,,o";
const char* kARUnitsParams		= "S,type,cm,r";
const char* kARVoltaParams		= "S,mark,,r;S,format,,o";


//---------------------------------------------
// tags parameters supported at language level
//---------------------------------------------
const char* kAboveStr 		= "above";
const char* kAbsTempoStr	= "abstempo";
const char* kAccidentalStr	= "accidental";
const char* kAdxStr			= "adx";
const char* kAdyStr			= "ady";
const char* kAfterStr		= "after";
const char* kAlphaStr		= "alpha";
//const char* kAnchorStr		= "anchor";
const char* kAutoBarlinesStr= "autoBarlines";
const char* kAutoMeasNumStr = "autoMeasuresNum";
const char* kAutoposStr		= "autopos";
const char* kBeginStr		= "begin";
const char* kBeforeStr 		= "before";
const char* kBelowStr 		= "below";
const char* kBlueStr		= "blue";
const char* kBmStr			= "bm";
const char* kBoldStr		= "bold";
const char* kBPMStr			= "bpm";
const char* kCharStr		= "char";
const char* kColorStr		= "color";
const char* kCountStr		= "count";
const char* kCurveStr		= "curve";
const char* kDdStr			= "dd";
const char* kDeltaYStr		= "deltaY";
const char* kDetuneStr		= "detune";
const char* kDisplayMeasNumStr= "displayMeasNum";
const char* kDispNoteStr	= "dispNote";
const char* kDirectionStr	= "direction";
const char* kDistanceStr	= "distance";
const char* kDMStr			= "dm";
const char* kDownStr		= "down";
const char* kDrawDurationStr= "drawDuration";
const char* kDStr			= "d";
const char* kDurationsStr	= "durations";
const char* kDurStr			= "dur";
const char* kDx1Str			= "dx1";
const char* kDx2Str			= "dx2";
const char* kDx3Str			= "dx3";
const char* kDx4Str			= "dx4";
const char* kDxStr			= "dx";
const char* kDy1Str			= "dy1";
const char* kDy2Str			= "dy2";
const char* kDy3Str			= "dy3";
const char* kDy4Str			= "dy4";
const char* kDyStr			= "dy";
const char* kEnclosureStr	= "enclosure";
const char* kFAttributesStr	= "fattrib";
const char* kFileStr		= "file";
const char* kFillStr		= "fill";
const char* kFontStr		= "font";
const char* kFormatStr		= "format";
const char* kFreeStr		= "free";
const char* kFSizeStr		= "fsize";
const char* kGreenStr		= "green";
const char* kGroupStr		= "group";
const char* kHdxStr			= "hdx";
const char* kHdyStr			= "hdy";
const char* kHideNaturalsStr= "hideNaturals";
const char* kHiddenStr		= "hidden";
const char* kHStr			= "h";
const char* kIDStr			= "id";
const char* kInverseStr		= "inverse";
const char* kIStr			= "i";
const char* kKeyStr			= "key";
const char* kLengthStr		= "length";
const char* kLineThicknesStr= "lineThickness";
const char* kLmStr			= "lm";
const char* kMarkStr		= "mark";
const char* kMeasNumStr		= "measNum";
const char* kModeStr		= "mode";
const char* kMStr			= "m";
const char* kNameStr		= "name";
const char* kNDotsStr		= "ndots";
const char* kNoteStr		= "note";
const char* kNStr			= "n";
const char* kNumDxStr		= "numDx";
const char* kNumDyStr		= "numDy";
const char* kPageFormatStr	= "pageformat";
const char* kPitchStr		= "pitch";
const char* kPositionStr	= "position";
const char* kR3Str			= "r3";
const char* kRangeStr		= "range";
const char* kRedStr			= "red";
const char* kRepeatStr		= "repeat";
const char* kRmStr			= "rm";
const char* kSizeStr		= "size";
const char* kSpeedStr		= "speed";
const char* kStyleStr		= "style";
const char* kTempoStr		= "tempo";
const char* kTextFormatStr	= "textformat";
const char* kTextSizeStr	= "textSize";
const char* kTextStr		= "text";
const char* kThicknesStr	= "thickness";
const char* kTmStr			= "tm";
const char* kTranspStr		= "transp";
const char* kTrStr			= "tr";
const char* kTypeStr		= "type";
const char* kUpStr			= "up";
const char* kWStr			= "w";
const char* kWavyStr		= "wavy";
const char* kYposStr		= "ypos";

// auto tag parameters
const char* kEndBarStr				= "endBar";
const char* kPageBreakStr			= "pageBreak";
const char* kSystemBreakStr			= "systemBreak";
const char* kClefKeyMeterOrderStr	= "clefKeyMeterOrder";
const char* kStretchLastLineStr		= "stretchLastLine";
const char* kStretchFirstLineStr	= "stretchFirstLine";
const char* kLyricsAutoPosStr		= "lyricsAutoPos";
const char* kInstrAutoPosStr		= "instrAutoPos";
const char* kIntensAutoPosStr		= "intensAutoPos";
const char* kAutoEndBarStr			= "autoEndBar";
const char* kAutoPageBreakStr		= "autoPageBreak";
const char* kAutoSystemBreakStr		= "autoSystemBreak";
const char* kAutoClefKeyMeterOrderStr= "autoClefKeyMeterOrder";
const char* kAutoStretchLastLineStr	= "autoStretchLastLine";
const char* kAutoStretchFirstLineStr= "autoStretchFirstLine";
const char* kAutoInstrPosStr		= "autoInstrPos";
const char* kAutoLyricsPosStr		= "autoLyricsPos";
const char* kAutoIntensPosStr		= "autoIntensPos";
const char* kHarmonyPosStr			= "harmonyPos";
const char* kFingeringPosStr		= "fingeringPos";
const char* kFingeringSizeStr		= "fingeringSize";
const char* kAutoHideTiedAccidentals = "autoHideTiedAccidentals";
const char* kAutoMultiVoiceCollisions = "resolveMultiVoiceCollisions";



