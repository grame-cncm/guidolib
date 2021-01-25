cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQt)

option ( IOS    "Generate an iOS library"	off )

#######################################
get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set (SRCDIR  	${ROOT})
set (GUIDODIR  	${ROOT}/../../../..)

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})

#######################################
if(IOS)
	message (STATUS "Generate GuidoQt library for iOS")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iphoneos")
	set (CMAKE_OSX_SYSROOT "iphoneos")
	set (DEFS -DIOS)
endif()

set ( DEFS ${DEFS} )

#######################################
# set sources and headers files
file (GLOB SRC           RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	"${SRCDIR}/*.cpp" 
)
file (GLOB HEADERS       RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	"${SRCDIR}/include/*.h" 
)
file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRCDIR}/include/*.h")

#######################################
# set includes
set (INCL ${INCL} 
	${SRCDIR}/include
	${GUIDODIR}/src/engine/include
)
file (GLOB INCDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INCL})
set (INCDIRS ${INCDIRS} ${QTINCDIRS})


#######################################
# Output folders 
set (BINDIR ${ROOT}/..)
set (LIBDIR ${ROOT}/..)

if (IOS)
	set ( TARGET GuidoQt-iOS )
else()
	set ( TARGET GuidoQt )
endif()

add_library(${TARGET} STATIC ${SRC} ${HEADERS} ${PUBLICHEADERS})
target_compile_definitions (${TARGET} PRIVATE ${DEFS})
target_include_directories (${TARGET} PRIVATE ${INCDIRS})
set_target_properties (${TARGET} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
	PUBLIC_HEADER "${PUBLICHEADERS}"
)
