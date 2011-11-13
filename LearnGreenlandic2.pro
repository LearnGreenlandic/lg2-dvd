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
    src/CryptFile.cpp \
    src/common.cpp \
    src/QAData.cpp \
    src/QATextOnly.cpp \
    src/ScalingGraphicsView.cpp \
    src/TaskChooser.cpp

HEADERS  += \
    src/CryptFile.hpp \
    src/common.hpp \
    src/QAData.hpp \
    src/QATextOnly.hpp \
    src/ScalingGraphicsView.hpp \
    src/TaskChooser.hpp

CONFIG -= warn_off
CONFIG += warn_on
CONFIG += qaxcontainer
QMAKE_CFLAGS   += -Wall -Wextra
QMAKE_CXXFLAGS += -Wall -Wextra
