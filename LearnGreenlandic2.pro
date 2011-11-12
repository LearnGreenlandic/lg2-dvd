QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4

QT += core gui phonon network
QT -= opengl

TARGET = LearnGreenlandic2
TEMPLATE = app

TRANSLATIONS = texts_da.ts texts_en.ts

RC_FILE = lg.rc
ICON = icon.icns

SOURCES += \
    src/LearnGreenlandic2.cpp \
    src/CryptFile.cpp

HEADERS  += \
    src/CryptFile.hpp

CONFIG -= warn_off
CONFIG += warn_on
CONFIG += qaxcontainer
QMAKE_CFLAGS   += -Wall -Wextra
QMAKE_CXXFLAGS += -Wall -Wextra
