# -------------------------------------------------
# Project created by QtCreator 2009-10-19T11:30:01
# -------------------------------------------------
QT += network
TARGET = qlnmonitor
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    about.cpp \
    networkDialog.cpp \
    errormessagedialog.cpp \
    ../gpibsocket/gpibsocket.cpp
HEADERS += mainwindow.h \
    about.h \
    networkDialog.h \
    errormessagedialog.h \
    ../gpibsocket/gpibsocket.h
FORMS += mainwindow.ui \
    about.ui \
    networkDialog.ui \
    errormessagedialog.ui
unix { 
    LIBS += -lqwt
    INCLUDEPATH += /usr/include/qwt5/
}
win32 { 
    LIBS += -L"../../" -lqwt5
    INCLUDEPATH += ../../qwt5/
    CONFIG += release
}
RESOURCES += ../../icons.qrc
