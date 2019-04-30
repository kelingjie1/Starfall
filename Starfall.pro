#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T11:37:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Starfall
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += \
        Library/glm \
        Library/rapidjson/include \
        Library/ObjectiveGL/include



SOURCES += \
        QT/sfrenderview.cpp \
        Source/Camera/SFCamera.cpp \
        Source/Parser/SFParser.cpp \
        QT/main.cpp \
        QT/mainwindow.cpp

HEADERS += \
        QT/sfrenderview.h \
        Source/Base/SFEmitter.h \
        Source/Base/SFNode.h \
        Source/Base/SFShader.h \
        Source/Base/SFSystem.h \
        Source/Camera/SFCamera.h \
        Source/Emitter/SFFillEmitter.h \
        Source/Emitter/SFNormalEmitter.h \
        Source/Parser/SFParser.h \
        Source/Starfall.h \
        QT/mainwindow.h

FORMS += \
        QT/mainwindow.ui

CONFIG += mobility

TRANSLATIONS += QT/zh_cn.ts

MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
