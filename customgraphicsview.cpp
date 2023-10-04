// customgraphicsview.cpp

#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(true);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        scale(1.2, 1.2);
    }
    else
    {
        scale(1.0 / 1.2, 1.0 / 1.2);
    }

    event->accept();
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit mouseClicked(event);
}

