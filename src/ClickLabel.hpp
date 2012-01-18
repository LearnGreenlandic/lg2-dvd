#ifndef CLICKLABEL_HPP
#define CLICKLABEL_HPP

#include <QLabel>

class ClickLabel : public QLabel {
    Q_OBJECT

public:
    ClickLabel(QString text);
    void mousePressEvent(QMouseEvent *e);

signals:
    void onClick();
};

#endif // CLICKLABEL_HPP
