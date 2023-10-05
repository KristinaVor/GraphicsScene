#include "pathfindingthread.h"

PathfindingThread::PathfindingThread(QObject *parent) : QThread(parent)
{
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);

    scene = nullptr;
    grid.clear();
    path.clear();
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
        parentMap[startSquare] = QPoint(-1, -1);

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
                    dfsStack.push(neighbor);
                    parentMap[neighbor] = currentSquare;
                }
            }
        }

        if (pathFound)
        {
            path.clear();
            QPoint current = endSquare;
            while (current != QPoint(-1, -1))
            {
                path.prepend(current);
                current = parentMap[current];
            }

            // Отправьте сигнал с найденным путем
            emit pathFoundSignal(path);
        }
        else
        {    // Путь не найден
            emit pathNotFoundSignal();
        }
    }
}

QList<QPoint> PathfindingThread::getNeighbors(const QPoint &point)
{
    // Вернуть список соседних квадратов (возможных ходов)
    // Например, соседи сверху, снизу, слева и справа от point
    QList<QPoint> neighbors;
    neighbors.push_back(QPoint(point.x() - 1, point.y()));
    neighbors.push_back(QPoint(point.x() + 1, point.y()));
    neighbors.push_back(QPoint(point.x(), point.y() - 1));
    neighbors.push_back(QPoint(point.x(), point.y() + 1));
    return neighbors;
}

void PathfindingThread::setStartPoint(const QPoint &point)
{
    startPoint = point;
}

void PathfindingThread::setEndPoint(const QPoint &point)
{
    endPoint = point;
}

void PathfindingThread::setSceneAndGrid(QGraphicsScene *scene, const QVector<QVector<int>> &newGrid)
{
    this->scene = scene;
    grid = newGrid;
}


void PathfindingThread::clearPath()
{
    path.clear();
}
