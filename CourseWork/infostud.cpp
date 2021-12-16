#include "infostud.h"
#include "ui_infostud.h"
#include <QDebug>

infoStud::infoStud(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::infoStud)
{
    ui->setupUi(this);
}

infoStud::~infoStud()
{
    delete ui;
}

void infoStud::recieveData(QString name, QString birth, QString group, QString mail, QString telephone, QString photo) {
    QString r_name = name;
    QString r_birth = birth;
    QString r_group = group;
    QString r_mail = mail;
    QString r_telephone = telephone;
    QPixmap pixmap(photo);

    ui->nameLabel->setText(r_name);
    ui->birthLabel->setText(r_birth);
    ui->groupLabel->setText(r_group);
    ui->mailLabel->setText(r_mail);
    ui->telephoneLabel->setText(r_telephone);
    ui->photoLabel->setPixmap(pixmap);
}

void infoStud::recieveDataProgress(QString os, QString math, QString prog, QString geom, QString sess) {
    ui->OSMark->setText(os);
    ui->MathMark->setText(math);
    ui->ProgMark->setText(prog);
    ui->GeomMark->setText(geom);
    ui->SessMark->setText(sess);
}

void infoStud::recieveDataEvent(QVector<QString> inf) {
    ui->listWidget->clear();
    QList<QString> texts;
    for(int i = 0; i < inf.size(); i++) {
        texts.append(inf[i]);
    }
    qDebug() << texts;
    ui->listWidget->addItems(texts);
}
