#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QStringList>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include "adddialogenum.h"
#include "headertableitem.h"
#include "messageboxfontwrapper.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

private:
    HeaderTableItem::Type type_;

public:
    explicit AddDialog(QWidget *parent = 0, QString title = QString("Dialog"), HeaderTableItem::Type type = HeaderTableItem::Type::Places);
    ~AddDialog();

    void setTask(QString task);
    QString getTask();

    void setDescription(QString description);
    QString getDescription();

    void setTime(QDateTime time);
    QDateTime getTime();

    void setImage(QString filename);
    QString getImage();

    int count;

    int getFieldCount();

    QMap<Field, QVariant> getFields();
    void setFields(QMap<Field, QVariant> data);

public slots:
    void checkAccepted();

private slots:
    void on_btnLoadImage_clicked();

private:
    QString comboBoxSavedStyle;
    void hideComboBoxArrow(bool state);

    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
