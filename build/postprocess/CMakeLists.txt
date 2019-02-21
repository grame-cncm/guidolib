#######################################
# install post processing
#######################################
project(guidotools)
cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6)

set (CMAKE_BUILD_TYPE Release)

#######################################
# install setup
if (APPLE OR WIN32)
if (APPLE)
	set (DEPLOY macdeployqt)
	set ( GUIDOEDITOR GuidoEditor.app)
	set ( GUIDOSCENECOMPOSER GuidoSceneComposer.app)
else()
	set (DEPLOY windeployqt)
	set ( GUIDOEDITOR GuidoEditor.exe)
	set ( GUIDOSCENECOMPOSER GuidoSceneComposer.exe)
endif()

set (BIN ${CMAKE_INSTALL_PREFIX}/bin)
install ( CODE
	"execute_process ( COMMAND echo ${BIN}/${GUIDOEDITOR} ${BIN}/${GUIDOSCENECOMPOSER} )"
)
install ( CODE
	"execute_process ( COMMAND ${DEPLOY} ${BIN}/${GUIDOEDITOR} )"
)
install ( CODE
	"execute_process ( COMMAND ${DEPLOY} ${BIN}/${GUIDOSCENECOMPOSER} )"
)
endif()

