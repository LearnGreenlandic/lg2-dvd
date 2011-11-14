#ifndef QATEXTONLY_HPP
#define QATEXTONLY_HPP

#include "QAData.hpp"
#include "TaskChooser.hpp"
#include <QtGui>

class QATextOnly : public QWidget {
    Q_OBJECT

public:
    explicit QATextOnly(TaskChooser& tc, QString which, QString title);

public slots:
    void showNext();
    void checkInput();
    void yieldWord();

private:
    TaskChooser& tc;

    QAData data;
    int curAt;

    QLabel *query, *result;
    QLineEdit *input;
    QPushButton *yield;
};

#endif // QATEXTONLY_HPP
