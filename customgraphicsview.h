// customgraphicsview.h

#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

public slots:
    void generateField(int width, int height);
    void displayPath(const std::vector<QPoint> &path);

signals:
    void setStartPointSignal(const QPoint &point);
    void setEndPointSignal(const QPoint &point);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setStartPoint(int x, int y);
    void setEndPoint(int x, int y);


    int fieldWidth;
    int fieldHeight;
    QPoint startPoint;
    QPoint endPoint;

    QVector<QVector<int>> grid;
};

#endif // CUSTOMGRAPHICSVIEW_H
