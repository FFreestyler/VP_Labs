#ifndef ADDEVENT_H
#define ADDEVENT_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDir>

namespace Ui {
class addEvent;
}

class addEvent : public QWidget
{
    Q_OBJECT

public:
    explicit addEvent(QWidget *parent = 0);
    ~addEvent();

private slots:
    void on_pushButton_2_clicked();

    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::addEvent *ui;
    QSqlDatabase eventDB;
    QSqlQuery *eventQuery;
    QSqlTableModel *eventModel;
};

#endif // ADDEVENT_H
