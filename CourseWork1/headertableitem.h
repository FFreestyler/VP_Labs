#ifndef HEADERTABLEITEM_H
#define HEADERTABLEITEM_H

#include <QWidget>
#include <QTabBar>
#include <QDebug>
#include <QFrame>
#include <QAbstractItemModel>
#include <QDialog>
#include <QModelIndex>
#include <QBoxLayout>
#include <QException>
#include <QSqlQuery>
#include <QException>
#include <QSqlRecord>
#include <QDateTime>
#include <QMap>
#include <QDate>
#include <QItemDelegate>
#include <QTableView>
#include "adddialogenum.h"

namespace Ui {
class HeaderTableItem;
}

class HeaderTableItem : public QWidget
{
    Q_OBJECT

public:
    enum class Type {
        None,
        Day,
        Education,
        Work,
        Personal,
        Places,
        VisitPlan,
        PersonalPlan
    };

    enum class WeekDay {
        None,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
        Any
    };

    enum class DayType {
        Education,
        Work,
        EducationAndWork,
        DayOff
    };

private:
    HeaderTableItem::Type type_ = HeaderTableItem::Type::None;
    HeaderTableItem::WeekDay day_ = HeaderTableItem::WeekDay::None;
    HeaderTableItem::DayType dayType_ = HeaderTableItem::DayType::DayOff;
    int weekNumber_ = 0;
    int yearNumber_ = 0;
    QDate date_;

public:
    explicit HeaderTableItem(QWidget *parent = 0);
    ~HeaderTableItem();

    void setTitle(QString title);
    QString getTitle();

    void setDay(QString title);
    QString getDay();

    void setType(HeaderTableItem::Type type);
    Type getType();

    void setTableModel(QAbstractItemModel* model);
    QAbstractItemModel* getTableModel();

    void setDayType(HeaderTableItem::DayType type);
    DayType getDayType();
    QString getDayTypeStr();
    void updateDayTypeInBase();

    void setDayNum(HeaderTableItem::WeekDay day);
    WeekDay getDayNum();
    QString getDayDBName();

    void setWeekNum(int week);
    int getWeekNum();

    void setYearNum(int year);
    int getYearNum();

    int getSelectedRowIndex();
    void setSelectedRow(int index);
    QMap<Field, QVariant> getRow(int index);

    void setDate(QDate date);
    QDate getDate();
    QStringList getAllTasks();
    QString getHtmlLayout();

    void addRow(QMap<Field, QVariant> data);
    void editRow(QMap<Field, QVariant> data, int row);
    void removeRow(int row);

    QFrame* getTitleFrame();
    QTableView* getTableView();

    void copySettings(HeaderTableItem* item);

    void updateModel();

    void columnVisible(QString columnName, bool state);
    void dayVisible(bool state);
    void configureVisible(bool state);

signals:
    void btnCofifguredClicked(HeaderTableItem* item);

private slots:
    void on_btnConfigure_clicked();

private:
    Ui::HeaderTableItem *ui;
};

#endif // HEADERTABLEITEM_H
