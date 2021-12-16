#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QGraphicsProxyWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* human;
    QGraphicsItemGroup* phone;
    QList<QGraphicsRectItem*>* borders;
    QPoint* dXY;
    double speed;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void moveAndCheck();
    void borderCheck();

private:
    QMap<QString, QGraphicsProxyWidget*> elements;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
