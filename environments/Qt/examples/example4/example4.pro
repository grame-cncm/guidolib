TEMPLATE = app
SOURCES = main.cpp
#CONFIG += console
macx:CONFIG += app_bundle
DESTDIR = ./bin

include( ../examples.shared )
