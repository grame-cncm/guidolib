/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOFactoryAdapter.h"

GUIDOFactoryAdapter::GUIDOFactoryAdapter()
{
	GuidoFactoryOpen(&factory);
}

GUIDOFactoryAdapter::~GUIDOFactoryAdapter()
{
	GuidoFactoryClose(factory);
}

GuidoErrCode GUIDOFactoryAdapter::openMusic()
{
	return GuidoFactoryOpenMusic(factory);
}

ARHandler GUIDOFactoryAdapter::closeMusic()
{
	return GuidoFactoryCloseMusic(factory);
}

GuidoErrCode GUIDOFactoryAdapter::openVoice()
{
	return GuidoFactoryOpenVoice(factory);
}

GuidoErrCode GUIDOFactoryAdapter::closeVoice()
{
	return GuidoFactoryCloseVoice(factory);
}

GuidoErrCode GUIDOFactoryAdapter::openChord()
{
	return GuidoFactoryOpenChord(factory);
}

GuidoErrCode GUIDOFactoryAdapter::closeChord()
{
	return GuidoFactoryCloseChord(factory);
}

GuidoErrCode GUIDOFactoryAdapter::insertCommata()
{
	return GuidoFactoryInsertCommata(factory);
}

GuidoErrCode GUIDOFactoryAdapter::openEvent(const std::string &inEventName)
{
	return GuidoFactoryOpenEvent(factory, inEventName.c_str());
}

GuidoErrCode GUIDOFactoryAdapter::closeEvent()
{
	return GuidoFactoryCloseEvent(factory);
}

GuidoErrCode GUIDOFactoryAdapter::addSharp()
{
	return GuidoFactoryAddSharp(factory);
}

GuidoErrCode GUIDOFactoryAdapter::addFlat()
{
	return GuidoFactoryAddFlat(factory);
}

GuidoErrCode GUIDOFactoryAdapter::setEventDots(int dots)
{
	return GuidoFactorySetEventDots(factory, dots);
}

GuidoErrCode GUIDOFactoryAdapter::setEventAccidentals(int accident)
{
	return GuidoFactorySetEventAccidentals(factory, accident);
}

GuidoErrCode GUIDOFactoryAdapter::setOctave(int octave)
{
	return GuidoFactorySetOctave(factory, octave);
}

GuidoErrCode GUIDOFactoryAdapter::setDuration(int numerator, int denominator)
{
	return GuidoFactorySetDuration(factory, numerator, denominator);
}

GuidoErrCode GUIDOFactoryAdapter::openTag(const std::string &name, long tagID)
{
	return GuidoFactoryOpenTag(factory, name.c_str(), tagID);
}

GuidoErrCode GUIDOFactoryAdapter::openRangeTag(const std::string &name, long tagID)
{
	return GuidoFactoryOpenRangeTag(factory, name.c_str(), tagID);
}

GuidoErrCode GUIDOFactoryAdapter::endTag()
{
	return GuidoFactoryEndTag(factory);
}

GuidoErrCode GUIDOFactoryAdapter::closeTag()
{
	return GuidoFactoryCloseTag(factory);
}

GuidoErrCode GUIDOFactoryAdapter::addTagParameterString(const std::string &val)
{
	return GuidoFactoryAddTagParameterString(factory, val.c_str());
}

GuidoErrCode GUIDOFactoryAdapter::addTagParameterInt(int val)
{
	return GuidoFactoryAddTagParameterInt(factory, val);
}

GuidoErrCode GUIDOFactoryAdapter::addTagParameterFloat(double val)
{
	return GuidoFactoryAddTagParameterFloat(factory, val);
}

GuidoErrCode GUIDOFactoryAdapter::setParameterName(const std::string &name)
{
	return GuidoFactorySetParameterName(factory, name.c_str());
}

GuidoErrCode GUIDOFactoryAdapter::setParameterUnit(const std::string &unit)
{
	return GuidoFactorySetParameterUnit(factory, unit.c_str());
}
