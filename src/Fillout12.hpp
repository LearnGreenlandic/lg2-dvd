#ifndef FILLOUT12_HPP
#define FILLOUT12_HPP

#include "TaskChooser.hpp"
#include <QtGui>

class Fillout12 : public QWidget {
    Q_OBJECT

public:
    Fillout12(TaskChooser& tc, QString which, QString title);

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

#endif // FILLOUT12_HPP
