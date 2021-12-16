#include "menchshikov_ismailov.h"
#include "ui_menchshikov_ismailov.h"
#include <QTextDocumentWriter>
#include <QFileDialog>
#include <QMessageBox>


Menchshikov_Ismailov::Menchshikov_Ismailov(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menchshikov_Ismailov)
{
    ui->setupUi(this);
}


Menchshikov_Ismailov::~Menchshikov_Ismailov()
{
    delete ui;
}

void Menchshikov_Ismailov::recieveData(QString str) {
    QStringList lst = str.split("*");
    ui->textEdit->setText(lst.at(1) + "\n" + lst.at(0));
    if(lst.size() > 1) {
        QImage image(lst.at(0));
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
}

void Menchshikov_Ismailov::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Reset") {
            ui->textEdit->clear();
            ui->label->clear();
        }
    else if (button->text() == "Save") {
        QString fileName = QFileDialog::getSaveFileName(0, "Сохранить файл", QDir::currentPath(), "*.txt");
        QTextDocumentWriter writer;
        writer.setFileName(fileName);
        writer.write(ui->textEdit->document());
    }
    else if (button->text() == "Open") {
        QString fileName = QFileDialog::getOpenFileName(0, "Открыть файл", QDir::currentPath(), "*.txt");
                QFile file(fileName);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui->textEdit->setPlainText(file.readAll());
                    QStringList lst = ui->textEdit->toPlainText().split("\n");
                    QImage photo(lst.at(4));
                    if (photo.isNull()){
                        return;
                    }
                    QPixmap photoPixmap(QPixmap::fromImage(photo).scaled(ui->label->width(), ui->label->maximumHeight(), Qt::KeepAspectRatio));
                    ui->label->setPixmap(photoPixmap);
                    ui->label->resize(photoPixmap.width(), photoPixmap.height());
                }
    }
}
