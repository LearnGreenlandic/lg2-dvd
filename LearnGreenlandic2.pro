QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4

QT += core gui phonon network
QT -= opengl

TARGET = LearnGreenlandic2
TEMPLATE = app

TRANSLATIONS = texts_da.ts texts_en.ts
CODECFORSRC = UTF-8
CODECFORTR = UTF-8

RC_FILE = lg.rc
ICON = icon.icns

SOURCES += \
    src/LearnGreenlandic2.cpp \
    src/CryptFile.cpp \
    src/common.cpp \
    src/QAData.cpp \
    src/QATextOnly.cpp \
    src/ScalingGraphicsView.cpp \
    src/TaskChooser.cpp \
    src/LecturePlayer.cpp \
    src/UpdownTwo.cpp \
    src/UpdownThree.cpp \
    src/UpdownOne.cpp \
    src/UpdownData.cpp \
    src/ListenRepeatPlayer.cpp \
    src/Fillout11.cpp \
    src/ListenRepeatAudio.cpp \
    src/QAAudio.cpp \
    src/ClickLabel.cpp \
    src/Fillout12.cpp \
    src/Fillout81.cpp \
    src/CorpusQuery.cpp \
    src/ValidateKey.cpp

HEADERS  += \
    src/CryptFile.hpp \
    src/common.hpp \
    src/QAData.hpp \
    src/QATextOnly.hpp \
    src/ScalingGraphicsView.hpp \
    src/TaskChooser.hpp \
    src/LecturePlayer.hpp \
    src/UpdownTwo.hpp \
    src/UpdownThree.hpp \
    src/UpdownOne.hpp \
    src/UpdownData.hpp \
    src/ListenRepeatPlayer.hpp \
    src/Fillout11.hpp \
    src/ListenRepeatAudio.hpp \
    src/QAAudio.hpp \
    src/ClickLabel.hpp \
    src/Fillout12.hpp \
    src/Fillout81.hpp \
    src/files.hpp \
    src/CorpusQuery.hpp \
    src/ValidateKey.hpp

CONFIG -= warn_off
CONFIG += warn_on
CONFIG += qaxcontainer
QMAKE_CFLAGS   += -Wall -Wextra
QMAKE_CXXFLAGS += -Wall -Wextra
