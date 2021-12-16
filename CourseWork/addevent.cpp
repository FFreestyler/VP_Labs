#include "addevent.h"
#include "ui_addevent.h"

addEvent::addEvent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addEvent)
{
    ui->setupUi(this);
    eventDB = QSqlDatabase::addDatabase("QSQLITE", "Third");
    eventDB.setDatabaseName("./eventDB.db");

    if(eventDB.open()) qDebug("open eventDB");
    else qDebug("eventDB open error");

    eventQuery = new QSqlQuery(eventDB);
}

addEvent::~addEvent()
{
    delete ui;
}

void addEvent::on_acceptButton_clicked()
{
    QString nameEvent = ui->nameLineEdit->text();
    QString dateEvent = ui->dateEdit->text();

    eventQuery->prepare("INSERT INTO Event(Event_Name, Event_Date) VALUES(:nameEvent, :dateEvent);");
    eventQuery->bindValue(":nameEvent", nameEvent);
    eventQuery->bindValue(":dateEvent", dateEvent);

    eventQuery->exec();
    eventQuery->clear();

    QString FilePath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + nameEvent + "/";
    QDir dir;

    if(dir.mkpath(FilePath)) {
        qDebug("Folder Created");
    }
    if(dir.exists(FilePath)) {
        qDebug("Folder exists");
    }


    this->close();
}

void addEvent::on_cancelButton_clicked()
{
    this->close();
}
