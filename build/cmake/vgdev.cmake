#######################################
# Windows settings
if (WIN32)
	message (STATUS "Include GDI and GDIPlus devices.")
	set (VGDEV_SRC "${VGDEV_PATH}/GDeviceWin32.cpp" "${VGDEV_PATH}/GFontWin32.cpp" "${VGDEV_PATH}/GSystemWin32.cpp"
		"${VGDEV_PATH}/GDeviceWin32GDIPlus.cpp" "${VGDEV_PATH}/GFontWin32GDIPlus.cpp" "${VGDEV_PATH}/GSystemWin32GDIPlus.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
endif()

#######################################
# MacOS settings
if (APPLE)
	message (STATUS "Include OSX device.")
	set (VGDEV_SRC "${VGDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})

#######################################
# Unix settings
elseif (UNIX)
	message (STATUS "Include Cairo device.")
	set (VGDEV_SRC "${VGDEV_PATH}/*Cairo*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"  OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} UINCL)
	set (DEFS "${DEFS} ${UINCL}" )
endif()


#######################################
# Open GL settings
#######################################
# manual pkg config path (windows)
macro (set_pkg_config_path)
	set (PP $ENV{PKG_CONFIG_PATH})
	foreach(A ${PP})
		string (REPLACE "\\" "/" A1 ${A} )
		string (REGEX REPLACE "C:/msys[^/]*" "" A2 ${A1} )
		set (LP "${LP}:${A2}")
	endforeach()
	set (ENV{PKG_CONFIG_PATH} ${LP} )
endmacro()


if (GL)
	set (ENVIRONMENTS ${ROOT}/environments)
	set(GLDEV_PATH ${ENVIRONMENTS}/opengl/device)

	message (STATUS "Include Open GL device. Use -DGL=no to change.")
	# independent svg needs to be enabled with this device
	set (GLDEFS "-DINDEPENDENTSVG" )
	set (GL_SRC "${GLDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h GL_H ${GL_SRC})


	find_package(PkgConfig REQUIRED)
	if (PKG_CONFIG_FOUND) 
		if ( WIN32 OR MSYS )
			set_pkg_config_path ()
			message(STATUS "Using MSYS PKG_CONFIG_PATH $ENV{PKG_CONFIG_PATH}")
		endif()
		
		pkg_search_module(GL "gl" REQUIRED)
		if (GL_FOUND)
			set(GLDEFS "${GL_CFLAGS}")
			set(GLLINK "${GL_LDFLAGS}")
			message (STATUS "Found gl ${GL_VERSION}")
		endif()

		pkg_search_module(FT "freetype2" REQUIRED)
		if (FT_FOUND)
			set(GLDEFS "${GLDEFS} ${FT_CFLAGS}")
			set(GLLINK "${GLLINK} ${FT_LDFLAGS}")
			message (STATUS "Found freetype2 ${FT_VERSION}")
		endif()

		pkg_search_module(FTGL "ftgl" REQUIRED)
		if (FTGL_FOUND)
			set(GLDEFS "${GLDEFS} ${FTGL_CFLAGS}")
			set(GLLINK "${GLLINK} ${FTGL_LDFLAGS}")
			message (STATUS "Found ftgl ${FTGL_VERSION}")
		endif()

		set (VGDEV_SRC ${VGDEV_SRC} ${GL_SRC})
		set (VGDEV_H   ${VGDEV_H}   ${GL_H})
		set (VGDEV_PATH ${VGDEV_PATH} ${ENVIRONMENTS}/opengl/device)
		set (DEFS "${DEFS} ${SGLDEFS}")
		set (LINK "${LINK} ${SGLLINK}")

	else()
		message (WARNING "No pkg-config, cannot locate OpenGl required packages: skip OpenGl")
		return ()
	endif()
endif()

