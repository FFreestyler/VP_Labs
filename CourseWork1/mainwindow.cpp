#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    affairsBase = QSqlDatabase::addDatabase("QSQLITE");
    affairsBase.setDatabaseName("./affairsBase.db");
    affairsBase.open();

    QSqlQuery query(affairsBase);
    query.exec("CREATE TABLE Учебные(Предмет TEXT, Задание TEXT, 'Срок сдачи' DATE);");
    query.exec("CREATE TABLE Рабочие(Задание TEXT, 'Срок сдачи' DATE);");
    query.exec("CREATE TABLE Личные(Задание TEXT);");
    query.exec("CREATE TABLE Места(Задание TEXT);");
    query.exec("CREATE TABLE Понедельник(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Вторник(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Среда(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Четверг(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Пятница(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Суббота(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Воскресенье(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE Любой(Время DATE, Задание TEXT, Описание TEXT, Год INT, Неделя INT);");
    query.exec("CREATE TABLE ТипДня(Тип INT, День DATE UNIQUE);");

    educationModel = new QSqlTableModel(this, affairsBase);
    educationModel->setTable("Учебные");
    educationModel->select();

    workModel = new QSqlTableModel(this, affairsBase);
    workModel->setTable("Рабочие");
    workModel->select();

    personalModel = new QSqlTableModel(this, affairsBase);
    personalModel->setTable("Личные");
    personalModel->select();

    placesModel = new QSqlTableModel(this, affairsBase);
    placesModel->setTable("Места");
    placesModel->select();

    ui->dayMonday->setTitle(tr("Monday"));
    ui->dayTuesday->setTitle(tr("Tuesday"));
    ui->dayWednesday->setTitle(tr("Wednesday"));
    ui->dayThursday->setTitle(tr("Thursday"));
    ui->dayFriday->setTitle(tr("Friday"));
    ui->daySaturday->setTitle(tr("Saturday"));
    ui->daySunday->setTitle(tr("Sunday"));
    ui->dayAny->setTitle(tr("Week"));

    ui->dayMonday->setDayNum(HeaderTableItem::WeekDay::Monday);
    ui->dayTuesday->setDayNum(HeaderTableItem::WeekDay::Tuesday);
    ui->dayWednesday->setDayNum(HeaderTableItem::WeekDay::Wednesday);
    ui->dayThursday->setDayNum(HeaderTableItem::WeekDay::Thursday);
    ui->dayFriday->setDayNum(HeaderTableItem::WeekDay::Friday);
    ui->daySaturday->setDayNum(HeaderTableItem::WeekDay::Saturday);
    ui->daySunday->setDayNum(HeaderTableItem::WeekDay::Sunday);
    ui->dayAny->setDayNum(HeaderTableItem::WeekDay::Any);

    QList<HeaderTableItem*> days {
        ui->dayMonday,
        ui->dayTuesday,
        ui->dayWednesday,
        ui->dayThursday,
        ui->dayFriday,
        ui->daySaturday,
        ui->daySunday,
        ui->dayAny
    };

    foreach (HeaderTableItem* item, days) {
        item->setType(HeaderTableItem::Type::Day);

        connect(item, SIGNAL(btnCofifguredClicked(HeaderTableItem*)), this, SLOT(onConfigurePageRequested(HeaderTableItem*)));

        QSqlTableModel* tempModel = new QSqlTableModel(this, affairsBase);
        tempModel->setTable(item->getDayDBName());
        item->setTableModel(tempModel);
        item->updateModel();

        QDate date = QDate::currentDate();
        date.setDate(date.year(), date.month(), item->getDay().toInt());

        QSqlQuery* dayTypeQuery = new QSqlQuery();
        dayTypeQuery->prepare("SELECT Тип FROM ТипДня WHERE День = ?");
        dayTypeQuery->bindValue(0, date);
        dayTypeQuery->exec();
        if (dayTypeQuery->next()) {
            item->setDayType(static_cast<HeaderTableItem::DayType>(dayTypeQuery->value(0).value<int>()));
        }
        dayTypeQuery->clear();

        delete dayTypeQuery;
    }

    ui->tableEducationAffairs->setTitle(tr("Education affairs"));
    ui->tableWorkAffairs->setTitle(tr("Work affairs"));
    ui->tablePersonalAffairs->setTitle(tr("Personal affairs"));
    ui->tablePlaces->setTitle(tr("Places"));

    ui->tableEducationAffairs->setType(HeaderTableItem::Type::Education);
    ui->tableWorkAffairs->setType(HeaderTableItem::Type::Work);
    ui->tablePersonalAffairs->setType(HeaderTableItem::Type::Personal);
    ui->tablePlaces->setType(HeaderTableItem::Type::Places);

    ui->tableEducationAffairs->setTableModel(educationModel);
    ui->tableWorkAffairs->setTableModel(workModel);
    ui->tablePersonalAffairs->setTableModel(personalModel);
    ui->tablePlaces->setTableModel(placesModel);

    QList<HeaderTableItem*> tables {
        ui->tableEducationAffairs,
        ui->tableWorkAffairs,
        ui->tablePersonalAffairs,
        ui->tablePlaces,
    };

    foreach (HeaderTableItem* item, tables) {
        connect(item, SIGNAL(btnCofifguredClicked(HeaderTableItem*)), this, SLOT(onConfigurePageRequested(HeaderTableItem*)));
    }

    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(onHelpRequested()));
    connect(ui->btnSelectWeek, SIGNAL(clicked(bool)), this, SLOT(onWeekChangeRequested()));
    connect(ui->btnWeekPrevious, &QPushButton::clicked, [this]() {setCurrentWeek(currentWeek.addDays(-7));});
    connect(ui->btnWeekNext, &QPushButton::clicked, [this]() {setCurrentWeek(currentWeek.addDays(+7));});

    ui->layoutHolder->addWidget(ui->twNavigation);

    setCurrentWeek(QDate::currentDate());

    ui->btnFormVisitPlan->setIcon(QIcon(":/images/visit_plan_placeholder.jpg"));
    ui->btnFormPersonalAffairs->setIcon(QIcon(":/images/personal_plan_placeholder.jpg"));

    visitPlanModel = new QStandardItemModel(this);
    visitPlanModel->setHorizontalHeaderLabels(QStringList() << "Место" << "Дата");

    personalGoalsModel = new QStandardItemModel(this);
    personalGoalsModel->setHorizontalHeaderLabels(QStringList() << "Цель" << "Изображение");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrentWeek(QDate date) {
    QDate weekStart = date;
    if (weekStart.dayOfWeek() != 1) {
        weekStart = weekStart.addDays(-weekStart.dayOfWeek() + 1);
    }
    currentWeek = weekStart;

    ui->lblChoosedYear->setText(QString::number(currentWeek.year()));
    ui->lblChoosedMonth->setText(currentWeek.toString("MMMM"));
    ui->lblChoosedWeek->setText(QString::number(currentWeek.weekNumber()));

    QList<HeaderTableItem*> days {
        ui->dayMonday,
        ui->dayTuesday,
        ui->dayWednesday,
        ui->dayThursday,
        ui->dayFriday,
        ui->daySaturday,
        ui->daySunday,
    };

    foreach (HeaderTableItem* item, days) {
        item->setDay(QString::number(currentWeek.addDays(static_cast<int>(item->getDayNum()) - 1).day()));
        item->setDate(currentWeek.addDays(static_cast<int>(item->getDayNum()) - 1));
        QSqlQuery* dayTypeQuery = new QSqlQuery();
        dayTypeQuery->prepare("SELECT Тип FROM ТипДня WHERE День = ?");
        dayTypeQuery->bindValue(0, currentWeek.addDays(static_cast<int>(item->getDayNum()) - 1));
        dayTypeQuery->exec();
        item->setYearNum(currentWeek.year());
        item->setWeekNum(currentWeek.weekNumber());
        if (dayTypeQuery->next()) {
            item->setDayType(static_cast<HeaderTableItem::DayType>(dayTypeQuery->value(0).value<int>()));
        }
        else {
            item->setDayType(HeaderTableItem::DayType::DayOff);
        }
        dayTypeQuery->clear();
        delete dayTypeQuery;
        item->updateModel();
    }
}

