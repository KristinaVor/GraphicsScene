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
    void setStartPoint(const QPoint &point);
    void setEndPoint(const QPoint &point);


    int fieldWidth;
    int fieldHeight;
    QPoint startPoint;
    QPoint endPoint;
    bool startPointSet;
    bool endPointSet;

    QVector<QVector<int>> grid;
};

#endif // CUSTOMGRAPHICSVIEW_H
