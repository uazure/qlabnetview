# -------------------------------------------------
# Project created by QtCreator 2009-10-19T11:30:01
# -------------------------------------------------
QT += network
TARGET = qlabnetview
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    about.cpp \
    assignCurvesDialog.cpp \
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
    gpibtablemodel.cpp \
    viewtabledata.cpp
HEADERS += mainwindow.h \
    about.h \
    assignCurvesDialog.h \
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
    gpibtablemodel.h \
    viewtabledata.h \
    main.h
FORMS += mainwindow.ui \
    about.ui \
    assignCurvesDialog.ui \
    networkDialog.ui \
    errormessagedialog.ui \
    experimentControl.ui \
    yesnodialog.ui \
    viewrawdata.ui \
    viewparseddata.ui \
    viewtabledata.ui

unix { 
    LIBS += -lqwt
    INCLUDEPATH += /usr/include/qwt5/
}
win32 { 
    LIBS += -L"../../" \
        -lqwt5
    INCLUDEPATH += ../../qwt5/
    CONFIG += release
}
RESOURCES += ../../icons.qrc


#installation related stuff
INSTALLS        += target
#INSTALLS        += translations
unix: INSTALLBASE = /usr/local
win32: INSTALLBASE = C:/qlabnetview

unix: target.path = $$INSTALLBASE/bin
win32: target.path = $$INSTALLBASE


# DEFINES = D_MYDEFINE
OTHER_FILES += todo.txt
