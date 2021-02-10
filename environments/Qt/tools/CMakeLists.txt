cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQtTools)

#######################################
set (ROOT 		${CMAKE_CURRENT_SOURCE_DIR})
set (GUIDODIR  	${CMAKE_CURRENT_SOURCE_DIR}/../../..)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} guido2image GuidoEditor GuidoSceneComposer)

#######################################
# Qt settings
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
set (QTLIBS 	Qt5::Widgets Qt5::PrintSupport)
message (STATUS "Using Qt version ${Qt5Widgets_VERSION}")

if (${Qt5Widgets_VERSION} VERSION_GREATER_EQUAL "5.11")
	set ( TOOLS_DEFINITIONS -DQTFONTMETRICS_511)
endif()

#######################################
# shared guidoengine settings
set (GUIDO_INCLUDE_DIRS ${GUIDODIR}/src/engine/include)
if (WIN32)
	if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine64.lib)
	else()
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine.lib)
	endif()
elseif (APPLE)
	set (GUIDO_LIBRARY "-F${GUIDODIR}/build/lib -framework GUIDOEngine")
else()
	set (GUIDO_LIBRARY "-L${GUIDODIR}/build/lib -lGUIDOEngine")
endif()

#message ("GUIDO_LIBRARY ${GUIDO_LIBRARY}")


set (BINDIR ${ROOT}/bin)

#######################################
# add fontmetrics
message (STATUS "Include FontMetrics tools")
set (fontmetrics fontmetrics)
add_executable(${fontmetrics} FontMetrics/main.cpp)
target_include_directories (${fontmetrics} PRIVATE ${Qt5Widget_INCLUDE_DIRS})
target_link_libraries (${fontmetrics} PRIVATE Qt5::Widgets)
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${fontmetrics} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${fontmetrics} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoFontMetrics
message (STATUS "Include GuidoFontMetrics tools")
set (gfmetrics guidofontmetrics)
add_executable(${gfmetrics} GuidoFontMetrics/main.cpp)
target_include_directories (${gfmetrics} PRIVATE ${Qt5Widget_INCLUDE_DIRS})
target_link_libraries (${gfmetrics} PRIVATE Qt5::Widgets)
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${gfmetrics} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${gfmetrics} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoQtFontViewer
message (STATUS "Include GuidoQtFontViewer tools")
set (fontviewer GuidoQtFontViewer)
if (WIN32)
	set(EXE WIN32)
elseif(APPLE)
	set(EXE MACOSX_BUNDLE)
endif()
set (VSRC 	GuidoQtFontViewer/main.cpp GuidoQtFontViewer/QFontDisplayer.cpp GuidoQtFontViewer/QFontDisplayer.h )
add_executable(${fontviewer} ${EXE} ${VSRC} )
target_include_directories (${fontviewer} PRIVATE ${Qt5Widget_INCLUDE_DIRS})
target_link_libraries (${fontviewer} PRIVATE Qt5::Widgets)
if (DEFINED TOOLS_DEFINITIONS)
	target_compile_definitions (${fontviewer} PRIVATE ${TOOLS_DEFINITIONS})
endif()
set_target_properties (${fontviewer} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)
