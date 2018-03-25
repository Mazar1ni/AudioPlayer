#-------------------------------------------------
#
# Project created by QtCreator 2017-06-26T13:13:58
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/../../AudioPlayer/Debug
} else {
    DESTDIR = $$OUT_PWD/../../AudioPlayer/Release
}
# разделяем по директориям все выходные файлы проекта
MOC_DIR = ../common/build/moc
RCC_DIR = ../common/build/rcc
UI_DIR = ../common/build/ui
unix:OBJECTS_DIR = ../common/build/o/unix
win32:OBJECTS_DIR = ../common/build/o/win32
macx:OBJECTS_DIR = ../common/build/o/mac

# в зависимости от режима сборки проекта
# запускаем win deploy приложения в целевой директории, то есть собираем все dll
CONFIG(debug, debug|release) {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/../../AudioPlayer/Debug
} else {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/../../AudioPlayer/Release
}


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc
