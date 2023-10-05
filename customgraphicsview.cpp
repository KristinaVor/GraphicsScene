// customgraphicsview.cpp

#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent), startPointSet(false), endPointSet(false)
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
    QPoint clickPos = event->pos();
    QPointF scenePos = mapToScene(clickPos);

    int x = static_cast<int>(scenePos.x() / 50);
    int y = static_cast<int>(scenePos.y() / 50);

    if (x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight)
    {
        if (!startPointSet && !endPointSet)
        {
            setStartPoint(QPoint(x, y));
            startPointSet = true;
        }
        else if (startPointSet && !endPointSet)
        {
            setEndPoint(QPoint(x, y));
            endPointSet = true;
        }
    }
}


void CustomGraphicsView::setStartPoint(const QPoint &point)
{
    QGraphicsItem *prevStart = scene()->itemAt(startPoint.x() * 50 + 1, startPoint.y() * 50 + 1, QTransform());
    if (prevStart)
    {
        QGraphicsRectItem *startRect = dynamic_cast<QGraphicsRectItem *>(prevStart);
        if (startRect)
            scene()->removeItem(startRect);
    }

    startPoint = point;
    QGraphicsEllipseItem *start = scene()->addEllipse(startPoint.x() * 50 + 10, startPoint.y() * 50 + 10, 30, 30, QPen(), QBrush(Qt::green));
    start->setZValue(1);

    emit setStartPointSignal(startPoint);
}

void CustomGraphicsView::setEndPoint(const QPoint &point)
{
    QGraphicsItem *prevEnd = scene()->itemAt(endPoint.x() * 50 + 1, endPoint.y() * 50 + 1, QTransform());
    if (prevEnd)
    {
        QGraphicsRectItem *endRect = dynamic_cast<QGraphicsRectItem *>(prevEnd);
        if (endRect)
            scene()->removeItem(endRect);
    }

    endPoint = point;
    QGraphicsEllipseItem *end = scene()->addEllipse(endPoint.x() * 50 + 10, endPoint.y() * 50 + 10, 30, 30, QPen(), QBrush(Qt::red));
    end->setZValue(1);

    emit setEndPointSignal(endPoint);
}



void CustomGraphicsView::generateField(int width, int height)
{
    scene()->clear();

    int squareSize = 50;
    fieldWidth = width;
    fieldHeight = height;

    for (int x = 0; x < fieldWidth; ++x)
    {
        for (int y = 0; y < fieldHeight; ++y)
        {
            QGraphicsRectItem *gridItem = new QGraphicsRectItem(x * squareSize, y * squareSize, squareSize, squareSize);
            gridItem->setPen(QPen(Qt::gray)); // Цвет сетки
            scene()->addItem(gridItem);
        }
    }

    scene()->setSceneRect(0, 0, fieldWidth * 50, fieldHeight * 50);

    for (int i = 0; i < fieldHeight; ++i)
    {
        QVector<int> row;
        for (int j = 0; j < fieldWidth; ++j)
        {
            int value = qrand() % 4; // 25% вероятность, что квадрат будет стеной
            row.append(value);
            if (value == 1)
            {
                QGraphicsRectItem *wall = scene()->addRect(j * 50, i * 50, 50, 50, QPen(), QBrush(Qt::black));
                wall->setZValue(1);
            }
        }
        grid.append(row);
    }
}


void CustomGraphicsView::displayPath(const std::vector<QPoint> &path)
{
    // Отобразите маршрут на поле
    QPen pathPen(Qt::blue, 3);

    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        QPoint startPoint = path[i];
        QPoint endPoint = path[i + 1];

        QGraphicsLineItem *pathLine = new QGraphicsLineItem(startPoint.x() * 50 + 25, startPoint.y() * 50 + 25,
                                                            endPoint.x() * 50 + 25, endPoint.y() * 50 + 25);
        pathLine->setPen(pathPen);
        scene()->addItem(pathLine);
    }
}
