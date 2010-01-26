include( qlabnetview.pri )

TEMPLATE = subdirs

SUBDIRS = \
    src \


#Remark this to disable monitor module
CONFIG+=monitor
