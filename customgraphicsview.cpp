// customgraphicsview.cpp

#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
}

CustomGraphicsView::~CustomGraphicsView()
{
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit mouseClicked(event);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    double scaleFactor = (delta > 0) ? 1.1 : 1 / 1.1;
    scale(scaleFactor, scaleFactor);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event); // Отправляем сигнал о движении мыши
}
