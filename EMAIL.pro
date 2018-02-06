#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T16:44:42
#
#-------------------------------------------------

QT       += core gui network sql tcp

TARGET = EMAIL
TARGET_EXT = .BIN

release: CONFIG -= debug
debug {
    DESTDIR = bin-debug
    OBJECTS_DIR = build-debug
    MOC_DIR = build-debug
    UI_DIR = build-debug
    RCC_DIR = build-debug
}
release {
    DESTDIR = bin-release
    OBJECTS_DIR = build-release
    MOC_DIR = build-release
    UI_DIR = build-release
    RCC_DIR = build-release
}

LIBS	+= -Wl,-rpath,./lib

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/smtp.cpp
QMAKE         = C:/Qt/4.8.6/bin/qmake.exe

HEADERS  += include/mainwindow.h \
    include/smtp.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH += "include"
