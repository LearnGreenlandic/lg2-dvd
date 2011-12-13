#ifndef FILLOUT11_HPP
#define FILLOUT11_HPP

#include "TaskChooser.hpp"
#include <QtGui>

class Fillout11 : public QWidget {
    Q_OBJECT

public:
    Fillout11(TaskChooser& tc, QString which, QString title);

private:
    TaskChooser& tc;
};

#endif // FILLOUT11_HPP
