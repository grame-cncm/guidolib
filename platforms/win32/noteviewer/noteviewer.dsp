# Microsoft Developer Studio Project File - Name="noteviewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=noteviewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "noteviewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "noteviewer.mak" CFG="noteviewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "noteviewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "noteviewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "noteviewer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../src/include" /I "../../src/tools" /I "../src" /I "../../src/musedata2guido" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "VC6" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 vfw32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"../noteviewer.exe"

!ELSEIF  "$(CFG)" == "noteviewer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../src/include" /I "../../src/tools" /I "../src" /I "../../src/musedata2guido" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "VC6" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vfw32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../noteviewer.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "noteviewer - Win32 Release"
# Name "noteviewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\tools\CGuidoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\childfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportGifDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\gmntextdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\gmnview32.cpp
# End Source File
# Begin Source File

SOURCE=.\gmnview32doc.cpp
# End Source File
# Begin Source File

SOURCE=.\gmnview32view.cpp
# End Source File
# Begin Source File

SOURCE=..\src\GuidoFeedbackWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\helpdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\M2GConvDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDConvLog.cpp
# End Source File
# Begin Source File

SOURCE=.\myhtmldoc.cpp
# End Source File
# Begin Source File

SOURCE=.\myhtmlview.cpp
# End Source File
# Begin Source File

SOURCE=.\myscrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\mystatic.cpp
# End Source File
# Begin Source File

SOURCE=.\noteviewpagewindow.cpp
# End Source File
# Begin Source File

SOURCE=.\pagebutton.cpp
# End Source File
# Begin Source File

SOURCE=.\pagedialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SpacingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\springdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\virtualpagedialog.cpp
# End Source File
# Begin Source File

SOURCE=.\zoomdialog2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\deu\bin00001.bin
# End Source File
# Begin Source File

SOURCE=.\res\deu\bin00002.bin
# End Source File
# Begin Source File

SOURCE=.\res\deu\bin00003.bin
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\deu\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\gmnview32.rc
# End Source File
# Begin Source File

SOURCE=.\res\deu\helptext.bin
# End Source File
# Begin Source File

SOURCE=.\res\deu\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\deu\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\deu\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=".\res\deu\sal-nv.ico"
# End Source File
# Begin Source File

SOURCE=".\res\sal-nv.ico"
# End Source File
# Begin Source File

SOURCE=.\res\deu\text1.bin
# End Source File
# End Group
# Begin Group "MuseData2Guido"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\musedata2guido\ConversionOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ConversionOptions.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ConversionParameters.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ConversionParameters.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ErrorHandling.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ErrorHandling.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\GuidoCreator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\GuidoCreator.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseData2Guido.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataCombineDivideRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataCombineDivideRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataDataRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataDataRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataEndRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataEndRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataFile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataMeasureLineRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataMeasureLineRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataMusicalAttributesRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataMusicalAttributesRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataNoteRecord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataNoteRecord.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataRecordList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\MuseDataRecordList.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ParseParameters.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\ParseParameters.h
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\parseutils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\musedata2guido\parseutils.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\deu\html1.htm
# End Source File
# Begin Source File

SOURCE=.\res\deu\readme.txt
# End Source File
# Begin Source File

SOURCE=..\guidolib\GUIDOEngine.lib
# End Source File
# Begin Source File

SOURCE=.\libs\guido2gif.lib
# End Source File
# Begin Source File

SOURCE=.\libs\guido2midi.lib
# End Source File
# Begin Source File

SOURCE=.\libs\midi2gmn.lib
# End Source File
# End Target
# End Project
