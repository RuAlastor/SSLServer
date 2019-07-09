TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    client.cpp \
    parser.cpp

LIBS += -lcrypto

HEADERS += \
    headers.h \
    client.h \
    exceptions.h \
    parser.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libxml++-2.6
unix: PKGCONFIG += libssl
