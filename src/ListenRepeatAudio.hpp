#ifndef LISTENREPEATAUDIO_HPP
#define LISTENREPEATAUDIO_HPP

#include "TaskChooser.hpp"
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/videowidget.h>
#include <phonon/seekslider.h>
#include <phonon/effect.h>
#include <QtGui>

class ListenRepeatAudio : public QWidget {
    Q_OBJECT

public:
    ListenRepeatAudio(TaskChooser& tc, QString which, QString title);
    void closeEvent(QCloseEvent *event);

public slots:
    void show();
    void showNext();
    void checkInput();
    void yieldWord();
    void tick(qint64 time);
    void togglePlay();

private:
    TaskChooser& tc;

    QStringList words;
    int curAt;

    QLabel *sum;
    QLabel *query;
    QLineEdit *input;
    QLabel *result;
    QPushButton *yield;

    Phonon::MediaObject *media;
    Phonon::AudioOutput *audio;

    QPushButton *playpause;
    Phonon::SeekSlider *seeker;
    QLCDNumber *timeLcd;
};

#endif // LISTENREPEATAUDIO_HPP
