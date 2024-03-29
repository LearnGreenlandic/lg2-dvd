#ifndef QATEXTONLY_HPP
#define QATEXTONLY_HPP

#include "QAData.hpp"
#include "TaskChooser.hpp"
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <QtGui>

class QATextOnly : public QWidget {
    Q_OBJECT

public:
    explicit QATextOnly(TaskChooser& tc, QString which, QString title);

public slots:
    void showNext();
    void checkInput();
    void yieldWord();
    void playAnswer();

private:
    TaskChooser& tc;

    QAData data;
    int curAt;

    QLabel *query, *result;
    QLineEdit *input;
    QPushButton *yield;

    Phonon::MediaObject *media;
    Phonon::AudioOutput *audio;
    QPushButton *playanswer;
};

#endif // QATEXTONLY_HPP
