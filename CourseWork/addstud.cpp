#include "addstud.h"
#include "ui_addstud.h"

AddStud::AddStud(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddStud)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DB.db");
    if(db.open()) qDebug("open");
    else qDebug("open error");

    query = new QSqlQuery(db);
}

AddStud::~AddStud()
{
    delete ui;
}

void AddStud::on_addPhotoButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(0, "Open picture", QDir::currentPath(), "*.png, *.jpg");
    QPixmap pixmap(filename);
    ui->photoPathLabel->setText(filename);
    ui->photoLabel->setPixmap(pixmap);
}

void AddStud::on_acceptButton_clicked()
{
    if(ui->photoPathLabel->text() == "Photo path..."
            || ui->nameLineEdit->text() == ""
            || ui->classLineEdit->text() == ""
            || ui->mailLineEdit->text() == ""
            || ui->telephoneLineEdit->text() == "") {
        QMessageBox::warning(0, "Незаполненные поля", "Для продолжения все поля должны быть заполнены!");
        return;
    }

    QString name = ui->nameLineEdit->text();
    QString birth = ui->dateEdit->text();
    QString classdb = ui->classLineEdit->text();
    QString photo = ui->photoPathLabel->text();
    QString mail = ui->mailLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();

    query->prepare("INSERT INTO ALog(Name, BirthDate, Class, Photo, Mail, Telephone)"
                   "VALUES (:name, :birth, :classdb, :photo, :mail, :telephone);");
    query->bindValue(":name", name);
    query->bindValue(":birth", birth);
    query->bindValue(":classdb", classdb);
    query->bindValue(":photo", photo);
    query->bindValue(":mail", mail);
    query->bindValue(":telephone", telephone);

    if(!query->exec())
            qDebug() << "Unable to add in table:\n" << query->lastError();
    query->clear();

    this->close();
}

void AddStud::on_cancelButton_clicked()
{
    this->close();
}
