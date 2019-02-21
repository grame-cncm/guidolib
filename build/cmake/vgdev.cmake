#######################################
# VG device settings
#######################################

#######################################
# Open GL settings
if (GL)
	message (STATUS "Include Open GL device. Use -DGL=no to change.")
	# independent svg needs to be enabled with this device
	set (DEFS "${DEFS} -DINDEPENDENTSVG" )
	set (VGDEV_SRC "${VGDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
	set(LINK ${LINK} "-l glut -l GLU")

	# gl
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "gl" OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} GLINCL)
	set (DEFS "${DEFS} ${GLINCL}" )
    EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "gl"   OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} GLLIB)
	set(LINK ${LINK} ${GLLIB})

	# freetype
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "freetype2" OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} FREETYPEINCL)
	set (DEFS "${DEFS} ${FREETYPEINCL}" )
    EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "freetype2"   OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} FREETYPELIB)
	set(LINK ${LINK} ${FREETYPELIB})

	# ftgl
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "ftgl"   OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} FTGLINCL)
	set (DEFS "${DEFS} ${FTGLINCL}" )
    EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "ftgl"     OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} FTGLLIB)
	set(LINK ${LINK} ${FTGLLIB})
else()
	message (STATUS "Open GL device won't be built. Use -DGL=yes to change.")
endif()

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
