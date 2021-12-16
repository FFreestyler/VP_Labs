#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myform = new Menchshikov_Ismailov();
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), myform, SLOT(show()));
    connect(this, SIGNAL(sendData(QString)), myform, SLOT(recieveData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(0,
                                                    "Выберите изображение",
                                                    QDir::currentPath(),
                                                    "*.png *.jpg *.gif *.jpeg");
        ui->label_5->setText(filename);
        QImage image(filename);
        ui->label_4->setPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_pushButton_2_clicked()
{
    QString st = ui->label_5->text() + "*"
            + ui->lineEdit->text() + "\n"
            + ui->lineEdit_2->text() + "\n"
            + ui->dateEdit->text();

    if(ui->radioButton->isChecked() == true) {
        st += "\nПол: Мужской";
    } else {
        st += "\nПол: Женский";
    }

    emit sendData(st);
}
