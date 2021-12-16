#-------------------------------------------------
#
# Project created by QtCreator 2021-11-23T20:00:06
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++1z

TARGET = CourseWork
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += translations/QtLanguage_ru.ts

CODECFORSRC     = UTF-8

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    headertableitemeditpage.cpp \
    adddialog.cpp \
    headertableitem.cpp \
    widecombobox.cpp \
    calendarpopup.cpp \
    pdfhelper.cpp \
    messageboxfontwrapper.cpp \
    imagebutton.cpp \
    itemdelegate.cpp

HEADERS += \
        mainwindow.h \
    headertableitem.h \
    headertableitemeditpage.h \
    adddialog.h \
    widecombobox.h \
    calendarpopup.h \
    pdfhelper.h \
    messageboxfontwrapper.h \
    adddialogenum.h \
    imagebutton.h \
    itemdelegate.h

FORMS += \
        mainwindow.ui \
    headertableitem.ui \
    headertableitemeditpage.ui \
    adddialog.ui \
    calendarpopup.ui

RESOURCES += \
    resources.qrc
