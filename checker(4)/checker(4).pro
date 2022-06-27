QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checkerbutton.cpp \
    chooseclient.cpp \
    chooseservice.cpp \
    clientwindow.cpp \
    main.cpp \
    mydialog.cpp \
    mydialog1.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    rank.cpp \
    room.cpp \
    serverwait.cpp \
    wait.cpp \
    widget.cpp

HEADERS += \
    checkerbutton.h \
    chooseclient.h \
    chooseservice.h \
    clientwindow.h \
    mydialog.h \
    mydialog1.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    rank.h \
    room.h \
    serverwait.h \
    wait.h \
    widget.h

FORMS += \
    clientwindow.ui \
    mydialog.ui \
    rank.ui \
    serverwait.ui \
    wait.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

RC_ICONS=mapp.ico

SUBDIRS += \
    test.pro \
    test.pro
