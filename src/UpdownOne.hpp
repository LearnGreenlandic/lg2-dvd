#ifndef UPDOWNONE_HPP
#define UPDOWNONE_HPP

#include "UpdownData.hpp"
#include "TaskChooser.hpp"

#include <QtGui>

class UpdownOne : public QWidget {
    Q_OBJECT

public:
    UpdownOne(TaskChooser& tc, QString which, QString title);

public slots:
    void skipExercise();
    void showNext();
    void showHelp();

private:
    TaskChooser& tc;

    UpdownData data;
    int curAt;

    QLabel *up;
    QLabel *down;
    QPushButton *help;
};

#endif // UPDOWNONE_HPP
