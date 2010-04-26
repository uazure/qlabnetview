# -------------------------------------------------
# Project created by QtCreator 2009-10-19T11:30:01
# -------------------------------------------------
QT += network
TARGET = qlabnetview
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    about.cpp \
    plot.cpp \
    zoomer.cpp \
    networkDialog.cpp \
    ../gpibsocket/gpibsocket.cpp \
    errormessagedialog.cpp \
    experimentControl.cpp \
    yesnodialog.cpp \
    viewrawdata.cpp \
    viewparseddata.cpp \
    gpibdata.cpp \
    viewtabledata.cpp \
    canvaspicker.cpp \
    setupcurvesdialog.cpp \
    plotcurve.cpp \
    plotcurvelistmodel.cpp
HEADERS += mainwindow.h \
    about.h \
    plot.h \
    zoomer.h \
    networkDialog.h \
    ../gpibsocket/gpibsocket.h \
    errormessagedialog.h \
    experimentControl.h \
    yesnodialog.h \
    viewrawdata.h \
    viewparseddata.h \
    gpibdata.h \
    viewtabledata.h \
    main.h \
    canvaspicker.h \
    setupcurvesdialog.h \
    plotcurve.h \
    plotcurvelistmodel.h
FORMS += mainwindow.ui \
    about.ui \
    networkDialog.ui \
    errormessagedialog.ui \
    experimentControl.ui \
    yesnodialog.ui \
    viewrawdata.ui \
    viewparseddata.ui \
    viewtabledata.ui \
    setupcurvesdialog.ui

# QWT_NAME = qwt5
QWT_NAME = qwt
QWT_INCLUDE_PATH = /usr/include/$$QWT_NAME
unix { 
    LIBS += -l$$QWT_NAME
    INCLUDEPATH += $$QWT_INCLUDE_PATH
}
win32 { 
    LIBS += -L"../../" \
        -lqwt5
    INCLUDEPATH += ../../qwt5/
    CONFIG += release
}
RESOURCES += ../../icons.qrc
message(Qt version: $$[QT_VERSION])
message(Using $$QWT_NAME header files installed in $$QWT_INCLUDE_PATH)

# installation related stuff
INSTALLS += target

# INSTALLS        += translations
unix:INSTALLBASE = /usr/local
win32:INSTALLBASE = C:/qlabnetview
unix:target.path = $$INSTALLBASE/bin
win32:target.path = $$INSTALLBASE

# DEFINES = D_MYDEFINE
OTHER_FILES += todo.txt
