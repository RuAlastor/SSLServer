TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    masterserver.cpp \
    parser.cpp \
    signer.cpp

LIBS += -lcrypto

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libssl

HEADERS += \
    headers.h \
    masterserver.h \
    parser.h \
    signer.h \
    mastersocketexceptions.h

unix: PKGCONFIG += libxml++-2.6
