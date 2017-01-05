#######################################
# CMAKE guidolib
#######################################

project(guidolib)
cmake_minimum_required(VERSION 2.6)

set (CMAKE_CONFIGURATION_TYPES Debug Release)
set (target GUIDOEngine)
set (statictarget SGUIDOEngine)
#######################################
# versions management
set (VERSION 1.64)
set (STRVERS "v.1.64")
set (GUIDOFWKVERS C)
set (SOVERS 1)

if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

#######################################
# Options disabled by default
option ( C++11 	"C++11 support" on )
option ( IOS 	"Generates project for iOS" off )
option ( DIST 	"Compiles universal binaries (x86_64 i386) on MacOSX" off )
option ( MIDIEXPORT "MIDI export support (requires MidiShareLight)" off)
option ( INDEPENDENTSVG "Independent SVG support (gets all font information from provided SVG files)" off)
option ( STATICLIB  "Generates a static library (default for iOS)" off)
option ( GL "Links to opengl device" off)


if(APPLE)
#######################################
#   iOS support
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "arm64 armv7 armv7s")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (libtype STATIC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	else ()
		message (STATUS "Generates Mac OS project- Use -DIOS=yes to change.")
	 	#######################################
		if ( C++11 )
			message (STATUS "Generates project using C++11 - Use -DC++11=no to change.")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		else()
			message (STATUS "Generates project without C++11 - Use -DC++11=yes to change.")
		endif()
		if (DIST)
			message (STATUS "All architectures will be compiled - Use -DDIST=no to change.")
			set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
		else ()
			message (STATUS "Only the current architecture will be compiled - Use -DDIST=yes to change.")
		endif ()
	add_definitions(-D_USE_QD_COORDINATES_ )
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.7)
#	set (CMAKE_MACOSX_RPATH 1)
	set (CMAKE_LDFLAGS -mmacosx-version-min=10.7)
	set (CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
	set (MACOSX_DEPLOYMENT_TARGET 10.7)
	endif ()
endif(APPLE)

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10 2010")
  add_definitions(-DVC2005)
elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013")
  add_definitions(-DVC2005)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
 elseif(${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles")
  add_definitions(-DGCC)
 endif()

 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  add_definitions(-DVC2005)
  add_definitions(-DWIN64)
  set (WIN64 1)
  set (target GUIDOEngine64)
  set (statictarget SGUIDOEngine64)
 endif ()

endif(WIN32)
add_definitions(-DGUIDOExport)

#######################################
# set directories, src and headers.
set (GUIDO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (PLATFORMS ${GUIDO_PATH}/platforms)
set (SRC_PATH	${GUIDO_PATH}/src/engine)
set (HEADERS_PATH ${SRC_PATH}/include)
if (GL STREQUAL "yes")
	set(VGDEV_PATH ${PLATFORMS}/opengl/device)
elseif(WIN32)
	set(VGDEV_PATH ${PLATFORMS}/win32/src)
elseif(APPLE)
	set(VGDEV_PATH ${PLATFORMS}/macosx/src)
elseif(UNIX)
	set(VGDEV_PATH ${PLATFORMS}/linux/src)
endif()

#######################################
# midi export support
if (MIDIEXPORT STREQUAL "yes")
	message (STATUS "MIDI export will be generated (requires MidiShareLight) - Use -DMIDIEXPORT=no to change.")
	set (MSLIGHT_PATH	${GUIDO_PATH}/src/midisharelight)
	add_definitions(-DMIDIEXPORT)
	
	set(INCL ${INCL} "${MSLIGHT_PATH}")			# add include folders
	if (WIN32)
		if (${CMAKE_CL_64})
			set(LINK ${LINK} "${MSLIGHT_PATH}/win64/midisharelight64.lib")
		else()
			set(LINK ${LINK} "${MSLIGHT_PATH}/win32/midisharelight.lib")
		endif()
	elseif(APPLE)
		set(LINK ${LINK} "-L${MSLIGHT_PATH}/macos -lmidisharelight")
	elseif(UNIX)
		set(LINK ${LINK} "-L/usr/local/lib -lmidisharelight")
	endif (WIN32)
else()
	message (STATUS "MIDI export will not be generated - Use -DMIDIEXPORT=yes to change (requires MidiShareLight).")
endif()

# OPENGL
if (GL STREQUAL "yes")
	message (STATUS "Open GL device will be built. Use -DGL=no to change.")
else()
	message (STATUS "Open GL device won't be built. Use -DGL=yes to change.")
endif()

set (SRC ${SRC} "${SRC_PATH}/*/*.cpp")					# add source files
# independent svg support
if (INDEPENDENTSVG STREQUAL "yes")
	message (STATUS "Independent SVG will be enforced and all SVG fonts must be elaborated in SVG files - Use -DINDEPENDENTSVG=no to change.")
	add_definitions(-DINDEPENDENTSVG)
else()
	message (STATUS "Independent SVG will not be enforced and SVG fonts will be approximated using system fonts - Use -DINDEPENDENTSVG=yes to change.")
endif()

# vgdev
if (GL STREQUAL "yes")
	# independent svg needs to be enabled with this device
	add_definitions(-DINDEPENDENTSVG)
	set (VGDEV_SRC "${VGDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
	set(LINK ${LINK} "-l glut -l GLU")
	# gl
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "gl"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} GLINCL)
	add_definitions(${GLINCL})
        EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "gl"
                  OUTPUT_VARIABLE TMP
                  ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} GLLIB)
	set(LINK ${LINK} ${GLLIB})
	# freetype
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "freetype2"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} FREETYPEINCL)
	add_definitions(${FREETYPEINCL})
        EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "freetype2"
                  OUTPUT_VARIABLE TMP
                  ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} FREETYPELIB)
	set(LINK ${LINK} ${FREETYPELIB})
	# ftgl
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "ftgl"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} FTGLINCL)
	add_definitions(${FTGLINCL})
        EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "ftgl"
                  OUTPUT_VARIABLE TMP
                  ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} FTGLLIB)
	set(LINK ${LINK} ${FTGLLIB})
