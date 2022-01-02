cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)


#######################################
set (SRCDIR  	${CMAKE_CURRENT_LIST_DIR})
set (GUIDODIR  	${CMAKE_CURRENT_LIST_DIR}/../../../..)

if (QT6)
find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt6Widget_INCLUDE_DIRS} ${Qt6PrintSupport_INCLUDE_DIRS})
set (QTVERS ${Qt6Widgets_VERSION} )
else()
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
set (QTINCDIRS 	${Qt5Widget_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})
set (QTVERS ${Qt5Widgets_VERSION} )
endif()

#######################################
# set sources and headers files
file (GLOB GUIDOQT_SRC           RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRCDIR}/*.cpp" )
file (GLOB GUIDOQT_HEADERS       RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRCDIR}/include/*.h" )

#######################################
# set includes
set (INCL ${INCL} 
	${SRCDIR}/include
	${GUIDODIR}/src/engine/include
)
file (GLOB INCDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INCL})
set (GUIDOQT_INCLUDE_DIRS ${INCDIRS} ${QTINCDIRS})

message (STATUS "GuidoQt library using Qt version ${QTVERS}")
