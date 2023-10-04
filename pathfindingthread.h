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
    void setScene(QGraphicsScene *scene);
    void setGrid(const QVector<QVector<int>> &newGrid);
    void clearPath();

signals:
    void pathFoundSignal(const std::vector<QPoint> &path); // Сигнал для передачи найденного пути
    void pathNotFoundSignal(); // Сигнал для передачи информации о том, что путь не найден

private:
    QPoint startPoint;
    QPoint endPoint;
    QGraphicsScene *scene;
    QVector<QVector<int>> grid;
    std::vector<QPoint> path;

    QList<QPoint> getNeighbors(const QPoint &point);
};

#endif // PATHFINDINGTHREAD_H
