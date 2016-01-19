#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T21:13:31
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp

HEADERS  += mainwindow.h \
    myglwidget.h

FORMS    += mainwindow.ui
QT += widgets webkitwidgets

RESOURCES += \
    source.qrc

RC_ICONS += icon.ico
