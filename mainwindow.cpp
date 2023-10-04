#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"
#include "pathfindingthread.h"
#include <stack>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pathfindingThread = new PathfindingThread(this);

    scene = new QGraphicsScene(this);
    CustomGraphicsView* customView = new CustomGraphicsView(scene, this);
    ui->verticalLayout->addWidget(customView);

    connect(ui->pushButton_generate, &QPushButton::clicked, this, &MainWindow::generateField);
    connect(pathfindingThread, &PathfindingThread::pathFoundSignal, this, &MainWindow::updatePath);
    connect(pathfindingThread, &PathfindingThread::pathNotFoundSignal, this, &MainWindow::handlePathNotFound);

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createScene()
{
    // Отобразите сетку
    for (int x = 0; x < fieldWidth; ++x)
    {
        for (int y = 0; y < fieldHeight; ++y)
        {
            QGraphicsRectItem *gridItem = new QGraphicsRectItem(x * 50, y * 50, 50, 50);
            gridItem->setPen(QPen(Qt::gray)); // Цвет сетки
            scene->addItem(gridItem);
        }
    }
}

void MainWindow::generateField()
{
    clearField();
    grid.clear();

    fieldWidth = ui->lineEdit_W->text().toInt();
    fieldHeight = ui->lineEdit_H->text().toInt();

    if (fieldWidth <= 0 || fieldHeight <= 0) {
        QMessageBox::critical(this, "Ошибка", "Некорректные размеры поля");
        return;
    }

    createScene();
    scene->setSceneRect(0, 0, fieldWidth * 50, fieldHeight * 50);

    for (int i = 0; i < fieldHeight; ++i) {
        QVector<int> row;
        for (int j = 0; j < fieldWidth; ++j) {
            int value = qrand() % 4; // 25% вероятность, что квадрат будет стеной
            row.append(value);
            if (value == 1) {
                QGraphicsRectItem *wall = scene->addRect(j * 50, i * 50, 50, 50, QPen(), QBrush(Qt::black));
                wall->setZValue(1);
            }
        }
        grid.append(row);
    }

    startPoint = endPoint = QPoint(-1, -1);
    path.clear();

    saveSettings();

    // Установите начальную и конечную точки для поиска пути
    pathfindingThread->setStartPoint(startPoint);
    pathfindingThread->setEndPoint(endPoint);

    // Запустите поток для выполнения поиска пути
    pathfindingThread->start();
}

/*void MainWindow::handleSquareClick(QMouseEvent *event)
{
    // Обработка клика по квадрату (установка точек А и Б)
    QPoint squarePos = ui->graphicsView->mapToScene(event->pos()).toPoint();
    int x = squarePos.x() / 50;
    int y = squarePos.y() / 50;

    if (x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight)
    {
        if (event->button() == Qt::LeftButton)
        {
            if (startPoint != QPoint(-1, -1))
            {
                // Очистка предыдущей точки А
                QGraphicsItem *prevStart = scene->itemAt(startPoint.x() * 50 + 1, startPoint.y() * 50 + 1, QTransform());
                if (prevStart)
                {
                    QGraphicsRectItem *startRect = dynamic_cast<QGraphicsRectItem*>(prevStart);
                    if (startRect)
                        scene->removeItem(startRect);
                }
            }

            startPoint = QPoint(x, y);
            QGraphicsEllipseItem *start = scene->addEllipse(x * 50 + 10, y * 50 + 10, 30, 30, QPen(), QBrush(Qt::green));
            start->setZValue(1);
        }
        else if (event->button() == Qt::RightButton)
        {
            if (endPoint != QPoint(-1, -1))
            {
                // Очистка предыдущей точки Б
                QGraphicsItem *prevEnd = scene->itemAt(endPoint.x() * 50 + 1, endPoint.y() * 50 + 1, QTransform());
                if (prevEnd)
                {
                    QGraphicsRectItem *endRect = dynamic_cast<QGraphicsRectItem*>(prevEnd);
                    if (endRect)
                        scene->removeItem(endRect);
                }
            }

            endPoint = QPoint(x, y);
            QGraphicsEllipseItem *end = scene->addEllipse(x * 50 + 10, y * 50 + 10, 30, 30, QPen(), QBrush(Qt::red));
            end->setZValue(1);
        }
    }
}

void MainWindow::handleMouseMove(QMouseEvent *event)
{
    // Обработка движения мыши (отображение пути от А до текущей позиции)
    if (startPoint != QPoint(-1, -1))
    {
        // Определите текущую позицию мыши и конвертируйте ее в координаты сцены
        QPoint squarePos = ui->graphicsView->mapToScene(event->pos()).toPoint();
        int x = squarePos.x() / 50;
        int y = squarePos.y() / 50;

        // Вызовите функцию для обновления конечной точки в потоке
        pathfindingThread->setEndPoint(QPoint(x, y));
    }
}
*/
void MainWindow::updatePath(const std::vector<QPoint> &path)
{
    // Преобразуйте QVector<QPoint> в std::vector<QPoint>
    std::vector<QPoint> stdPath(path.begin(), path.end());

    // Обработка найденного пути
    displayPathOnField(stdPath);
}

void MainWindow::handlePathNotFound()
{
    // Обработка случая, когда путь не найден
    QMessageBox::critical(this, "Ошибка", "Путь не найден");
    return;
}

void MainWindow::displayPathOnField(const std::vector<QPoint> &path)
{
    // Очистите предыдущий путь, если он уже отображался
    clearField();

    // Отобразите путь на поле
    QPen pathPen(Qt::blue, 3); // Установите цвет и толщину линии пути

    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        QPoint startPoint = path[i];
        QPoint endPoint = path[i + 1];

        // Отобразите линию между точками
        QGraphicsLineItem *pathLine = new QGraphicsLineItem(startPoint.x() * 50 + 25, startPoint.y() * 50 + 25,
                                                            endPoint.x() * 50 + 25, endPoint.y() * 50 + 25);
        pathLine->setPen(pathPen);
        scene->addItem(pathLine);

        // Сохраните указатель на линию в векторе, чтобы можно было очистить путь позже
        pathLines.push_back(pathLine);
    }
}

void MainWindow::clearField()
{
    // Очистите предыдущий отображенный путь
    for (QGraphicsLineItem *pathLine : pathLines)
    {
        scene->removeItem(pathLine);
        delete pathLine;
    }
    pathLines.clear();
}

void MainWindow::loadSettings()
{
    QSettings settings("MyCompany", "PathfindingApp");
    QPoint windowPos = settings.value("windowPos", QPoint(100, 100)).toPoint();
    fieldWidth = settings.value("fieldWidth", 10).toInt();
    fieldHeight = settings.value("fieldHeight", 10).toInt();

    resize(800, 600); // Установите размер окна по умолчанию
    move(windowPos);
}

void MainWindow::saveSettings()
{
    QSettings settings("MyCompany", "PathfindingApp");
    settings.setValue("windowPos", pos());
    settings.setValue("fieldWidth", fieldWidth);
    settings.setValue("fieldHeight", fieldHeight);
}
