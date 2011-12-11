#ifndef LISTENREPEATPLAYER_H
#define LISTENREPEATPLAYER_H

#include "CryptFile.hpp"
#include "TaskChooser.hpp"

#include <QAxWidget>
#include <QAxObject>
#include <QtGui>
#include <stdint.h>

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

#endif // LISTENREPEATPLAYER_H
