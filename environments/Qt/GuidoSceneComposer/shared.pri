SOURCES_DIR = src
RESOURCES_DIR = rsc

win32:DebugBuild:CONFIG += console
TEMPLATE = app
QT += xml
win32:DebugBuild {
	TARGET = $${APP_NAME}_debug
} else {
	TARGET = $${APP_NAME}
}
DESTDIR = ../bin
MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc
win32:!win32-g++ {
	TEMPLATE = vcapp
}


macx {
	RC_FILE += $$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$RESOURCES_DIR/$${APP_NAME}Info.plist
}

win32 {
	RC_FILE = $$RESOURCES_DIR/$${APP_NAME}.rc
}

####### SOURCES ######
SOURCES += $$SOURCES_DIR/*.cpp $$SOURCES_DIR/GUI/*.cpp $$SOURCES_DIR/GuidoItemContainer/*.cpp $$SOURCES_DIR/GuidoHighlighter/*.cpp 

####### HEADERS ######

HEADERS += $$SOURCES_DIR/GUI/*.h $$SOURCES_DIR/GuidoItemContainer/*.h $$SOURCES_DIR/GuidoHighlighter/*.h
INCLUDEPATH += $$SOURCES_DIR/GUI $$SOURCES_DIR/GuidoItemContainer $$SOURCES_DIR/GuidoHighlighter

###### RESOURCES ######
RESOURCES += $$RESOURCES_DIR/guido.qrc
