
@echo off

set MSVC="Visual Studio 15 2017 Win64"
set VERSION="1.7.1"
set BUILDDIR="builddir"

if [%1]==[]     GOTO USAGE
set GEN=%1

@rem ------------------------------------------------
echo Compile Qt libraries
cd libs
call :QTCOMPILE
cd ..

@rem ------------------------------------------------
echo Compile Qt applications
cd apps
call :QTCOMPILE
cd ..

@rem ------------------------------------------------
echo Compile Qt tools
cd tools
call :QTCOMPILE
cd ..

:QTCOMPILE
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
echo Usage: %0 <cmake generator>
:DONE
