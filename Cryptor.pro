QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4

QT += core gui
QT -= opengl phonon

TARGET = Cryptor
TEMPLATE = app

SOURCES += \
    src/Cryptor.cpp \
    src/CryptFile.cpp

HEADERS  += \
    src/CryptFile.hpp

CONFIG -= warn_off
CONFIG += warn_on
QMAKE_CFLAGS   += -Wall -Wextra
QMAKE_CXXFLAGS += -Wall -Wextra
