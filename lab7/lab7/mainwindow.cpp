#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QDialog>
#include <QSpinBox>
#include <QDebug>
#include <QColor>
#include <QTimer>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 650, 400);

    int borderWidth = 10;
    dXY = new QPoint(2, 2);

    QGraphicsRectItem* topBorder = new QGraphicsRectItem(0, 0, scene->width(), borderWidth);
    QGraphicsRectItem* rightBorder = new QGraphicsRectItem(scene->width() - borderWidth, 0, borderWidth, scene->height());
    QGraphicsRectItem* bottomBorder = new QGraphicsRectItem(0, scene->height() - borderWidth, scene->width(), borderWidth);
    QGraphicsRectItem* leftBorder = new QGraphicsRectItem(0, 0, borderWidth, scene->height());

    borders = new QList<QGraphicsRectItem*>;
    borders->append(topBorder);
    borders->append(rightBorder);
    borders->append(leftBorder);
    borders->append(bottomBorder);

    topBorder->setBrush(QBrush(QColor(12, 181, 0)));
    rightBorder->setBrush(QBrush(QColor(12, 181, 0)));
    leftBorder->setBrush(QBrush(QColor(12, 181, 0)));
    bottomBorder->setBrush(QBrush(QColor(12, 181, 0)));

    scene->addItem(topBorder);
    scene->addItem(rightBorder);
    scene->addItem(leftBorder);
    scene->addItem(bottomBorder);

    human = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/human.png")).scaledToHeight(190));
    human->setFlags(QGraphicsItem::ItemIsMovable);
    human->setPos(scene->width()/2, scene->height()/2);

    scene->addItem(human);

    QGraphicsRectItem* telephone = new QGraphicsRectItem(0, 0, 15, 30);
    QGraphicsRectItem* telephoneScreen = new QGraphicsRectItem(3, 3, 9, 25);
    QGraphicsRectItem* telephoneAntenna = new QGraphicsRectItem(5, 0, -2, -5);

    telephone->setBrush(QBrush(QColor(0,0,0)));
    telephoneScreen->setBrush(QBrush(QColor(36, 218, 255)));
    telephoneAntenna->setBrush(QBrush(QColor(150, 150, 150)));

    QList<QGraphicsItem*> allPhone;
    allPhone.append(telephone);
    allPhone.append(telephoneScreen);
    allPhone.append(telephoneAntenna);

    scene->addItem(telephone);
    scene->addItem(telephoneScreen);
    scene->addItem(telephoneAntenna);

    phone = scene->createItemGroup(allPhone);
    phone->setPos(scene->width()/3, scene->height()/3);
    phone->setFlag(QGraphicsItem::ItemIsMovable);
    speed = 1;

    QTimer* mainTimer = new QTimer();
    mainTimer->setInterval(16);
    mainTimer->setSingleShot(false);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(moveAndCheck()));
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(borderCheck()));
    mainTimer->start();


}

void MainWindow::borderCheck() {
    if(human->x() < 0 || human->x() > scene->width()-50 || human->y() < 0 || human->y() > scene->height()-50) {
        human->setPos(scene->width()/2, scene->height()/2);
    }
    if(phone->x() < 0 || phone->x() > scene->width()-50 || phone->y() < 0 || phone->y() > scene->height()-50) {
        phone->setPos(scene->width()/3, scene->height()/3);
    }
}

void MainWindow::moveAndCheck() {
    phone->moveBy(dXY->x() * speed, dXY->y() * speed);

    if (phone->collidesWithItem(borders->at(0)) || phone->collidesWithItem(borders->at(3))) {
        dXY->setY(dXY->y() * -1);
    }

    if (phone->collidesWithItem(borders->at(1)) || phone->collidesWithItem(borders->at(2))) {
        dXY->setX(dXY->x() * -1);
    }

    if (phone->collidesWithItem(human)) {
        dXY = new QPoint(dXY->x() * -1, dXY->y() * -1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
