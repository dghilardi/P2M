#-------------------------------------------------
#
# Project created by QtCreator 2015-04-11T12:19:48
#
#-------------------------------------------------
unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += yaml-cpp
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P2M
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mailcomposer.cpp \
    mailsender.cpp

# Location of SMTP Library
SMTP_LIBRARY_LOCATION = $$PWD/../../../build/SMTPEmail-Desktop-Debug

win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION -lSMTPEmail -lquazip

HEADERS  += mainwindow.h \
    mailcomposer.h \
    mailsender.h

FORMS    += mainwindow.ui
