#include "ScalingGraphicsView.hpp"
#include <QResizeEvent>

ScalingGraphicsView::ScalingGraphicsView(QGraphicsScene *scene, QWidget *parent) :
QGraphicsView(scene, parent)
{
}

void ScalingGraphicsView::resizeEvent(QResizeEvent*) {
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
