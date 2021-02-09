#######################################
# install post processing
#######################################
project(guidotools)
cmake_minimum_required(VERSION 3.4)

set (CMAKE_BUILD_TYPE Release)

#######################################
# install setup
#######################################
if (PACK)
	set (CMAKE_INSTALL_PREFIX .)
endif()



if (APPLE OR WIN32)
if (APPLE)
	set ( DEPLOY macdeployqt)
	set ( GUIDOEDITOR GuidoEditor.app)
	set ( GUIDOSCENECOMPOSER GuidoSceneComposer.app)
else()
	set ( DEPLOY windeployqt)
	set ( GUIDOEDITOR GuidoEditor.exe)
	set ( GUIDOSCENECOMPOSER GuidoSceneComposer.exe)
endif()

if (PACK)
if (WIN32)
install ( CODE
	"execute_process ( COMMAND ${ROOT}/build/deploy.bat )"
)
endif()

else ()

set (BIN ${CMAKE_INSTALL_PREFIX}/bin)
install ( CODE
	"execute_process ( COMMAND ${DEPLOY} ${BIN}/${GUIDOEDITOR} )"
)
install ( CODE
	"execute_process ( COMMAND ${DEPLOY} ${BIN}/${GUIDOSCENECOMPOSER} )"
)
endif()
endif()
