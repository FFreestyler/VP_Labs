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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 1280, 720);

    qreal borderWidth = 10;
    dXY = new QPoint(-10, -10);

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

    human = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/human.png")).scaledToHeight(300));
    human->setFlags(QGraphicsItem::ItemIsMovable);
    human->setPos(scene->width()/2, scene->height()/2);

    QGraphicsRectItem* telephone = new QGraphicsRectItem(0, 0, 25, 50);
    QGraphicsRectItem* telephoneScreen = new QGraphicsRectItem(3, 3, 18, 35);
    QGraphicsRectItem* telephoneAntenna = new QGraphicsRectItem(5, 0, -3, -10);

    telephone->setBrush(QBrush(QColor(0,0,0)));
    telephoneScreen->setBrush(QBrush(QColor(36, 218, 255)));
    telephoneAntenna->setBrush(QBrush(QColor(150, 150, 150)));

    scene->addItem(topBorder);
    scene->addItem(rightBorder);
    scene->addItem(leftBorder);
    scene->addItem(bottomBorder);

    scene->addItem(human);

    scene->addItem(telephone);
    scene->addItem(telephoneScreen);
    scene->addItem(telephoneAntenna);

    QList<QGraphicsItem*> allPhone;
    allPhone.append(telephone);
    allPhone.append(telephoneScreen);
    allPhone.append(telephoneAntenna);

    phone = scene->createItemGroup(allPhone);
    phone->setPos(scene->width()/2, scene->height()/2);
    phone->setFlag(QGraphicsItem::ItemIsMovable);
    speed = 1;

    this->setGeometry(100,100,960,540);

    QTimer* mainTimer = new QTimer();
    mainTimer->setInterval(16);
    mainTimer->setSingleShot(false);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(moveAndCheck()));
    mainTimer->start();

    QTimer* resizeTimer = new QTimer();
    resizeTimer->setInterval(0);
    resizeTimer->setSingleShot(true);
    connect(resizeTimer, SIGNAL(timeout()), this, SLOT(resizeTrigger()));
    resizeTimer->start();
}

void MainWindow::resizeTrigger() {
    emit resizeEvent(new QResizeEvent(this->size(), this->size()));
}

void MainWindow::moveAndCheck() {
    phone->moveBy(dXY->x() * speed, dXY->y() * speed);
    qsrand(time(NULL));

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

void MainWindow::resizeEvent(QResizeEvent *) {
    QRectF bounds = ui->graphicsView->scene()->itemsBoundingRect();
    ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
