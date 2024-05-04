QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    futuresform.cpp \
    main.cpp \
    mainwindow.cpp \
    utils.cpp \
    wei_tuo.cpp

HEADERS += \
    futuresform.h \
    mainwindow.h \
    utils.h \
    wei_tuo.h

FORMS += \
    futuresform.ui \
    mainwindow.ui

TRANSLATIONS += \
    FuturesGame_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
