#ifndef CALENDARPOPUP_H
#define CALENDARPOPUP_H

#include <QDialog>
#include <QDate>

namespace Ui {
class CalendarPopup;
}

class CalendarPopup : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarPopup(QWidget *parent = 0, QDate date = QDate::currentDate());
    ~CalendarPopup();

    QDate selectedDate() const;

private:
    Ui::CalendarPopup *ui;
};

#endif // CALENDARPOPUP_H
