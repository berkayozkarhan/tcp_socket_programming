QT -= gui
QT += httpserver
QT += concurrent
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bank.cpp \
        client.cpp \
        database.cpp \
        main.cpp \
        server.cpp \
        user.cpp \
        utils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bank.h \
    client.h \
    database.h \
    globals.h \
    server.h \
    user.h \
    utils.h
