#######################################
# CMAKE guidotools
#######################################
project(guidotools)
cmake_minimum_required(VERSION 2.6)

set (target GUIDOEngine)
#######################################
# set directories, src and headers.
set (TOOLS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/..)
set (TESTS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/../../validate)
set (GUIDO	${CMAKE_CURRENT_SOURCE_DIR}/../../src)

#######################################
if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

if(APPLE)
	add_definitions(-D_USE_QD_COORDINATES_)
	set (CMAKE_OSX_ARCHITECTURES "x86_64")
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.6)
	set (CMAKE_LDFLAGS -mmacosx-version-min=10.6)
	set (DEVICE ${GUIDO}/../macosx/src)
elseif(WIN32)
	set (DEVICE ${GUIDO}/../win32/src)
elseif(UNIX)
	set (DEVICE ${GUIDO}/../linux/src)
endif()

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
endif()

include_directories( ${GUIDO}/include ${GUIDO}/devices ${GUIDO}/tools ${DEVICE} )

if(APPLE OR WIN32)
	if (${CMAKE_CL_64})
		set(GUIDOEngine GUIDOEngine64)
	else ()
		set(GUIDOEngine GUIDOEngine)
	endif()
 	find_library (GUIDOLIB ${GUIDOEngine} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../build/*/Release NO_DEFAULT_PATH)
else()
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} UINCL)
	add_definitions(${UINCL})
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} CAIRO)
	set(LINK ${LINK} ${CAIRO})

	find_library (GUIDOLIB GUIDOEngine PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../build/unix NO_DEFAULT_PATH)
endif()

#######################################
# tools generation
set(TOOLS guido2midi guido2svg guido2abstract guido2svgmap guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetstaffmap guidogetsystemmap guidogetvoicemap)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)

set(TOOLS guidoparse guidoar2gr guidodraw)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TESTS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)

