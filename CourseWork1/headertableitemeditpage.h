#ifndef HEADERTABLEITEMEDITPAGE_H
#define HEADERTABLEITEMEDITPAGE_H

#include "headertableitem.h"
#include <tuple>
#include <QWidget>
#include <QDialog>
#include <QSqlTableModel>
#include <QSqlError>
#include <QFont>
#include <QDateTime>
#include <QComboBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QMap>
#include "widecombobox.h"
#include "pdfhelper.h"

namespace Ui {
class HeaderTableItemEditPage;
}

class HeaderTableItemEditPage : public QWidget
{
    Q_OBJECT

    HeaderTableItem* item_;
    WideComboBox* cmbDayType;

public:
    explicit HeaderTableItemEditPage(QWidget *parent = 0, HeaderTableItem* item = nullptr);
    ~HeaderTableItemEditPage();

signals:
    void btnBackClicked();

private slots:
    void on_btnBack_clicked();

    void on_btnSave_clicked();

    void on_dayTypeSelected(int index);

    void on_btnAdd_clicked();

    void on_btnEdit_clicked();

    void on_btnRemove_clicked();

private:
    Ui::HeaderTableItemEditPage *ui;
};

#endif // HEADERTABLEITEMEDITPAGE_H
