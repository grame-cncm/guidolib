cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 11)

#######################################
set (SRCDIR  		${CMAKE_CURRENT_LIST_DIR})
set (GUIDOQTLIBDIR 	${CMAKE_CURRENT_LIST_DIR}/../../libs)
set (GUIDOQTDIR 	${GUIDOQTLIBDIR}/GuidoQt)

if (QT6)
	find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)
	set (QTINCDIRS 	${Qt6Widgets_INCLUDE_DIRS} ${Qt6PrintSupport_INCLUDE_DIRS})
	set (QTLIBS 	Qt6::Widgets Qt6::PrintSupport)
	set ( GUIDOEDITOR_DEFINITIONS -DQTTABSTOPDISTANCE -DQTFONTMETRICS_511 -DQTSETPAGESIZE)
	set (QTVERS		${Qt6Widgets_VERSION})
else()
	find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
	set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
	set (QTLIBS 	Qt5::Widgets Qt5::PrintSupport)
	set (QTVERS		${Qt5Widgets_VERSION})
endif()

#######################################
# set sources and headers files
set (GUIDO2IMAGE_SRC "${SRCDIR}/main.cpp")
if (WIN32)
	set (GUIDO2IMAGE_SRC ${GUIDO2IMAGE_SRC} "${SRCDIR}/XGetopt.cpp")
	set (GUIDO2IMAGE_HEADERS "${SRCDIR}/XGetopt.h")
endif()

#######################################
# set includes
set (GUIDO2IMAGE_INCLUDE_DIRS ${GUIDOQTDIR}/include ${QTINCDIRS})

#######################################
# set libraries and definitions
if (WIN32)
	set (GUIDO2IMAGE_LIBRARIES ${GUIDOQTLIBDIR}/GuidoQt.lib)
else()
	set (GUIDO2IMAGE_LIBRARIES -L${GUIDOQTLIBDIR} -lGuidoQt)
	set (GUIDO2IMAGE_DEFINITIONS -DUSE_UNISTD)
endif()
set (GUIDO2IMAGE_LIBRARIES ${GUIDO2IMAGE_LIBRARIES} ${QTLIBS})

message (STATUS "Include guido2image tools using Qt version ${QTVERS}")
