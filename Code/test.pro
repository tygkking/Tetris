#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T11:45:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    gamepage.cpp \
    piece.cpp \
    makesure.cpp \
    chooseoption.cpp \
    chooseoption.cpp \
    highscore.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    gamepage.h \
    piece.h \
    makesure.h \
    chooseoption.h \
    chooseoption.h \
    highscore.h

FORMS    += mainwindow.ui \
    dialog.ui \
    gamepage.ui \
    makesure.ui \
    chooseoption.ui \
    chooseoption.ui \
    highscore.ui

RC_ICONS = icon.ico
