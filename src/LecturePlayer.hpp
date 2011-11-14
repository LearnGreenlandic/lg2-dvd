#ifndef LECTUREPLAYER_H
#define LECTUREPLAYER_H

#include "common.hpp"
#include "ScalingGraphicsView.hpp"
#include "TaskChooser.hpp"

#include <QAxWidget>
#include <QAxObject>
#include <QtGui>
#include <stdint.h>

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

#endif // LECTUREPLAYER_H
