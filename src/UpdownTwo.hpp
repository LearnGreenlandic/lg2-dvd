#ifndef UPDOWNTWO_HPP
#define UPDOWNTWO_HPP

#include "UpdownData.hpp"
#include "TaskChooser.hpp"

#include <QtGui>

class UpdownTwo : public QWidget {
    Q_OBJECT

public:
    UpdownTwo(TaskChooser& tc, QString which, QString title);

public slots:
    void skipExercise();
    void showNext();
    void checkInput();
    void yieldWord();

private:
    TaskChooser& tc;

    UpdownData data;
    int curAt;

    QLabel *up;
    QLabel *result;
    QLineEdit *input;
    QPushButton *check;
    QPushButton *yield;
};

#endif // UPDOWNTWO_HPP
