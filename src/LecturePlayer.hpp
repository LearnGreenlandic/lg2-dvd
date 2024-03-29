#ifndef LECTUREPLAYER_H
#define LECTUREPLAYER_H

#include "common.hpp"
#include "ScalingGraphicsView.hpp"
#include "CryptFile.hpp"
#include "TaskChooser.hpp"
#include <QtGui>
#include <stdint.h>

#if defined(Q_WS_WIN)

#include <QAxWidget>
#include <QAxObject>

class LecturePlayer : public QWidget {
    Q_OBJECT

public:
    LecturePlayer(TaskChooser& tc, QString lecDir, QString title);

    void closeEvent(QCloseEvent *event);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void show();
    void tick();
    void stateChange(int state);
    void finished();
    void askNext();

private:
    TaskChooser& tc;

    typedef QMap<uint32_t, QString> slides_t;
    slides_t slides;
    QString curSlide;

    QGraphicsScene *scene;
    ScalingGraphicsView *view;
    QGraphicsPixmapItem *slide;

    QAxWidget *video;
    QAxObject *controls;
    QString tmpfile;
};

#else

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/videowidget.h>
#include <phonon/seekslider.h>
#include <phonon/effect.h>

class LecturePlayer : public QWidget {
    Q_OBJECT

public:
    LecturePlayer(TaskChooser& tc, QString lecDir, QString title);

    void closeEvent(QCloseEvent *event);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void show();
    void tick(qint64 time);
    void togglePlay();
    void finished();
    void askNext();

private:
    TaskChooser& tc;

    typedef QMap<uint32_t, QString> slides_t;
    slides_t slides;
    QString curSlide;

    QGraphicsScene *scene;
    ScalingGraphicsView *view;
    QGraphicsPixmapItem *slide;

    CryptFile *mediafile;
    Phonon::MediaObject *media;
    Phonon::VideoWidget *video;
    Phonon::AudioOutput *audio;

    QPushButton *playpause;
    Phonon::SeekSlider *seeker;
    QLCDNumber *timeLcd;
};

#endif // Q_WS_WIN
#endif // LECTUREPLAYER_H