elseif(WIN32)
	set (VGDEV_SRC "${VGDEV_PATH}/GDeviceWin32.cpp" "${VGDEV_PATH}/GFontWin32.cpp" "${VGDEV_PATH}/GSystemWin32.cpp"
		"${VGDEV_PATH}/GDeviceWin32GDIPlus.cpp" "${VGDEV_PATH}/GFontWin32GDIPlus.cpp" "${VGDEV_PATH}/GSystemWin32GDIPlus.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
elseif(APPLE)
	set (VGDEV_SRC "${VGDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
elseif(UNIX)
	set (VGDEV_SRC "${VGDEV_PATH}/*Cairo*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} UINCL)
	add_definitions(${UINCL})
endif()

file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC} ${VGDEV_SRC})

set(HEADERS ${HEADERS} "${SRC_PATH}/*/*.h")		# add header files
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS} ${VGDEV_H})
file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${HEADERS_PATH}/*.h" "${VGDEV_H}" "${SRC_PATH}/devices/*H")

set(INCL ${INCL} "${SRC_PATH}/*")			# add include folders
set(INCL ${INCL} "${VGDEV_PATH}")			# add include folders
file (GLOB INCDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INCL})

#######################################
# set includes
include_directories( ${INCDIRS})
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

#######################################
# set library target
if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBSRC} ${LIBH} ${PLATFORMS}/win32/GUIDOEngine.rc)
	set(LINK ${LINK} "msimg32.lib" "gdiplus.lib")
else(WIN32)
	set(LIBCONTENT ${LIBSRC} ${LIBH})
endif(WIN32)

if(APPLE)
	set(LINK ${LINK} "-framework Carbon -compatibility_version 1.0.0 -lc++")
elseif (UNIX)
      EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} CAIRO)
	set(LINK ${LINK} ${CAIRO})
endif(APPLE)

#######################################
# library generation
if (IOS STREQUAL "yes")
	add_library(${target} STATIC ${LIBCONTENT})

else()

	add_library(${target} SHARED ${LIBCONTENT})
	set_target_properties (${target} PROPERTIES
				FRAMEWORK TRUE
				FRAMEWORK_VERSION ${GUIDOFWKVERS}
				PUBLIC_HEADER "${PUBLICHEADERS}"
				DEFINE_SYMBOL LIBMUSICXML_EXPORTS
				VERSION ${VERSION}
#				MACOSX_RPATH TRUE
				SOVERSION ${SOVERS}
				MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
				MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION}
				MACOSX_FRAMEWORK_BUNDLE_VERSION "A")

	if (STATICLIB)
		message (STATUS "Static Guido lib will be generated - Use -DSTATICLIB=no to change.")
		add_library(${statictarget} STATIC ${LIBCONTENT})
	else ()
		message (STATUS "Static Guido lib will not be generated - Use -DSTATICLIB=yes to change.")
	endif ()

	set (LIBDEST /usr/lib)
	target_link_libraries (${target} ${LINK})
endif()

if(WIN32)
	set_target_properties (${target} PROPERTIES
		DEBUG_OUTPUT_NAME ${target}D
		RELEASE_OUTPUT_NAME ${target})
endif()

#######################################
# install setup
if(APPLE)
set(FRMWK  FRAMEWORK DESTINATION "/Library/Frameworks" CONFIGURATIONS Release)
endif(APPLE)

if (NOT WIN32 AND NOT APPLE)
 if (DEFINED LIBINSTALL)
	set(LIBPATH ${LIBINSTALL})
 else (DEFINED LIBINSTALL)
	set(LIBPATH "/usr/lib")
 endif (DEFINED LIBINSTALL)
 message (STATUS "Library install path set to ${LIBPATH} - Use -DLIBINSTALL=path to change.")

install ( TARGETS ${target}
    	LIBRARY DESTINATION ${LIBPATH}
    	${FRMWK}
    	PUBLIC_HEADER DESTINATION /usr/include/guido
)
endif()


