#-------------------------------------------------
#
# Project created by QtCreator 2013-11-17T17:51:20
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = CS3307Group3
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    qcustomplot.cpp \
    choosedata.cpp \
    widgetbarchart.cpp \
    widgetlinechart.cpp \
    widgettablechart.cpp \
    municipality.cpp \
    year.cpp \
    measure.cpp \
    servicetype.cpp \
    data.cpp \
    filterdata.cpp \
    widgetcomplinechart.cpp \
    displaywindow.cpp \
    graphtemplate.cpp


HEADERS  += mainwindow.h \
    qcustomplot.h \
    choosedata.h \
    widgetbarchart.h \
    widgetlinechart.h \
    widgettablechart.h \
    municipality.h \
    year.h \
    measure.h \
    servicetype.h \
    data.h \
    filterdata.h \
    widgetcomplinechart.h \
    displaywindow.h \
    graphtemplate.h


FORMS    += mainwindow.ui \
    choosedata.ui \
    widgetbarchart.ui \
    widgetlinechart.ui \
    widgettablechart.ui \
    widgetcomplinechart.ui \
    displaywindow.ui

