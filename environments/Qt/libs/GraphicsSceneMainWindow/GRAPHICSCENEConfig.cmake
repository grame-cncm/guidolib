cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

#######################################
set (ROOT 		${CMAKE_CURRENT_SOURCE_DIR}/GraphicsSceneMainWindow)
set (SRCDIR  	${ROOT}/src)
set (GUIDODIR  	${ROOT}/../../..)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Xml Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Xml_INCLUDE_DIRS} ${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})

#######################################
# set sources and headers files
file (GLOB SRC  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRCDIR}/*/*.cpp )
file (GLOB GRAPHICSCENE_HEADERS  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	${ROOT}/include/*.h 
	${SRCDIR}/*/*.h
)

#######################################
# set includes
set (INCDIRS 
	${ROOT}
	${ROOT}/include 
	${SRCDIR}/LanguageCommandPalette
)
set (GRAPHICSCENE_INCLUDE_DIRS ${INCDIRS} ${QTINCDIRS})

#qt5_wrap_ui(UI ${SRCDIR}/GUI/ResolutionDialog.ui)
#qt5_add_resources (RSRC ${ROOT}/rsc/application.qrc)

set (UI ${SRCDIR}/GUI/ResolutionDialog.ui)
set (RSRC ${ROOT}/rsc/application.qrc)

set (GRAPHICSCENE_SRC ${SRC} ${UI} ${RSRC})

message (STATUS "GraphicsSceneMainWindow library using Qt version ${Qt5Widgets_VERSION}")
