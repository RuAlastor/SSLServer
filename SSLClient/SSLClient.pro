TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /home/student/C++/SSLServer

LIBS += -lcrypto

SOURCES += main.cpp \
    client.cpp \
    ../SSLServer/parser.cpp \
    ../SSLServer/slave.cpp \
    ../SSLServer/sockets.cpp

unix: CONFIG += link_pkgconfig

unix: PKGCONFIG += libxml++-2.6
unix: PKGCONFIG += libssl

HEADERS += \
    headers.h \
    client.h \
    ../SSLServer/parser.h \
    ../SSLServer/slave.h \
    ../SSLServer/sockets.h
