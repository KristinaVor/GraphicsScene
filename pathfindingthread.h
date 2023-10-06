#ifndef PATHFINDINGTHREAD_H
#define PATHFINDINGTHREAD_H

#include <QThread>
#include <QPoint>
#include <QVector>
#include <QList>
#include <stack>
#include <map>

class QGraphicsScene;

class PathfindingThread : public QThread
{
    Q_OBJECT

public:
    PathfindingThread(QObject *parent = nullptr);
    ~PathfindingThread();

    void run() override;
    void setStartPoint(const QPoint &point);
    void setEndPoint(const QPoint &point);
    void setSceneAndGrid(QGraphicsScene *scene, const QVector<QVector<int>> &grid);
    void SetPath(const QVector<QPoint>& newPath);
    QVector<QPoint> GetPath() const;

signals:
    void pathFoundSignal();
    void pathNotFoundSignal();

private:
    QPoint startPoint;
    QPoint endPoint;
    QGraphicsScene *scene;
    QVector<QVector<int>> grid;
    QVector<QPoint> path;

    QList<QPoint> getNeighbors(const QPoint &point);
    void clearPath();
};

#endif // PATHFINDINGTHREAD_H
