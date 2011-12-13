#ifndef LISTENREPEATPLAYER_H
#define LISTENREPEATPLAYER_H

#include "CryptFile.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>
//#undef Q_WS_WIN
#if defined(Q_WS_WIN)

#include <QAxWidget>
#include <QAxObject>
#include <QtGui>

class ListenRepeatPlayer : public QWidget {
    Q_OBJECT

public:
    ListenRepeatPlayer(TaskChooser& tc, QString which, QString title);

    void closeEvent(QCloseEvent *event);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void show();
    void showNext();
    void checkInput();
    void yieldWord();

private:
    TaskChooser& tc;

    QStringList words;
    int curAt;

    QAxWidget *video;
    QAxObject *controls;
    QString tmpfile;

    QLabel *sum;
    QLabel *query;
    QLineEdit *input;
    QLabel *result;
    QPushButton *yield;
};


#else

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/videowidget.h>
#include <phonon/seekslider.h>
#include <phonon/effect.h>
#include <QtGui>

class ListenRepeatPlayer : public QWidget {
    Q_OBJECT

public:
    ListenRepeatPlayer(TaskChooser& tc, QString which, QString title);

    void closeEvent(QCloseEvent *event);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void show();
    void showNext();
    void checkInput();
    void yieldWord();
    void tick(qint64 time);
    void togglePlay();
    void finished();

private:
    TaskChooser& tc;

    QStringList words;
    int curAt;

    QLabel *sum;
    QLabel *query;
    QLineEdit *input;
    QLabel *result;
    QPushButton *yield;

    CryptFile *mediafile;
    Phonon::MediaObject *media;
    Phonon::VideoWidget *video;
    Phonon::AudioOutput *audio;

    QPushButton *playpause;
    Phonon::SeekSlider *seeker;
    QLCDNumber *timeLcd;
};

#endif // Q_WS_WIN
#endif // LISTENREPEATPLAYER_H
