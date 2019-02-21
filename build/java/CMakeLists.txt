#######################################
# CMAKE guidolib jni
#######################################
project(guidojni)
cmake_minimum_required(VERSION 2.6)

set (CMAKE_BUILD_TYPE Release)
set (java jniGUIDOEngine)

#######################################
# check for variables that are normally set at upper level
if (NOT DEFINED target)
	set (target GUIDOEngine)
endif()
if (NOT DEFINED ROOT)
	set (ROOT ../..)
endif()

if(NOT APPLE)
	if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	  set (java jniGUIDOEngine64)
	endif()
endif()


FIND_PACKAGE(JNI )
if (NOT ${JNI_FOUND})
	message (WARNING "JNI package not found - skip jniGUIDOEngine target")
	return () 
endif()

#FIND_PACKAGE(Java REQUIRED)
#message ( "JNI_INCLUDE_DIRS	  ${JNI_INCLUDE_DIRS}")
#message ( "JAVA_AWT_LIBRARY	  ${JAVA_AWT_LIBRARY}")
#message ( "JAVA_JVM_LIBRARY	  ${JAVA_JVM_LIBRARY}")
#message ( "JAVA_INCLUDE_PATH	  ${JAVA_INCLUDE_PATH}")
#message ( "JAVA_INCLUDE_PATH2	  ${JAVA_INCLUDE_PATH2}")
#message ( "JAVA_AWT_INCLUDE_PATH ${JAVA_AWT_INCLUDE_PATH}")


if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  set (java jniGUIDOEngine64)
 endif ()
endif(WIN32)

#######################################
# set directories, src and headers.
set (JNI_PATH ${ROOT}/lang/java/jni)
set (GUIDO_SRC ${ROOT}/src/engine)

set (SRC "${JNI_PATH}/*.cpp")				# add source files
set (HEADERS "${JNI_PATH}/*.h")				# add header files
set (INCJAVA ${JNI_INCLUDE_DIRS} ${GUIDO_SRC}/include ${GUIDO_SRC}/devices ${GUIDO_SRC}/tools)

if(APPLE)
	set (INCJAVA ${INCJAVA} ${ROOT}/platforms/macosx/src)
	set (SRC ${SRC} "${JNI_PATH}/macos/*.cpp")
elseif(WIN32)
	set (INCJAVA ${INCJAVA} ${ROOT}/platforms/win32/src)
	set (SRC ${SRC} "${JNI_PATH}/win32/*.cpp")
elseif(UNIX)
	set (INCJAVA ${INCJAVA} ${ROOT}/platforms/linux/src)
	set (SRC ${SRC} "${JNI_PATH}/linux/*.cpp")
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo" OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} UINCL)
	set ( JAVADEFS ${JAVADEFS} ${UINCL})
endif()


#######################################
# set src header and libraries
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

set (LIBCONTENT ${LIBSRC} ${LIBH})
set (LINK ${LINK}  ${target})

if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBCONTENT} ${JNI_PATH}/win32/jniGUIDOEngine.rc)
	set(LINK ${LINK} Gdiplus)
elseif(APPLE)
	set(LINK ${LINK}  "-framework Cocoa  -framework CoreFoundation")
endif(WIN32)

#######################################
# add jni target
add_library(${java} SHARED ${LIBCONTENT})
set_target_properties (${java} PROPERTIES 
	VERSION ${VERSION}
	SOVERSION ${SOVERS}
)
target_compile_definitions (${target} PRIVATE ${JAVADEFS})
target_include_directories (${java} PRIVATE ${INCJAVA})
target_link_libraries (${java} ${LINK})

#######################################
# install settings
install ( TARGETS ${java}	
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib CONFIGURATIONS Release
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib CONFIGURATIONS Release
)

####################################
# install java classes
set (JAVA ${ROOT}/lang/java)
set (JAVADEST ${CMAKE_INSTALL_PREFIX}/share/guidolib/java)
install (
    DIRECTORY ${JAVA}/guidoengine DESTINATION ${JAVADEST}
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    FILES_MATCHING PATTERN "*.java"
)

####################################
# install java doc
install (
    DIRECTORY ${JAVA}/doc DESTINATION ${JAVADEST}
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN ".DS_Store" EXCLUDE
)

####################################
# install java samples
install (
    FILES     ${JAVA}/guidoviewer.java  ${JAVA}/factorysample.java  DESTINATION ${JAVADEST}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

####################################
# install jar file + misc
install (
    FILES  ${JAVA}/guidoengine.jar ${JAVA}/package/readme.txt ${JAVA}/changelog.txt DESTINATION ${JAVADEST}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

