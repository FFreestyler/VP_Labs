#ifndef INFOSTUD_H
#define INFOSTUD_H

#include <QWidget>
#include <QString>
#include <QSqlTableModel>

namespace Ui {
class infoStud;
}

class infoStud : public QWidget
{
    Q_OBJECT

public:
    explicit infoStud(QWidget *parent = 0);
    ~infoStud();

public slots:
    void recieveData(QString name, QString birth, QString group, QString mail, QString telephone, QString photo);

    void recieveDataProgress(QString os, QString math, QString prog, QString geom, QString sess);

    void recieveDataEvent(QVector<QString> inf);

private:
    Ui::infoStud *ui;
};

#endif // INFOSTUD_H
