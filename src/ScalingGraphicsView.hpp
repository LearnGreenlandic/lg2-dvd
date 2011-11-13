#ifndef SCALINGGRAPHICSVIEW_HPP
#define SCALINGGRAPHICSVIEW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>

class ScalingGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ScalingGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
    void resizeEvent(QResizeEvent*);
};

#endif // SCALINGGRAPHICSVIEW_HPP
