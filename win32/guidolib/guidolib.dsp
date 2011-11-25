# Microsoft Developer Studio Project File - Name="guidolib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=guidolib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "guidolib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "guidolib.mak" CFG="guidolib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "guidolib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "guidolib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "guidolib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUIDOLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../../src/include" /I "../../src/abstract" /I "../../src/graphic" /I "../../src/lib" /I "../../src/misc" /I "../../src/parser" /I "../../src/tools" /D "NDEBUG" /D "WIN32" /D "GUIDOExport" /D "VC6" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib msimg32.lib /nologo /dll /machine:I386 /out:"../GUIDOEngine.dll" /implib:"GUIDOEngine.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "guidolib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUIDOLIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "../../src/include" /I "../../src/abstract" /I "../../src/graphic" /I "../../src/lib" /I "../../src/misc" /I "../../src/parser" /I "../../src/tools" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUIDOExport" /D "VC6" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib msimg32.lib /nologo /dll /debug /machine:I386 /out:"../GUIDOEngine.dll" /implib:"GUIDOEngine.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "guidolib - Win32 Release"
# Name "guidolib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "abstract"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\abstract\ARABreak.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAcc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAccelerando.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAccent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAccol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAlter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARAuto.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBarFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBeam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBembel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBowing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARBreathMark.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARChord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARChordComma.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARChordTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARClef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARCoda.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARComposer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARCrescEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARCrescendo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARCue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDaCapo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDaCoda.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDalSegno.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDefineTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDimEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDiminuendo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDisplayDuration.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDotFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDoubleBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDrHoos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDrRenz.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARDynamics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARFermata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARFine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARFingering.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARFinishBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARGrace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARInstrument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARIntens.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARJump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARLyrics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMarcato.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMark.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMeter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMTParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusicalEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusicalObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusicalTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusicalVoice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARMusicalVoiceState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARNaturalKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARNewSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARNote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARNoteFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARNoteName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\AROctava.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARPageFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRangeEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRepeatBegin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRepeatEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRestFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARRitardando.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARSegno.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARShareLocation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARShareStem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARSlur.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARSpecial.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARStaccato.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARStaff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARStaffFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARSystemFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTAccent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTagBegin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTagEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTArticulation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTBowing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTempo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTenuto.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTHead.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTie.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTitle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTPortato.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTremolo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTrill.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTStaccato.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTStem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARTuplet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARUnits.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARUserChordTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARVoiceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\ARVolta.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\Fraction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\MoreMusicalTags.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\TagParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\TagParameterFloat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\TagParameterInt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\TagParameterList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\abstract\TagParameterString.cpp
# End Source File
# End Group
# Begin Group "graphic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\graphic\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRAccelerando.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRAccidental.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRAccolade.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GraphTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRARCompositeNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRARNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRArticulation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBarFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBeam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBembel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBowing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBreakMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRBreathMark.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRChord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRChordTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRClef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRCoda.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRComposedNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRCompositeNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRCrescendo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDiminuendo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDoubleBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDrHoos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDrRenz.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRDynamics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GREmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GREvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRFinishBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRFlag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRGlobalLocation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRGlobalStem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRGlue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRGrace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRInstrument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRIntens.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRJump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRMeter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRMusic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRMusicalObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRNewTuplet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRNote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRNoteDot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRNoteHead.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRPageText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRPositionTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRPossibleBreakState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRPTagARNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRange.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRepeatBegin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRepeatEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRitardando.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRRod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSegno.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSimpleBeam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSingleNote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSingleRest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSliceHeight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSlur.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpaceForceFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpacingMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpecial.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpringCollider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSpringForceIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRStaff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRStaffFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRStaffManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRStdNoteHead.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRStem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSystemSlice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSystemTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRSystemTagInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTagARNotationElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTempo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTie.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTremolo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTrill.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRTuplet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRVoice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRVoiceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\GRVolta.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\Image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\NEList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\NEPointerList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\NVPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\graphic\NVRect.cpp
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\lib\FormatStringParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOEngineObsolete.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOEngineParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOInternal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\GUIDOScoreMap.cpp
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\misc\nvstring.cpp
# End Source File
# End Group
# Begin Group "parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\parser\gmntools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\parser\guido.cpp
# End Source File
# End Group
# Begin Group "tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\tools\ScoreSymbolMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\tools\SVGSystem.cpp
# End Source File
# End Group
# Begin Group "devices"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\tools\DeviceExporter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\tools\GDevicePostScript.cpp
# End Source File
# Begin Source File

SOURCE=..\src\GDeviceWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\GFontWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\GSystemWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\GUIDOMainWin32.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\include\GDeviceDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\Guido2GIF.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\Guido2Midi.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GUIDOEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GUIDOExport.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GUIDOFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GuidoFeedback.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GuidoFeedbackStd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GUIDOScoreMap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\GUIDOTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\Midi2Guido.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\MuseData2Guido.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\MusicalSymbols.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\ScoreSymbolMap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\SVGSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\TRect.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VGColor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VGDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VGFont.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VGPen.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VGSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\src\include\VScoreSymbolMap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\GUIDOEngine.rc
# End Source File
# End Group
# End Target
# End Project
