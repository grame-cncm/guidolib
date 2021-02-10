cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

#######################################
set (SRCDIR  		${CMAKE_CURRENT_LIST_DIR}/src)
set (GUIDOROOT  	${CMAKE_CURRENT_LIST_DIR}/../../../..)
set (GUIDOQTLIBDIR 	${CMAKE_CURRENT_LIST_DIR}/../../libs)
set (GUIDOQTDIR 	${GUIDOQTLIBDIR}/GuidoQt)
set (GUIDOGSDIR 	${GUIDOQTLIBDIR}/GraphicsSceneMainWindow)
set (RSRCDIR		${CMAKE_CURRENT_LIST_DIR}/rsc)

find_package(Qt5 COMPONENTS Xml Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Xml_INCLUDE_DIRS} ${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
set (QTLIBS 	Qt5::Xml Qt5::Widgets Qt5::PrintSupport)

#######################################
# set sources and headers files
file (GLOB GUIDOSCENECOMPOSER_SRC  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	${SRCDIR}/*/*.cpp
	${SRCDIR}/*.cpp
) 
file (GLOB GUIDOSCENECOMPOSER_HEADERS  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRCDIR}/*/*.h) 

#######################################
# set includes
set (GUIDOSCENECOMPOSER_INCLUDE_DIRS 
	${GUIDOQTDIR}/include 
	${GUIDOGSDIR}/include
	${SRCDIR}/GUI 
	${SRCDIR}/GuidoItemContainer 
	${SRCDIR}/GuidoHighlighter 
	${QTINCDIRS}
)

set (GUIDOSCENECOMPOSER_DEFINITIONS -DGUIDO_SCENE_COMPOSER_APP)

#######################################
# set libraries and definitions
if (WIN32)
	set (GUIDOSCENECOMPOSER_LIBRARIES ${GUIDOQTLIBDIR}/GuidoQt.lib ${GUIDOQTLIBDIR}/GraphicsSceneMainWindow.lib)
else()
	set (GUIDOSCENECOMPOSER_LIBRARIES -L${GUIDOQTLIBDIR} -lGuidoQt -lGraphicsSceneMainWindow)
endif()
set (GUIDOSCENECOMPOSER_LIBRARIES ${GUIDOSCENECOMPOSER_LIBRARIES} ${QTLIBS})

#######################################
# ui and resources
set (GUIDOSCENECOMPOSER_SRC ${GUIDOSCENECOMPOSER_SRC} ${RSRCDIR}/guido.qrc)
set (GUIDOSCENECOMPOSER_INFO_PLIST ${RSRCDIR}/GuidoSceneComposerInfo.plist)
set (GUIDOSCENECOMPOSER_ICON_FILE guido.icns)
set (RSRC "${RSRCDIR}/guido.icns" "${RSRCDIR}/English.lproj/InfoPlist.strings")
set_source_files_properties(${RSRC} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set (FONT ${GUIDOROOT}/src/guido2.svg ${GUIDOROOT}/src/guido2.ttf)
set_source_files_properties(${FONT} PROPERTIES MACOSX_PACKAGE_LOCATION "Fonts")

set (GUIDOSCENECOMPOSER_SRC ${GUIDOSCENECOMPOSER_SRC} ${RSRC} ${FONT})

if (WIN32)
	enable_language(RC)
	set(GUIDOSCENECOMPOSER_SRC ${GUIDOSCENECOMPOSER_SRC} ${RSRCDIR}/GuidoSceneComposer.rc)
	set(GUIDOSCENECOMPOSER_EXE WIN32)
elseif(APPLE)
	set(GUIDOSCENECOMPOSER_EXE MACOSX_BUNDLE)
endif()

message (STATUS "Include GuidoSceneComposer using Qt version ${Qt5Widgets_VERSION}")