void MainWindow::onHelpRequested() {
    QProcess *process = new QProcess;
     QStringList args;
     args << QLatin1String("-collectionFile")
         << QLatin1String("helpDiary.qhc")
         << QLatin1String("-enableRemoteControl");
     process->start(QLatin1String("./assistant.exe"), args);
     if (!process->waitForStarted())
         return;
}

void MainWindow::onConfigurePageRequested(HeaderTableItem *item) {
    ui->twNavigation->setVisible(false);
    ui->frameWeekNavigation->setVisible(false);

    HeaderTableItemEditPage* editPage = new HeaderTableItemEditPage(this, item);
    connect(editPage, SIGNAL(btnBackClicked()), this, SLOT(onRestoreNavigationRequested()));
    savedTab = editPage;

    ui->layoutHolder->addWidget(editPage);
}

void MainWindow::onRestoreNavigationRequested() {
    ui->twNavigation->setVisible(true);
    ui->frameWeekNavigation->setVisible(true);
    ui->layoutHolder->removeWidget(savedTab);

    delete savedTab;
}

void MainWindow::onWeekChangeRequested() {
    if (ui->btnSelectWeek->isChecked()) {
        CalendarPopup* calendar = new CalendarPopup(this, currentWeek);
        calendar->move(this->mapToParent(ui->frameWeekChoose->mapToParent(ui->btnSelectWeek->pos())) + QPoint(-calendar->width()/2, 65));
        calendar->exec();

        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(100);
        connect(timer, &QTimer::timeout, [this, timer]() {
            ui->btnSelectWeek->setChecked(false);
            timer->deleteLater();
        });
        timer->start();

        if(calendar->result() == QDialog::Accepted) {
            setCurrentWeek(calendar->selectedDate());
        }
    }
}

void MainWindow::on_btnFormVisitPlan_clicked()
{
    HeaderTableItem* item = new HeaderTableItem(this);
    item->setTitle(tr("Visit plan"));
    item->setTableModel(visitPlanModel);
    item->setType(HeaderTableItem::Type::VisitPlan);

    connect(item, &HeaderTableItem::btnCofifguredClicked, this, &MainWindow::onConfigurePageRequested);
    item->btnCofifguredClicked(item);
}

void MainWindow::on_btnFormPersonalAffairs_clicked()
{
    HeaderTableItem* item = new HeaderTableItem(this);
    item->setTitle(tr("Personal goals"));
    item->setTableModel(personalGoalsModel);
    item->setType(HeaderTableItem::Type::PersonalPlan);

    connect(item, &HeaderTableItem::btnCofifguredClicked, this, &MainWindow::onConfigurePageRequested);
    item->btnCofifguredClicked(item);
}
