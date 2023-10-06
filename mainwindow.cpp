#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customgraphicsview.h"
#include "pathfindingthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
    , pathfindingThread(nullptr)
    , customView(nullptr)
    , startPoint(-1, -1)
    , endPoint(-1, -1)
    , fieldWidth(0)
    , fieldHeight(0)
{
    ui->setupUi(this);

    pathfindingThread = new PathfindingThread(this);

    scene = new QGraphicsScene(this);
    customView = new CustomGraphicsView(scene, this);
    ui->verticalLayout->addWidget(customView);

    connect(ui->pushButton_generate, &QPushButton::clicked, this, &MainWindow::createScene);
    connect(pathfindingThread, SIGNAL(pathFoundSignal()), this, SLOT(updatePath()));
    connect(pathfindingThread, SIGNAL(pathNotFoundSignal()), this, SLOT(handlePathNotFound()));


    connect(customView, &CustomGraphicsView::setStartPointSignal, this, &MainWindow::setStartPoint);
    connect(customView, &CustomGraphicsView::setEndPointSignal, this, &MainWindow::setEndPoint);

    loadSettings();
}

MainWindow::~MainWindow()
{
    if (pathfindingThread != nullptr)
    {
        delete pathfindingThread;
    }
    if (customView != nullptr)
    {
        delete customView;
    }
    if (scene != nullptr)
    {
        delete scene;
    }
    if (ui != nullptr)
    {
        delete ui;
    }

}

void MainWindow::createScene()
{
    grid.clear();

    fieldWidth = ui->lineEdit_W->text().toInt();
    fieldHeight = ui->lineEdit_H->text().toInt();

    if (fieldWidth <= 0 || fieldHeight <= 0)
    {
        QMessageBox::critical(this, "Ошибка", "Некорректные размеры поля");
        return;
    }

    customView->generateField(fieldWidth, fieldHeight);
    grid = customView->getGrid();
    pathfindingThread->setSceneAndGrid(scene, grid);
}

void MainWindow::setStartPoint(const QPoint &point)
{
    startPoint = point;
    pathfindingThread->setStartPoint(startPoint);
}

void MainWindow::setEndPoint(const QPoint &point)
{
    endPoint = point;
    pathfindingThread->setEndPoint(endPoint);
    if (!startPoint.isNull())
    {
        pathfindingThread->start();
    }
}

void MainWindow::updatePath()
{
    path = pathfindingThread->GetPath();
    customView->displayPath(path);
}

void MainWindow::handlePathNotFound()
{
    QMessageBox::critical(this, "Ошибка", "Путь не найден");
}

void MainWindow::loadSettings()
{
    QSettings settings("MyCompany", "PathfindingApp");
    QPoint windowPos = settings.value("windowPos", QPoint(100, 100)).toPoint();
    fieldWidth = settings.value("fieldWidth", 10).toInt();
    fieldHeight = settings.value("fieldHeight", 10).toInt();

    resize(800, 600);
    move(windowPos);
}

void MainWindow::saveSettings()
{
    QSettings settings("MyCompany", "PathfindingApp");
    settings.setValue("windowPos", pos());
    settings.setValue("fieldWidth", fieldWidth);
    settings.setValue("fieldHeight", fieldHeight);
}
