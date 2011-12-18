#ifndef FILLOUT11_HPP
#define FILLOUT11_HPP

#include "TaskChooser.hpp"
#include <QtGui>

class Fillout11 : public QWidget {
    Q_OBJECT

public:
    Fillout11(TaskChooser& tc, QString which, QString title);

public slots:
    void showNext();
    void checkInputs();

private:
    TaskChooser& tc;

    int curAt;
    QVector<QStringList> words;

    QLabel *query;
    QVector<QLineEdit*> inputs;
    QVector<QLabel*> checks;
};

#endif // FILLOUT11_HPP
