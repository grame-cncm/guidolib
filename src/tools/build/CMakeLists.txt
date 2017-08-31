#######################################
# CMAKE guidotools
#######################################
project(guidotools)
cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6)
#cmake_policy(SET CMP0046 OLD)

set (target GUIDOEngine)
#######################################
# set directories, src and headers.
set (TOOLS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/..)
set (TESTS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/../../../validate)
set (GUIDO		${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set (GUIDOSRC	${GUIDO}/src)

#######################################
if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

if(APPLE)
	add_definitions(-D_USE_QD_COORDINATES_)
	set (CMAKE_OSX_ARCHITECTURES "x86_64")
#	set (CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
	set (DEVICE ${GUIDO}/platforms/macosx/src)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -stdlib=libc++)
elseif(WIN32)
	set (DEVICE ${GUIDO}/platforms/win32/src)
elseif(UNIX)
	set (DEVICE ${GUIDO}/platforms/linux/src)
endif()

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
endif()

include_directories( ${GUIDOSRC}/engine/include ${GUIDOSRC}/engine/devices ${GUIDOSRC}/engine/tools ${DEVICE} )

if(APPLE OR WIN32)
	if (${CMAKE_CL_64})
		set(GUIDOEngine GUIDOEngine64)
	else ()
		set(GUIDOEngine GUIDOEngine)
	endif()
 	find_library (GUIDOLIB ${GUIDOEngine} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/*/Release NO_DEFAULT_PATH)
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

	find_library (GUIDOLIB GUIDOEngine PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/linux NO_DEFAULT_PATH)
endif()

#######################################
# tools generation
set(TOOLS guidoparse guidoar2gr guidodraw)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TESTS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)

set(TOOLS guido2midi guido2svg guido2abstract guido2proll midi2proll guido2svgmap guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetstaffmap guidogetsystemmap guidogetvoicemap)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)


#######################################
# install setup
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX /usr )
endif()
if (NOT WIN32 AND NOT APPLE)
message (STATUS "Library install prefix set to ${CMAKE_INSTALL_PREFIX} - Use -DCMAKE_INSTALL_PREFIX=path to change.")

install ( TARGETS ${TOOLS}
    	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
endif()



