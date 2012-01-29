#include "ClickLabel.hpp"

ClickLabel::ClickLabel(QString text) :
QLabel(text)
{
    setMouseTracking(true);
    setContentsMargins(10, 5, 10, 5);
}

void ClickLabel::mousePressEvent(QMouseEvent*) {
    emit onClick();
}

void ClickLabel::click() {
    emit onClick();
}
