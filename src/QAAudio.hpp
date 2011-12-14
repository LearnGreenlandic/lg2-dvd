#ifndef QAAUDIO_HPP
#define QAAUDIO_HPP

#include "QAData.hpp"
#include "TaskChooser.hpp"
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <QtGui>

class QAAudio : public QWidget {
    Q_OBJECT

public:
    explicit QAAudio(TaskChooser& tc, QString which, QString title);

public slots:
    void showNext();
    void checkInput();
    void yieldWord();
    void playQuestion();
    void playAnswer();

private:
    TaskChooser& tc;

    QAData data;
    int curAt;

    QLabel *result;
    QLineEdit *input;
    QPushButton *yield;

    Phonon::MediaObject *media_q;
    Phonon::AudioOutput *audio_q;
    QPushButton *playquestion;

    Phonon::MediaObject *media;
    Phonon::AudioOutput *audio;
    QPushButton *playanswer;
};

#endif // QAAUDIO_HPP
