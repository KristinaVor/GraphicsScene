#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>
#include <QSettings>

class PathfindingThread;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateField();
    void clearField();
    void loadSettings();
    void saveSettings();
    void handlePathNotFound();

    void updatePath(const std::vector<QPoint> &path);

private:
    void createGrid();
    void displayPathOnField(const std::vector<QPoint> &path);

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    PathfindingThread *pathfindingThread;
    QList<QGraphicsLineItem *> pathLines;
    QPoint startPoint;
    QPoint endPoint;
    int fieldWidth;
    int fieldHeight;
    QVector<QPoint> path;
    QVector<QVector<int>> grid;

};
#endif // MAINWINDOW_H
