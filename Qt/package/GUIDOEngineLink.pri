# GUIDOEngine library link for each platform
macx:LIBS += -F.. -framework GUIDOEngine
win32:LIBS += ../GUIDOEngine.lib
unix:!macx:LIBS += -L.. -lguidoengine
INCLUDEPATH += ../include
