#include "pathfindingthread.h"

PathfindingThread::PathfindingThread(QObject *parent) : QThread(parent)
{
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    scene = nullptr;
}

PathfindingThread::~PathfindingThread()
{
    clearPath();
}

struct QPointComparator {
    bool operator()(const QPoint& p1, const QPoint& p2) const {
        return (p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y()));
    }
};

void PathfindingThread::run()
{
    if (startPoint != QPoint(-1, -1) && endPoint != QPoint(-1, -1))
    {
        // Определите начальную точку (А) и конечную точку (Б)
        QPoint startSquare = startPoint;
        QPoint endSquare = endPoint;

        // Создайте стек для выполнения поиска в глубину
        std::stack<QPoint> dfsStack;
        dfsStack.push(startSquare);

        // Создайте карту для отслеживания предков (для восстановления пути)
        std::map<QPoint, QPoint, QPointComparator> parentMap;

        bool pathFound = false;

        // Выполните поиск в глубину
        while (!dfsStack.empty())
        {
            QPoint currentSquare = dfsStack.top();
            dfsStack.pop();

            if (currentSquare == endSquare)
            {
                pathFound = true;
                break;
            }

            // Получите соседние квадраты (возможные ходы)
            QList<QPoint> neighbors = getNeighbors(currentSquare);

            for (const QPoint &neighbor : neighbors)
            {
                if (parentMap.find(neighbor) == parentMap.end())
                {
                    // Проверьте, является ли соседняя точка стеной
                    if (grid[neighbor.y()][neighbor.x()] != 1) {
                        dfsStack.push(neighbor);
                        parentMap[neighbor] = currentSquare;
                    }
                }
            }
        }


        if (pathFound)
        {
            QVector<QPoint> pathToEmit;
            QPoint current = endSquare;
            while (current != QPoint(-1, -1) && current != startPoint)
            {
                pathToEmit.prepend(current);
                current = parentMap[current];
            }
            pathToEmit.prepend(startPoint);
            SetPath(pathToEmit);

            emit pathFoundSignal();
        }
        else
        {
            emit pathNotFoundSignal();
        }
    }
}

QList<QPoint> PathfindingThread::getNeighbors(const QPoint &point)
{
    QList<QPoint> neighbors;

    if (point.y() - 1 >= 0)
        neighbors.push_back(QPoint(point.x(), point.y() - 1)); // Сверху
    if (point.y() + 1 < grid.size())
        neighbors.push_back(QPoint(point.x(), point.y() + 1)); // Снизу
    if (point.x() - 1 >= 0)
        neighbors.push_back(QPoint(point.x() - 1, point.y())); // Слева
    if (point.x() + 1 < grid[0].size())
        neighbors.push_back(QPoint(point.x() + 1, point.y())); // Справа

    return neighbors;
}

QVector<QPoint> PathfindingThread::GetPath() const
{
    return path;
}

void PathfindingThread::SetPath(const QVector<QPoint>& newPath)
{
    path = newPath;
}

void PathfindingThread::setStartPoint(const QPoint &point)
{
    startPoint = point;
}

void PathfindingThread::setEndPoint(const QPoint &point)
{
    endPoint = point;
}

void PathfindingThread::setSceneAndGrid(QGraphicsScene *scene, const QVector<QVector<int>> &grid)
{
    this->scene = scene;
    this->grid = grid;
}

void PathfindingThread::clearPath()
{
    path.clear();
}
