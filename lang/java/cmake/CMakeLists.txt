#######################################
# CMAKE guidolib
#######################################
project(guidojni)
cmake_minimum_required(VERSION 2.6)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set (target jniGUIDOEngine64)
else()
  set (target jniGUIDOEngine)
endif()

#######################################
# versions management
set (VERSION 1.21)
set (STRVERS "v.1.21")
set (SSTRVERS "1.21")

FIND_PACKAGE(JNI REQUIRED)
#FIND_PACKAGE(Java REQUIRED)
#message ( "JNI_INCLUDE_DIRS		 ${JNI_Iset_target_properties (${target} PROPERTIES
#message ( "JAVA_AWT_LIBRARY		 ${JAVA_AWT_LIBRARY}")
#message ( "JAVA_JVM_LIBRARY		 ${JAVA_JVM_LIBRARY}")
#message ( "JAVA_INCLUDE_PATH	 ${JAVA_INCLUDE_PATH}")
#message ( "JAVA_INCLUDE_PATH2	 ${JAVA_INCLUDE_PATH2}")
#message ( "JAVA_AWT_INCLUDE_PATH ${JAVA_AWT_INCLUDE_PATH}")


if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64")
	set (CMAKE_OSX_DEPLOYMENT_TARGET 10.6)
endif(APPLE)

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
  add_definitions(-DVC2005)
 endif()
  set (LIBDEST /usr/lib64)

 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  set (WIN64 1)
  set (target jniGUIDOEngine64)
 endif ()

endif(WIN32)

#######################################
# set directories, src and headers.
set (JNI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../jni)
set (ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set (GUIDO_SRC ${ROOT}/src/engine)

set (SRC "${JNI_PATH}/*.cpp")				# add source files
set (HEADERS "${JNI_PATH}/*.h")				# add header files
set (INCL ${JNI_INCLUDE_DIRS} ${GUIDO_SRC}/include ${GUIDO_SRC}/devices ${GUIDO_SRC}/tools)


if (INDEPENDENTSVG STREQUAL "yes")
	message (STATUS "Independent SVG will be enforced and all SVG fonts must be elaborated in SVG files - Use -DINDEPENDENTSVG=no to change.")
	add_definitions(-DINDEPENDENTSVG)
	set (SRC ${SRC} "${JNI_PATH}/svg/*.cpp")
	set (INCL ${INCL} "${JNI_PATH}/svg/*.h")
elseif(APPLE)
	set (INCL ${INCL} ${GUIDO_SRC}/include ${ROOT}/platforms/macosx/src)
	set (SRC ${SRC} "${JNI_PATH}/macos/*.cpp")
elseif(WIN32)
	set (INCL ${INCL} ${GUIDO_SRC}/include ${ROOT}/platforms/win32/src)
	set (SRC ${SRC} "${JNI_PATH}/win32/*.cpp")
elseif(UNIX)
	set (SRC ${SRC} )
	set (INCL ${INCL} ${GUIDO_SRC}/include ${ROOT}/platforms/linux/src)
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} UINCL)
	add_definitions(${UINCL})
	set (SRC ${SRC} "${JNI_PATH}/linux/*.cpp")
else(UNIX)
	message (FATAL_ERROR "Unknown target platform")
endif()

if (INDEPENDENTSVG STREQUAL "yes")
else()
	message (STATUS "Independent SVG will not be enforced and SVG fonts will be approximated using system fonts - Use -DINDEPENDENTSVG=yes to change.")
endif()

#######################################
# set includes
include_directories( ${INCL})

#######################################
# set library target
file (GLOB JAVA RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${JAVA_PATH}/*.java")
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})
set (GUIDO_PATH ${ROOT}/build)
if (WIN64)
 find_library (GUIDO GUIDOEngine64 PATHS ${GUIDO_PATH}/win64/Release NO_DEFAULT_PATH)
elseif(APPLE)
 find_library (GUIDO GUIDOEngine PATHS ${GUIDO_PATH}/MacOS/Release NO_DEFAULT_PATH)
elseif(UNIX)
 find_library (GUIDO GUIDOEngine PATHS ${GUIDO_PATH}/linux/Release NO_DEFAULT_PATH)
else()
 find_library (GUIDO GUIDOEngine PATHS ${GUIDO_PATH}/win32/Release NO_DEFAULT_PATH)
endif()

set (LIBCONTENT ${LIBSRC} ${LIBH})
#set (LINK ${JNI_LIBRARIES} ${GUIDO})
set (LINK  ${GUIDO})

if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBCONTENT} ../jni/win32/jniGUIDOEngine.rc)
	set(LINK ${LINK} Gdiplus)
elseif(APPLE)
	set(LINK ${LINK}  "-framework Cocoa  -framework CoreFoundation")
endif(WIN32)

add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES VERSION ${VERSION})
target_link_libraries (${target} ${LINK})

#######################################
# install settings
if (NOT WIN32 AND NOT IOS)
set (LIBDEST /usr/lib)
install ( TARGETS ${target}	LIBRARY DESTINATION ${LIBDEST})
endif()

