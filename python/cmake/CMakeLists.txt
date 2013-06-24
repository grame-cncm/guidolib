project(PyGuido)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.7.0 REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

set(SHIBOKEN_GENERATOR_EXTRA_FLAGS "")
set(SHIBOKEN_INCLUDE_PATHS
    ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/src/include
    ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/Qt/GuidoQt/include
)
set(EXTRA_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/src/include
    ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/Qt/GuidoQt/include
)
set(EXTRA_LINK_LIBRARIES "")

set(CMAKE_BUILD_TYPE Release)

if(CMAKE_HOST_WIN32)
    # Windows
    # shiboken
    set(SHIBOKEN_BASE_DIR "C:/Python27/Lib/site-packages/PySide")
    
    set(SHIBOKEN_BINARY "${SHIBOKEN_BASE_DIR}/shiboken.exe")
    set(SHIBOKEN_INCLUDE_DIR "${SHIBOKEN_BASE_DIR}/include/shiboken")
    set(SHIBOKEN_LIBRARY "${SHIBOKEN_BASE_DIR}/shiboken-python2.7.lib")
    set(SHIBOKEN_PYTHON_LIBRARIES "")
    
    # needed to find pyside_global.h header and guidolib headers
    set(SHIBOKEN_INCLUDE_PATHS ${SHIBOKEN_INCLUDE_PATHS}
        ${SHIBOKEN_BASE_DIR}/include/PySide
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/win32/src
    )    
    set(SHIBOKEN_INCLUDE_PATH_SEPARATOR "\\;")

    # PySide
    set(PYSIDE_BASE_DIR "C:/Python27/Lib/site-packages/PySide")
    
    set(PYSIDE_INCLUDE_DIR "${PYSIDE_BASE_DIR}/include/PySide")
    set(PYSIDE_LIBRARY "${PYSIDE_BASE_DIR}/pyside-python2.7.lib")
    set(PYSIDE_TYPESYSTEMS "${PYSIDE_BASE_DIR}/typesystems")
    
    # PyGuido
    set(PYGUIDO_GLOBAL_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/global_win.h")
    set(PYGUIDO_TYPESYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/typesystem_pyguido_win.xml")
    
    set(PYGUIDO_BASE_SRC_PLATFORM
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdevicewin32_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdevicewin32gdiplus_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gfontwin32_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gfontwin32gdiplus_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gsystemwin32_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gsystemwin32gdiplus_wrapper.cpp
    )
    
    # make sure Windows can find python27.lib
    link_directories("C:/Python27/libs")
    
    # needs to be set on Windows (for linker to resolve symbols properly)
    set(SHIBOKEN_GENERATOR_EXTRA_FLAGS ${SHIBOKEN_GENERATOR_EXTRA_FLAGS} "--avoid-protected-hack")
    add_definitions(-DAVOID_PROTECTED_HACK)
    
    set(EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/win32/src
        ${QT_QTGUI_INCLUDE_DIR}
    )
    set(EXTRA_LINK_LIBRARIES ${EXTRA_LINK_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Release/GUIDOEngine64.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Qt/GuidoQt/release/GuidoQt.lib
        ${QT_QTGUI_LIBRARY}
    )
    
elseif(CMAKE_HOST_APPLE)
    # Mac
    find_package(Shiboken REQUIRED)
    find_package(PySide REQUIRED)
    
    set(SHIBOKEN_INCLUDE_PATHS ${SHIBOKEN_INCLUDE_PATHS}
        ${PYSIDE_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/macosx/src
    )
    set(SHIBOKEN_INCLUDE_PATH_SEPARATOR ":")
    
    # PyGuido
    set(PYGUIDO_GLOBAL_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/global_mac.h")
    set(PYGUIDO_TYPESYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/typesystem_pyguido_mac.xml")
    
    set(PYGUIDO_BASE_SRC_PLATFORM
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/fontencodingconverter_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdeviceosx_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdisplaydeviceosx_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gfontosx_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gmemorydeviceosx_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gprinterdeviceosx_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gsystemosx_wrapper.cpp
    )
    
    set(EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/macosx/src
        ${QT_QTGUI_INCLUDE_DIR}
    )
    set(EXTRA_LINK_LIBRARIES ${EXTRA_LINK_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Release/GUIDOEngine.framework
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Qt/GuidoQt/libGuidoQt.a
        ${QT_QTGUI_LIBRARY}
    )

elseif(CMAKE_HOST_UNIX)
    # assume Linux
    # shiboken
    find_package(Shiboken REQUIRED)
    find_package(PySide REQUIRED)
    
    # at the moment, use cairo backend (ignore gtk)
    set(CAIRO_INCLUDE_DIR "/usr/include/cairo")
    
    set(SHIBOKEN_INCLUDE_PATHS ${SHIBOKEN_INCLUDE_PATHS}
        ${PYSIDE_INCLUDE_DIR} #${SHIBOKEN_BASE_DIR}/include/PySide
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/linux/src
        
        ${CAIRO_INCLUDE_DIR}
        ${QT_QTGUI_INCLUDE_DIR}
    )
    set(SHIBOKEN_INCLUDE_PATH_SEPARATOR ":")
    
    # PyGuido
    set(PYGUIDO_GLOBAL_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/global_linux.h")
    set(PYGUIDO_TYPESYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido/typesystem_pyguido_linux.xml")
    
    set(PYGUIDO_BASE_SRC_PLATFORM
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/cairodevice_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/cairofont_wrapper.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/cairosystem_wrapper.cpp
#        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdevicegtk_wrapper.cpp
#        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gfontgtk_wrapper.cpp
#        ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gsystemgtk_wrapper.cpp
    )
    
    set(EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}/guidolib-src/linux/src
        ${CAIRO_INCLUDE_DIR}
        ${QT_QTGUI_INCLUDE_DIR}
    )
    set(EXTRA_LINK_LIBRARIES ${EXTRA_LINK_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/libGUIDOEngine.so
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Qt/GuidoQt/libGuidoQt.a
        ${QT_QTGUI_LIBRARY}
    )

else()
    # unknown host
endif()

include(${QT_USE_FILE})

set(PYGUIDO_BASE_SRC_COMMON
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/arhandler_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/carhandler_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/cgrhandler_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/floatrect_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gpaintstruct_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/grhandler_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guido2midiparams_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidodate_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidoelementinfos_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidofeedback_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidoinitdesc_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidolayoutsettings_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidoondrawdesc_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guidopageformat_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/intrect_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/mapcollector_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/mapelement_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/rectinfos_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/singletongsystem_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/time2graphicmap_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/timemapcollector_wrapper.cpp
#    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/timesegment_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/vgcolor_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/vgdevice_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/vgfont_wrapper.cpp
#    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/vgpen_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/vgsystem_wrapper.cpp
)

set(PYGUIDO_BASE_SRC
    ${PYGUIDO_BASE_SRC_COMMON}
    ${PYGUIDO_BASE_SRC_PLATFORM}
)

set(PYGUIDO_SRC
    # module
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/pyguido_module_wrapper.cpp

    # PyGuido
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gdeviceqt_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gfontqt_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/gsystemqt_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guido2image_params_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/guido2image_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidographicsitem_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidoimporter_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidopainter_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidowidget_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qpagemanager_wrapper.cpp
    
    # GuidoLib
    ${PYGUIDO_BASE_SRC}
)
 
set(PYGUIDO_INCLUDE_DIRECTORIES
    ${SHIBOKEN_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${PYSIDE_INCLUDE_DIR}
    ${PYSIDE_INCLUDE_DIR}/QtCore
    ${PYSIDE_INCLUDE_DIR}/QtGui
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${EXTRA_INCLUDE_DIRS}    
)

set(PYGUIDO_LINK_LIBRARIES
    ${QT_QTCORE_LIBRARY}
    ${SHIBOKEN_PYTHON_LIBRARIES}
    ${SHIBOKEN_LIBRARY}
    ${PYSIDE_LIBRARY}
    ${EXTRA_LINK_LIBRARIES}
)

include_directories(PyGuido ${PYGUIDO_INCLUDE_DIRECTORIES})
add_library(PyGuido MODULE ${PYGUIDO_SRC})
set_property(TARGET PyGuido PROPERTY PREFIX "")
target_link_libraries(PyGuido ${PYGUIDO_LINK_LIBRARIES})

if(CMAKE_HOST_WIN32)
    # make sure output dll gets .pyd extension
    set_property(TARGET PyGuido PROPERTY SUFFIX ".pyd")
endif()

string(REPLACE ";" "${SHIBOKEN_INCLUDE_PATH_SEPARATOR}" SHIBOKEN_INCLUDE_PATHS "${SHIBOKEN_INCLUDE_PATHS}")

# copy include header files and patch them
add_custom_command(
    OUTPUT NotAFile_copy_and_patch
    COMMAND ${CMAKE_COMMAND} -E make_directory guidolib-src/src/include
    COMMAND ${CMAKE_COMMAND} -E make_directory guidolib-src/win32/src
    COMMAND ${CMAKE_COMMAND} -E make_directory guidolib-src/macosx/src
    COMMAND ${CMAKE_COMMAND} -E make_directory guidolib-src/linux/src
    COMMAND ${CMAKE_COMMAND} -E make_directory guidolib-src/Qt/GuidoQt/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../src/include guidolib-src/src/include/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../win32/src guidolib-src/win32/src/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../macosx/src guidolib-src/macosx/src/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../linux/src guidolib-src/linux/src/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../Qt/GuidoQt/include guidolib-src/Qt/GuidoQt/include/

    COMMAND patch guidolib-src/Qt/GuidoQt/include/Guido2Image.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/Qt/GuidoQt/include/Guido2Image.h.patch
    COMMAND patch guidolib-src/src/include/GUIDO2Midi.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/src/include/GUIDO2Midi.h.patch
    COMMAND patch guidolib-src/src/include/GUIDOEngine.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/src/include/GUIDOEngine.h.patch
    COMMAND patch guidolib-src/src/include/GUIDOScoreMap.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/src/include/GUIDOScoreMap.h.patch
    COMMAND patch guidolib-src/src/include/VGColor.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/src/include/VGColor.h.patch
    COMMAND patch guidolib-src/win32/src/GSystemWin32GDIPlus.h ${CMAKE_CURRENT_SOURCE_DIR}/../patches/guidolib-src/win32/src/GSystemWin32GDIPlus.h.patch
    
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying and patching include header files"
)

add_custom_command(
    DEPENDS NotAFile_copy_and_patch
    OUTPUT NotAFile_shiboken #2 #${PYGUIDO_SRC}
    COMMAND ${SHIBOKEN_BINARY}
    --debug-level=sparse # sparse, medium, or full
    --generatorSet=shiboken
    --include-paths=${SHIBOKEN_INCLUDE_PATHS}
    --no-suppress-warnings
    --output-directory=${CMAKE_CURRENT_BINARY_DIR}
    --typesystem-paths="${PYSIDE_TYPESYSTEMS}${SHIBOKEN_INCLUDE_PATH_SEPARATOR}${CMAKE_CURRENT_SOURCE_DIR}/../PyGuido"
    
    ${SHIBOKEN_GENERATOR_EXTRA_FLAGS}

#    --avoid-protected-hack                  # Avoid the use of the '#define protected public' hack.
#    --disable-verbose-error-messages        # Disable verbose error messages. Turn the python code hard to debug but safe few kB on the generated bindings.
    --enable-parent-ctor-heuristic          # Enable heuristics to detect parent relationship on constructors.
    --enable-pyside-extensions              # Enable PySide extensions, such as support for signal/slots, use this if you are creating a binding for a Qt-based library.
    --enable-return-value-heuristic         # Enable heuristics to detect parent relationship on return values (USE WITH CAUTION!)
#    --use-isnull-as-nb_nonzero              # If a class have an isNull()const method, it will be used to compute the value of boolean casts

    ${PYGUIDO_GLOBAL_HEADER}
    ${PYGUIDO_TYPESYSTEM}
    
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running shiboken..."
)

if(FIX_METAOBJECT_HEADER_ERROR)
    message("will use sed to patch header files")
    add_custom_command(
        DEPENDS NotAFile_shiboken
        OUTPUT ${PYGUIDO_SRC}

        COMMAND sed -i.bak "'s%QGuidoGraphicsItemWrapper\(QGraphicsItem \\* parent = 0\);%QGuidoGraphicsItemWrapper\(QGraphicsItem * parent = 0\);virtual const QMetaObject* metaObject\(\) const;%'" ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidographicsitem_wrapper.h
        
        COMMAND sed -i.bak "'s%QGuidoWidgetWrapper\(QWidget \\* parent = 0\);%QGuidoWidgetWrapper\(QWidget * parent = 0\);virtual const QMetaObject* metaObject\(\) const;%'" ${CMAKE_CURRENT_BINARY_DIR}/PyGuido/qguidowidget_wrapper.h
        
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Patch files..."
    )
else()
    add_custom_command(
        DEPENDS NotAFile_shiboken
        OUTPUT ${PYGUIDO_SRC}
        COMMAND
        COMMENT "Getting ready to compile..."
    )
endif()


if(CMAKE_HOST_WIN32)
    # Windows
    add_custom_command(
        TARGET PyGuido
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ../lib/win
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Release/PyGuido.pyd ../lib/win/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying resulting PyGuido.pyd module to lib/win directory"
    )
    
elseif(CMAKE_HOST_APPLE)
    # Mac
    add_custom_command(
        TARGET PyGuido
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ../lib/mac
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/PyGuido.so ../lib/mac/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying resulting PyGuido.so module to lib/mac directory"
    )

elseif(CMAKE_HOST_UNIX)
    # assume Linux
    add_custom_command(
        TARGET PyGuido
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ../lib/linux
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/PyGuido.so ../lib/linux/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying resulting PyGuido.so module to lib/linux directory"
    )
    
else()
    # unknown host
endif()
