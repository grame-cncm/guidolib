cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQtTools)
option ( QT6    "Use Qt6"	on )
set (CMAKE_CXX_STANDARD 17)

#######################################
set (ROOT 		${CMAKE_CURRENT_SOURCE_DIR})
set (GUIDODIR  	${CMAKE_CURRENT_SOURCE_DIR}/../../..)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} guido2image GuidoEditor GuidoSceneComposer)

#######################################
# Qt settings
if (QT6)
	find_package(Qt6 COMPONENTS Widgets REQUIRED)
	set (QTINCDIRS 	${Qt6Widget_INCLUDE_DIRS} ${Qt6Core_INCLUDE_DIRS})
	set (QTLIBS 	Qt6::Widgets)
	set ( TOOLS_DEFINITIONS -DQTFONTMETRICS_511)
	message (STATUS "Using Qt version ${Qt6Widgets_VERSION}")
else ()
	find_package(Qt5 COMPONENTS Widgets REQUIRED)
	set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS})
	set (QTLIBS 	Qt5::Widgets)
	if (${Qt5Widgets_VERSION} VERSION_GREATER_EQUAL "5.11")
		set ( TOOLS_DEFINITIONS -DQTFONTMETRICS_511)
	endif()
	message (STATUS "Using Qt version ${Qt5Widgets_VERSION}")
endif()

#######################################
# shared guidoengine settings
set (GUIDO_INCLUDE_DIRS ${GUIDODIR}/src/engine/include)
if (WIN32)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine64.lib)
	else()
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine.lib)
	endif()
elseif (APPLE)
	set (GUIDO_LIBRARY "-F${GUIDODIR}/build/lib -framework GUIDOEngine")
else()
	set (GUIDO_LIBRARY "-L${GUIDODIR}/build/lib -lGUIDOEngine")
endif()


set (BINDIR ${ROOT}/bin)

#######################################
# add fontmetrics
message (STATUS "Include FontMetrics tool")
set (fontmetrics fontmetrics)
add_executable(${fontmetrics} FontMetrics/main.cpp)
target_include_directories (${fontmetrics} PRIVATE ${Qt5Widget_INCLUDE_DIRS})
target_link_libraries (${fontmetrics} PRIVATE ${QTLIBS})
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${fontmetrics} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${fontmetrics} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoFontMetrics
message (STATUS "Include GuidoFontMetrics")
set (gfmetrics guidofontmetrics)
add_executable(${gfmetrics} GuidoFontMetrics/main.cpp)
target_include_directories (${gfmetrics} PRIVATE ${Qt5Widget_INCLUDE_DIRS})
target_link_libraries (${gfmetrics} PRIVATE ${QTLIBS})
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${gfmetrics} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${gfmetrics} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoQtFontViewer
message (STATUS "Include GuidoQtFontViewer")
set (fontviewer GuidoQtFontViewer)
if (WIN32)
	set(EXE WIN32)
elseif(APPLE)
	set(EXE MACOSX_BUNDLE)
endif()
set (VSRC 	GuidoQtFontViewer/main.cpp GuidoQtFontViewer/QFontDisplayer.cpp GuidoQtFontViewer/QFontDisplayer.h )
add_executable(${fontviewer} ${EXE} ${VSRC} )
target_include_directories (${fontviewer} PRIVATE ${QTINCDIRS})
target_link_libraries (${fontviewer} PRIVATE ${QTLIBS})
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${fontviewer} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${fontviewer} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add BravuraFontViewer
message (STATUS "Include BravuraFontViewer")
set (bfontviewer BravuraFontViewer)
if (WIN32)
	set(EXE WIN32)
elseif(APPLE)
	set(EXE MACOSX_BUNDLE)
endif()
set (BSRC 	BravuraFontViewer/main.cpp BravuraFontViewer/QFontDisplayer.cpp BravuraFontViewer/QFontDisplayer.h )
add_executable(${bfontviewer} ${EXE} ${BSRC} )
target_include_directories (${bfontviewer} PRIVATE ${QTINCDIRS})
target_link_libraries (${bfontviewer} PRIVATE ${QTLIBS})
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${bfontviewer} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${bfontviewer} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)
