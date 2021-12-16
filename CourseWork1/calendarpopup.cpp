#include "calendarpopup.h"
#include "ui_calendarpopup.h"

CalendarPopup::CalendarPopup(QWidget *parent, QDate date) :
    QDialog(parent, Qt::Popup),
    ui(new Ui::CalendarPopup)
{
    ui->setupUi(this);

    ui->calendarWidget->setSelectedDate(date);

    connect(ui->btnToday, &QPushButton::clicked, [this]() {ui->calendarWidget->setSelectedDate(QDate::currentDate()); accept();});
}

CalendarPopup::~CalendarPopup()
{
    delete ui;
}

QDate CalendarPopup::selectedDate() const{
    return ui->calendarWidget->selectedDate();
}
