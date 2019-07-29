TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    parser.cpp \
    signer.cpp \
    server.cpp \
    sockets.cpp \
    slave.cpp

#LIBS += -lcrypto

unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libssl

HEADERS += \
    headers.h \
    parser.h \
    signer.h \
    sockets.h \
    server.h \
    slave.h

unix: PKGCONFIG += libxml++-2.6
