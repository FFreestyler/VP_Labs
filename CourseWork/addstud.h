#ifndef ADDSTUD_H
#define ADDSTUD_H

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
#include <QSqlError>

namespace Ui {
class AddStud;
}

class AddStud : public QWidget
{
    Q_OBJECT

public:
    explicit AddStud(QWidget *parent = 0);
    ~AddStud();

private slots:
    void on_addPhotoButton_clicked();

    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddStud *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlQuery *query;
};

#endif // ADDSTUD_H
