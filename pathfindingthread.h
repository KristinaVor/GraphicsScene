#ifndef PATHFINDINGTHREAD_H
#define PATHFINDINGTHREAD_H

#include <QThread>
#include <QPoint>
#include <QGraphicsScene>
#include <QVector>
#include <QList>
#include <stack>
#include <map>
#include <vector>

class PathfindingThread : public QThread
{
    Q_OBJECT

public:
    explicit PathfindingThread(QObject *parent = nullptr);
    ~PathfindingThread(); // Добавлен деструктор

    void run() override;
    void setStartPoint(const QPoint &point);
    void setEndPoint(const QPoint &point);
    void clearPath();
    void setSceneAndGrid(QGraphicsScene *scene, const QVector<QVector<int>> &newGrid);

signals:
    void pathFoundSignal(const QVector<QPoint> &path); // Сигнал для передачи найденного пути
    void pathNotFoundSignal(); // Сигнал для передачи информации о том, что путь не найден

private:
    QPoint startPoint;
    QPoint endPoint;
    QGraphicsScene *scene;
    QVector<QVector<int>> grid;
    QVector<QPoint> path;

    QList<QPoint> getNeighbors(const QPoint &point);
};

#endif // PATHFINDINGTHREAD_H
