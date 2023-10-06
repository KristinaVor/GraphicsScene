#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPoint>
#include <QMessageBox>
#include <QSettings>

class PathfindingThread;
class CustomGraphicsView;

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
    void createScene();
    void handlePathNotFound();
    void updatePath();
    void setStartPoint(const QPoint &point);
    void setEndPoint(const QPoint &point);
    void loadSettings();
    void saveSettings();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    PathfindingThread *pathfindingThread;
    CustomGraphicsView* customView;
    QList<QGraphicsLineItem *> pathLines;
    QPoint startPoint;
    QPoint endPoint;
    int fieldWidth;
    int fieldHeight;
    QVector<QPoint> path;
    QVector<QVector<int>> grid;

};
#endif // MAINWINDOW_H
