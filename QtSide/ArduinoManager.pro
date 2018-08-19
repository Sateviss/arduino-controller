#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T17:42:49
#
#-------------------------------------------------

QT += core gui widgets

QT += serialport

TARGET = ArduinoManager
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

CONFIG += c++17

CONFIG+= static

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    pinbutton.cpp \
    portreader.cpp \
    processrunner.cpp \
    addkeydialog.cpp

HEADERS += \
        mainwindow.h \
    pinbutton.h \
    portreader.h \
    processrunner.h \
    addkeydialog.h

FORMS += \
        mainwindow.ui \
    addkeydialog.ui

RESOURCES += \
    arduinoscheme.qrc

unix {
    QMAKE_POST_LINK += $$quote(python3 -m venv $${OUT_PWD}/venv;)
}

macx {
    QMAKE_POST_LINK += $$quote($${OUT_PWD}/venv/bin/pip3 install pyobjc-core pyobjc pyautogui;)
}

linux {
    QMAKE_POST_LINK += $$quote($${OUT_PWD}/venv/bin/pip3 install python3-xlib pyautogui wheel;)
}

win32 {
    QMAKE_POST_LINK += \"C:\Program Files\Python36\python\" -m venv \"$${OUT_PWD}/venv\" &&
    QMAKE_POST_LINK += \"$${OUT_PWD}/venv/scripts/pip3.exe\" install pyautogui
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

