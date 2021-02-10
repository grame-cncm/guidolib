cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

#######################################
set (SRCDIR  		${CMAKE_CURRENT_LIST_DIR})
set (GUIDOROOT  	${CMAKE_CURRENT_LIST_DIR}/../../../..)
set (GUIDOQTLIBDIR 	${CMAKE_CURRENT_LIST_DIR}/../../libs)
set (GUIDOQTDIR 	${GUIDOQTLIBDIR}/GuidoQt)
set (RSRCDIR		${SRCDIR}/rsc)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
set (QTLIBS 	Qt5::Widgets Qt5::PrintSupport)

#######################################
# set sources and headers files
file (GLOB GUIDOEDITOR_SRC  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRCDIR}/*/*.cpp) 
file (GLOB GUIDOEDITOR_HEADERS  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRCDIR}/*/*.h) 

#######################################
# set includes
set (GUIDOEDITOR_INCLUDE_DIRS 
	${GUIDOQTDIR}/include 
	${SRCDIR}/src 
	${SRCDIR}/SetupDialog 
	${SRCDIR}/QFindWidget 
	${QTINCDIRS}
)

#######################################
# set libraries and definitions
if (WIN32)
	set (GUIDOEDITOR_LIBRARIES ${GUIDOQTLIBDIR}/GuidoQt.lib)
else()
	set (GUIDOEDITOR_LIBRARIES -L${GUIDOQTLIBDIR} -lGuidoQt)
	if (APPLE)
		set (GUIDOEDITOR_LIBRARIES ${GUIDOEDITOR_LIBRARIES} "-framework CoreFoundation")
	endif()
endif()
set (GUIDOEDITOR_LIBRARIES ${GUIDOEDITOR_LIBRARIES} ${QTLIBS})

#######################################
# ui and resources
file (GLOB FORMS  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  ${SRCDIR}/*/*.ui) 
set (GUIDOEDITOR_SRC ${GUIDOEDITOR_SRC} ${FORMS} ${RSRCDIR}/application.qrc)
set (GUIDOEDITOR_INFO_PLIST ${RSRCDIR}/GuidoEditorInfo.plist)
set (GUIDOEDITOR_ICON_FILE guido.icns)
set (RSRC "${RSRCDIR}/guido.icns" "${RSRCDIR}/English.lproj/InfoPlist.strings")
set_source_files_properties(${RSRC} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set (FONT ${GUIDOROOT}/src/guido2.svg ${GUIDOROOT}/src/guido2.ttf)
set_source_files_properties(${FONT} PROPERTIES MACOSX_PACKAGE_LOCATION "Fonts")
set (DOC ${GUIDOROOT}/doc/refcard/latex/RefCardsParams.pdf ${GUIDOROOT}/doc/refcard/latex/RefCardsTags.pdf)
set_source_files_properties(${DOC} PROPERTIES MACOSX_PACKAGE_LOCATION "Doc")

set (GUIDOEDITOR_SRC ${GUIDOEDITOR_SRC} ${RSRC} ${FONT} ${DOC})

if (WIN32)
	enable_language(RC)
	set(GUIDOEDITOR_SRC ${GUIDOEDITOR_SRC} ${RSRCDIR}/GuidoEditor.rc)
	set(GUIDOEDITOR_EXE WIN32)
elseif(APPLE)
	set(GUIDOEDITOR_EXE MACOSX_BUNDLE)
endif()

message (STATUS "Include GuidoEditor using Qt version ${Qt5Widgets_VERSION}")
