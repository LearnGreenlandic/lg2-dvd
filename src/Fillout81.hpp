#ifndef FILLOUT81_HPP
#define FILLOUT81_HPP

#include "TaskChooser.hpp"
#include <QtGui>

class Fillout81 : public QWidget {
    Q_OBJECT

public:
    Fillout81(TaskChooser& tc, QString which, QString title);

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

#endif // FILLOUT81_HPP
