#ifndef QATEXTONLY_HPP
#define QATEXTONLY_HPP

#include "QAData.hpp"
#include <QtGui>

class QATextOnly : public QWidget {
    Q_OBJECT

public:
    explicit QATextOnly(const dirmap_t& dirs, QString which, QString title);

public slots:
    void showNext();
    void checkInput();
    void yieldWord();

private:
    QAData data;
    int curAt;

    QLabel *query, *result;
    QLineEdit *input;
    QPushButton *yield;
};

#endif // QATEXTONLY_HPP
