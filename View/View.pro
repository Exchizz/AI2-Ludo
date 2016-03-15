#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T10:40:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = View
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused
