
@echo off

set MSVC="Visual Studio 16 2019"
set VERSION="1.7.5"
set BUILDDIR="guidodir"

if [%1]==[]     GOTO USAGE
if %1==all (
	CALL :ALL
	GOTO DONE
)
if %1==library (
	CALL :COMPILE
	GOTO DONE
)
if %1==Qt (
	CALL :QT
	GOTO DONE
)
if %1==deploy (
	CALL :DEPLOY
	GOTO DONE
)
if %1==install (
	CALL :INSTALL
	GOTO DONE
)
if %1==package (
	CALL :PACKAGE
	GOTO DONE
)
if %1==help 	GOTO USAGE

GOTO USAGE

@rem --- installation -------------------------------
:INSTALL
cd %BUILDDIR%
cmake --build . --config Release --target install
cd ..
EXIT /B

@rem --- packaging ----------------------------------
:PACKAGE
cd %BUILDDIR%
cmake .. -DPACK=on
cmake --build . --config Release --target package
move Guidolib-*.exe ..
cd ..
EXIT /B

@rem --- deployment ----------------------------------
:DEPLOY
	echo Call windeployqt in place
	set dest=../environments/Qt/apps/bin

	windeployqt %dest%/GuidoEditor.exe
	windeployqt %dest%/GuidoSceneComposer.exe
	windeployqt ../environments/Qt/tools/bin/GuidoQtFontViewer.exe

	echo Copy Guido library
	xcopy /y "lib\GUIDOEngine64.dll" "%dest%\"
EXIT /B

@rem --- all target ----------------------------------
:ALL
	CALL :COMPILE
	CALL :QT
	CALL :DEPLOY
EXIT /B

@rem --- Qt compilation ----------------------
:QT
	echo Compiles Qt environment
	cd ../environments/Qt
	CALL Make.bat "%MSVC%"
	cd ../../build
EXIT /B

@rem --- inscore compilation -------------------------
:COMPILE
	IF NOT EXIST %BUILDDIR% (
		echo Create %BUILDDIR% output folder
		mkdir %BUILDDIR%
	)
	cd %BUILDDIR%
	cmake .. -G %MSVC%
	cmake --build . --config Release
	cd ..
EXIT /B


:USAGE
echo Usage: %0 [COMMAND]
echo where COMMAND is in:
echo   all     : compile submodules
echo   library : compile the GUIDOEngine library
echo   Qt      : compile the Qt libraries and applications
echo   deploy  : deploy Qt applications in place
echo   install : install the guidolib package in current folder
echo   package : build the guidolib package
echo   help    : display this help
echo This script is using %MSVC% to compile, edit 
echo the script and set the MSVC variable to change. 
:DONE
