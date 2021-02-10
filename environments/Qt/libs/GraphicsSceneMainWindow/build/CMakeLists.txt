cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GraphicsSceneMainWindow)

option ( IOS    "Generate an iOS library"	off )


#######################################
get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set (SRCDIR  	${ROOT}/src)
set (GUIDODIR  	${ROOT}/../../../..)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Xml Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Xml_INCLUDE_DIRS} ${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
message (STATUS "Qt version ${Qt5Widgets_VERSION}")


#######################################
if(IOS)
	message (STATUS "Generate GraphicsSceneMainWindow library for iOS")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iphoneos")
	set (CMAKE_OSX_SYSROOT "iphoneos")
endif()

#######################################
# set sources and headers files
file (GLOB SRC  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	${SRCDIR}/GUI/*.cpp
	${SRCDIR}/GraphicsItem/*.cpp
	${SRCDIR}/LanguageCommandPalette/*.cpp
	${SRCDIR}/SyntaxHighlighter/*.cpp
)
file (GLOB HEADERS  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	${ROOT}/include/*.h ${SRCDIR}/LanguageCommandPalette/*.h
)

#######################################
# set includes
set (INCDIRS 
	${ROOT}/include 
	${SRCDIR}/LanguageCommandPalette
)
set (INCDIRS ${INCDIRS} ${QTINCDIRS})

#######################################
# Output folders 
set (BINDIR ${ROOT}/..)
set (LIBDIR ${ROOT}/..)

if (IOS)
	set ( TARGET GraphicsSceneMainWindow-iOS )
else()
	set ( TARGET GraphicsSceneMainWindow )
endif()

if (UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS -fPIC)
endif()

qt5_wrap_ui(UI ${SRCDIR}/GUI/ResolutionDialog.ui)
qt5_add_resources (RSRC ${ROOT}/rsc/application.qrc)

add_library(${TARGET} STATIC ${SRC} ${HEADERS} ${UI} ${RSRC})
target_include_directories (${TARGET} PRIVATE ${INCDIRS})
set_target_properties (${TARGET} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
)
