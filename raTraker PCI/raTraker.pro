#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T09:01:58
#
#-------------------------------------------------

QT       += core gui

TARGET = raTracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    satview.cpp \
    satscene.cpp \
    satitem.cpp \
    global.cpp \
    core/stdafx.cpp \
    core/globals.cpp \
    core/cVector.cpp \
    core/cTLE.cpp \
    core/cSite.cpp \
    core/cOrbit.cpp \
    core/coord.cpp \
    core/cNoradSGP4.cpp \
    core/cNoradSDP4.cpp \
    core/cNoradBase.cpp \
    core/cJulian.cpp \
    core/cEci.cpp \
    trackerthread.cpp \
    azieleplot.cpp \
    trackerwidget.cpp \
    dialog.cpp \
    eleplot.cpp

HEADERS  += mainwindow.h \
    satview.h \
    satscene.h \
    satitem.h \
    global.h \
    core/globals.h \
    core/exceptions.h \
    core/cVector.h \
    core/cTLE.h \
    core/cSite.h \
    core/coreLib.h \
    core/cOrbit.h \
    core/coord.h \
    core/cNoradSGP4.h \
    core/cNoradSDP4.h \
    core/cNoradBase.h \
    core/cJulian.h \
    core/cEci.h \
    core/stdafx.h \
    trackerthread.h \
    azieleplot.h \
    trackerwidget.h \
    non_gui_clases/VARY.H \
    Guid.h \
    dialog.h \
    eleplot.h

FORMS    += mainwindow.ui \
    dialog.ui

RC_FILE = app.rc
RESOURCES += \
    resources.qrc

LIBS += -L./lib -lSetupAPI
