#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T16:31:07
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Movierilla
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           movieselector.cpp

HEADERS  += mainwindow.h \
            movieselector.h

FORMS    += mainwindow.ui \
            movieselector.ui
