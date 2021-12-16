#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <addstud.h>
#include <addevent.h>
#include <itemdelegate.h>
#include <infostud.h>
#include <windows.h>
#include <QProcess>
//#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //STUDENT TABLE
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DB.db");
    if(db.open()) qDebug("open");
    else qDebug("open error");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE ALog(Name TEXT, BirthDate DATE, Class TEXT, Photo TEXT, Mail TEXT, Telephone TEXT);");

    model = new QSqlTableModel(this, db);
    model->setTable("ALog");
    model->select();

    ui->DataBase->setModel(model);


    //EVENT TABLE
    eventDB = QSqlDatabase::addDatabase("QSQLITE", "Second");
    eventDB.setDatabaseName("./eventDB.db");
    if(eventDB.open()) qDebug("open eventDB");
    else qDebug("eventDB open error");

    eventQuery = new QSqlQuery(eventDB);
    eventQuery->exec("CREATE TABLE Event(Event_name TEXT, Event_Date DATE);");

    eventModel = new QSqlTableModel(this, eventDB);
    eventModel->setTable("Event");
    eventModel->select();

    updateGroupStudentComboBox();

    ui->eventDataBase->setModel(eventModel);
    updateEventStudentComboBox();

    checkProgressFile();

    updateGroupProgressComboBox();

    updateGroupHeadmanComboBox();

    updateStudentsHeadmanComboBox();
    createHeadmanTableView();
    checkHeadmanTable();


    ItemDelegate *itDelegate = new  ItemDelegate;
    ui->DataBase->setItemDelegateForColumn(1, itDelegate);

    connect(this,
            SIGNAL(sendData(QString,QString,QString,QString,QString,QString)),
            infoStudent,
            SLOT(recieveData(QString,QString,QString,QString,QString, QString)));

    connect(this,
            SIGNAL(sendDataProgress(QString,QString,QString,QString,QString)),
            infoStudent,
            SLOT(recieveDataProgress(QString,QString,QString,QString,QString)));

    connect(this,
            SIGNAL(sendDataEvent(QVector<QString>)),
            infoStudent,
            SLOT(recieveDataEvent(QVector<QString>)));

    //ui->DataBase->hideColumn(3);
    createAllStudentsFolder();
    findEqual();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findEqual() {
    QString folderName;
    QString studentInfoName;
    QString studentInfoGroup;
    QVector<QString> information;
    studentInfoName = model->index(row, 0).data().toString();
    studentInfoGroup = model->index(row, 2).data().toString();
    for(int j = 0; j < eventModel->rowCount(); j++) {
        folderName = eventModel->index(j, 0).data().toString();
        QString infoEventStudents = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + folderName + "/eventStudent.txt";
        QFile file(infoEventStudents);
        if(file.open(QIODevice::ReadOnly)) {
            while(!file.atEnd()) {
                QString line = file.readLine();
                QString equal = studentInfoName + " " + studentInfoGroup + "\n";
                if(line == equal) {
                    information.push_back(folderName);
                }
            }
        }
    }
    emit sendDataEvent(information);
}

void MainWindow::onButtonSendData() {
    for(int i = 0; i < model->rowCount(); i++) {
        if(ui->infoNameLabel->text() == model->index(i, 0).data().toString()) {
            emit sendData(ui->infoNameLabel->text(), ui->infoBirthLabel->text(), ui->infoClassLabel->text(),
                      ui->infoMailLabel->text(), ui->infoTelephoneLabel->text(), model->index(i, 3).data().toString());
        }
    }
}

void MainWindow::onButtonSendProgress() {

    for(int i = 0; i < progressModel->rowCount(); i++) {
        QString OS = progressModel->index(i, 2).data().toString();
        QString Math = progressModel->index(i, 3).data().toString();
        QString Prog = progressModel->index(i, 4).data().toString();
        QString Geom = progressModel->index(i, 5).data().toString();
        QString Sess = progressModel->index(i, 6).data().toString();
        if(ui->infoNameLabel->text() == progressModel->index(i,0).data().toString()) {
            emit sendDataProgress(OS, Math, Prog, Geom, Sess);
        }
    }
}
void MainWindow::on_pushButton_9_clicked()
{
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(onButtonSendData()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(onButtonSendProgress()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(findEqual()));
    infoStudent->show();
}


void MainWindow::on_addStud_clicked()
{
    AddStud *addStud = new AddStud();
    addStud->show();

    if(!addStud->isEnabled()) {
        model = new QSqlTableModel(this, db);
        model->setTable("ALog");
        model->select();

        ui->DataBase->setModel(model);

        createStudentFolder();
        updateEventStudentComboBox();
    }
}

void MainWindow::updateGroupStudentComboBox()
{
    QVector<QString> elem;
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        uniqueGroup.insert(model->index(i,2).data().toString());
    }

    foreach (const QString &value, uniqueGroup) {
        elem.push_back(value);
    }

    for(int i = 0; i < elem.size(); i++) {
        ui->groupStudentComboBox->addItem(elem[i]);
    }
}

void MainWindow::on_groupSearchPB_clicked()
{
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        ui->DataBase->setRowHidden(i, false);
    }
    QString pickGroup = ui->groupStudentComboBox->currentText();

    for(int i = 0; i < size; i++) {
        QString group = model->index(i,2).data().toString();
        group = group.trimmed();
        if(group != pickGroup) {
            ui->DataBase->setRowHidden(i, true);
        }
    }
}

void MainWindow::on_nameSearchPB_clicked()
{
    int size = model->rowCount();
    QString name = ui->nameStudentLineEdit->text();

    for(int i = 0; i < size; i++) {
        QString nameInTable = model->index(i,0).data().toString();
        //qDebug() << nameInTable.indexOf(name) << ;
        if(nameInTable.indexOf(name)) {
            ui->DataBase->setRowHidden(i, true);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    model = new QSqlTableModel(this, db);
    model->setTable("ALog");
    model->select();

    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        ui->DataBase->setRowHidden(i, false);
    }

    //qDebug() << model->columnCount();

    ui->DataBase->setModel(model);

    createStudentFolder();
    updateEventStudentComboBox();
}

void MainWindow::on_DataBase_clicked(const QModelIndex &index)
{
    row = index.row();
    QString name = model->index(row, 0).data().toString();
    QString birth = model->index(row, 1).data().toString();
    QString classdb = model->index(row, 2).data().toString();
    QString photo = model->index(row, 3).data().toString();
    QString mail = model->index(row, 4).data().toString();
    QString telephone = model->index(row, 5).data().toString();
    QPixmap pixmap(photo);
    ui->infoNameLabel->setText(name);
    ui->infoBirthLabel->setText(birth);
    ui->infoClassLabel->setText(classdb);
    ui->infoPhotoLabel->setPixmap(pixmap);
    ui->infoMailLabel->setText(mail);
    ui->infoTelephoneLabel->setText(telephone);
}

void MainWindow::on_delStud_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Удалить студента", "Вы действительно хотите удалить студента из базы данных?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        model->removeRow(row);

        model = new QSqlTableModel(this, db);
        model->setTable("ALog");
        model->select();

        ui->DataBase->setModel(model);

        createStudentFolder();
        updateEventStudentComboBox();
    }
}

void MainWindow::on_addEvent_clicked()
{
    addEvent *event = new addEvent();
    event->show();
}

void MainWindow::on_eventDataBase_clicked(const QModelIndex &index)
{
    eventRow = index.row();
    QString nameEvent = eventModel->index(eventRow,0).data().toString();
    QString dateEvent = eventModel->index(eventRow,1).data().toString();

    ui->nameEventLabel->setText(nameEvent);
    ui->dateEventLabel->setText(dateEvent);

    images.clear();
    ui->eventPhotoLabel->clear();
    QString infoPhotoPathForLabel = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/"
            + nameEvent + "/images.txt";

    QFile file(infoPhotoPathForLabel);

    if(file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!file.atEnd()) {
            QString str = file.readLine();
            str.remove(str.length()-1,str.length());
            images.push_back(str);
        }
    } else {
        qDebug() << "File not open";
    }
    if(images.isEmpty()) {
        ui->eventPhotoLabel->clear();
        ui->eventPhotoLabel->setText("Photo absent");
    } else {
        QPixmap eventPixmap(images[0]);
        ui->eventPhotoLabel->setPixmap(eventPixmap);
    }

    ui->eventStudentListWidget->clear();
    ui->eventStudentListWidget->addItem(nameEvent + ":");

    QList<QString> texts;
    QString infoEventStudents = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/"
            + nameEvent + "/eventStudent.txt";
    QFile fileSecond(infoEventStudents);
    if(fileSecond.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!fileSecond.atEnd()) {
            QString str = fileSecond.readLine();
            str.remove(str.length()-1,str.length());
            texts.push_back(str);
        }
    } else {
        qDebug() << "File not open!";
    }

    for(int i = 1; i < texts.size(); i++) {
        ui->eventStudentListWidget->addItem(texts[i]);
    }


}

void MainWindow::on_pushButton_2_clicked()
{
    eventModel = new QSqlTableModel(this, eventDB);
    eventModel->setTable("Event");
    eventModel->select();

    ui->eventDataBase->setModel(eventModel);
}

int removeFolder(QDir & dir)
{
  int res = 0;
  //Получаем список каталогов
  QStringList lstDirs = dir.entryList(QDir::Dirs |
                  QDir::AllDirs |
                  QDir::NoDotAndDotDot);
  //Получаем список файлов
  QStringList lstFiles = dir.entryList(QDir::Files);

  //Удаляем файлы
  foreach (QString entry, lstFiles)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
   QFile::remove(entryAbsPath);
  }

  //Для папок делаем рекурсивный вызов
  foreach (QString entry, lstDirs)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QDir dr(entryAbsPath);
   removeFolder(dr);
  }

  //Удаляем обрабатываемую папку
  if (!QDir().rmdir(dir.absolutePath()))
  {
    res = 1;
  }
  return res;
}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Удалить мероприятие", "Вы действительно хотите удалить мероприятие?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QString folderName = eventModel->index(eventRow, 0).data().toString();
        QDir dir;

        dir = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + folderName + "/";
        removeFolder(dir);

        eventModel->removeRow(eventRow);
    }
}

void MainWindow::on_addPhotoButton_clicked()
{
    QString folderName = eventModel->index(eventRow,0).data().toString();
    QString infoPhotoPath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + folderName + "/images.txt";

    QFile file(infoPhotoPath);
    QString pathToPhoto = QFileDialog::getOpenFileName(0, "Open image", QDir::currentPath(), "*.png *.jpg");

    if(file.open(QIODevice::Append)) {
        QTextStream out(&file);
        out << pathToPhoto;
        out << "\n";
    }

    file.close();
}

void MainWindow::on_deletePhotoButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Удалить фото", "Вы действительно хотите удалить фото с мероприятия?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QString folderName = eventModel->index(eventRow, 0).data().toString();
        QString infoPhotoPath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + folderName + "/images.txt";

        QFile file(infoPhotoPath);
        QStringList lines;
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream edit(&file);
            while(!edit.atEnd()) {
                lines.push_back(edit.readLine());
            }
            file.close();
        }
        qDebug() << counter << "  " << lines.length();
        if(lines.length() > 0) {
            lines.removeAt(counter);
        }
        qDebug() << lines.length();
        QFile outFile(infoPhotoPath);
        if(outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream edit(&outFile);
            for(int i = 0; i < lines.size(); i++) {
                edit << lines[i] << endl;
            }
            outFile.close();
        }
    }
}

void MainWindow::on_prevPhotoButton_clicked()
{
    int maxSize = images.size();
    if(counter == 0) {
        counter = maxSize - 1;
        QPixmap currentPhoto(images[counter]);
        ui->eventPhotoLabel->clear();
        ui->eventPhotoLabel->setPixmap(currentPhoto);
    } else {
        counter--;
        QPixmap currentPhoto(images[counter]);
        ui->eventPhotoLabel->clear();
        ui->eventPhotoLabel->setPixmap(currentPhoto);
    }
}

void MainWindow::on_nextPhotoButton_clicked()
{
    int maxSize = images.size();
    if(counter<maxSize-1) {
        counter++;
        QPixmap currentPhoto(images[counter]);
        ui->eventPhotoLabel->clear();
        ui->eventPhotoLabel->setPixmap(currentPhoto);
    } else {
        counter = 0;
        QPixmap currentPhoto(images[counter]);
        ui->eventPhotoLabel->clear();
        ui->eventPhotoLabel->setPixmap(currentPhoto);
    }
}

void MainWindow::updateEventStudentComboBox()
{
    ui->eventStudentComboBox->clear();
    int rowStudentCount = model->rowCount();
    ui->eventStudentComboBox->addItem("");
    for(int i = 0; i < rowStudentCount; i++) {
        ui->eventStudentComboBox->addItem(model->index(i,0).data().toString() + " " + model->index(i,2).data().toString());
    }
}

void MainWindow::createStudentFolder()
{
    QString folderPath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/AllStudents/";
    QDir dir;
    if(dir.mkpath(folderPath)) {
        qDebug("folder create");
    } else {
        qDebug("folder exists");
    }

    folderPath += "studentsInfo.txt";
    QFile studentsInfoFile(folderPath);
    QTextStream ts(&studentsInfoFile);

    if(studentsInfoFile.open(QIODevice::WriteOnly)) {
        for(int i = 0; i < model->rowCount(); i++) {
            QString fullName = model->index(i, 0).data().toString()
                    + " "
                    + model->index(i, 2).data().toString()
                    + "\n";
            ts << fullName;
        }
        studentsInfoFile.close();
    }
}

void MainWindow::createAllStudentsFolder() {
    /*QString fileName = " ";
    QString fileGroup = " ";
    for(int i = 0; i < model->rowCount(); i++) {
        QString folderPath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/AllStudentsInfo/";
        QDir dir;
        dir.mkpath(folderPath);
        fileName = model->index(i, 0).data().toString();
        fileGroup = model->index(i, 2).data().toString();
        folderPath += fileName + " " + fileGroup + ".txt";
        fileGroup = " ";
        fileName = " ";
    }*/
}

void MainWindow::on_eventStudentComboBox_currentIndexChanged(int index)
{
    QString student = ui->eventStudentComboBox->currentText();
    ui->eventStudentListWidget->addItem(student);

}

void MainWindow::on_pushButton_3_clicked()
{
    QList<QString> texts;
    QList<QListWidgetItem*> items = ui->eventStudentListWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    foreach(QListWidgetItem *item, items)
        texts.append(item->text());

    QString folderName = eventModel->index(eventRow,0).data().toString();
    QString filePath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/EventPhoto/" + folderName + "/";
    filePath += "eventStudent.txt";
    QFile file(filePath);

    QTextStream ts(&file);
    ts.setCodec("UTF-8");

    if(file.open(QIODevice::WriteOnly)) {
        for(int i = 0; i < texts.size(); i++) {
            ts << texts[i] + "\n";
        }
        file.close();
    }
}

void MainWindow::on_eventStudentListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QList<QListWidgetItem*> items = ui->eventStudentListWidget->selectedItems();
    foreach (item, items) {
        delete ui->eventStudentListWidget->takeItem(ui->eventStudentListWidget->row(item));
    }
}

void MainWindow::createProgressTableView() {
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        studentName.push_back(model->index(i, 0).data().toString());
        studentGroup.push_back(model->index(i,2).data().toString());
    }

    columnName << "FullName" << "Group" << "OS" << "Math" << "Programming" << "Geometry" << "Session";

    progressModel = new QStandardItemModel(size, 7);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < 7; j++) {
            QStandardItem *item = new QStandardItem(QString("").arg(i).arg(j));
            progressModel->setItem(i, j, item);
        }
    }

    ui->progressTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->progressTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    for(int i = 0; i < 7; i++) {
        QStandardItem* newColumn = new QStandardItem(QString(columnName[i]));
        progressModel->setHorizontalHeaderItem(i, newColumn);
    }

    ui->progressTableView->setModel(progressModel);
}

void MainWindow::saveProgressTable() {
    QString textData;
    int row = model->rowCount();
    //int column = model->columnCount();

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < 7; j++) {
            textData += progressModel->data(progressModel->index(i,j)).toString();
            textData += ";";
        }
        textData[textData.size()-1] = '\n';
    }

    QString folderPath = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/Progress/progress.csv";
    QDir dir;
    QFile csvFile(folderPath);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
}

void MainWindow::fillProgressTable() {
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        QStandardItem* nameElement = new QStandardItem(QString(studentName[i]));
        QStandardItem* groupElement = new QStandardItem(QString(studentGroup[i]));
        progressModel->setItem(i, 0, nameElement);
        progressModel->setItem(i, 1, groupElement);
    }
}

void MainWindow::readProgressTable() {
    QFile file("C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/Progress/progress.csv");
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "ErrorCSV";
    } else {
        QTextStream ts(&file);
        progressModel->clear();
        for(int i = 0; i < 7; i++) {
            QStandardItem* newColumn = new QStandardItem(QString(columnName[i]));
            progressModel->setHorizontalHeaderItem(i, newColumn);
        }
        while(!ts.atEnd()) {
            QString line = ts.readLine();
            QList<QStandardItem*> standartItemList;
            for(QString item : line.split(";")) {
                standartItemList.append(new QStandardItem(item));
            }
            progressModel->insertRow(progressModel->rowCount(), standartItemList);
        }
        file.close();
    }
}

void MainWindow::checkProgressFile() {
    QFile progressFile("C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/Progress/progress.csv");
    if(progressFile.open(QFile::ReadOnly | QFile::Text)) {
        createProgressTableView();
        readProgressTable();
    } else {
        createProgressTableView();
        fillProgressTable();
    }
}

void MainWindow::on_saveProgressButton_clicked()
{
    saveProgressTable();
}

void MainWindow::setSessionProgress() {
    int size = progressModel->rowCount();

    for(int i = 0; i < size; i++) {
        double result = 0.0;
        int cnt = 0;
        for(int j = 2; j < 6; j++) {
            result+=progressModel->index(i, j).data().toDouble();
            if(progressModel->index(i,j).data().toInt() == 2) {
                cnt++;
            }
        }
        //qDebug() << result/4 << "\n";
        QString mark = "";
        if(cnt > 0) {
            mark = "2";
        } else {
            double RRes = result/4;
            mark = QString::number(RRes);
        }
        QStandardItem *markItem = new QStandardItem(mark);
        progressModel->setItem(i,6,markItem);
    }

}

void MainWindow::on_sessionResultProgressButton_clicked()
{
    setSessionProgress();
}

void MainWindow::on_unsuccessfulProgressButton_clicked()
{
    int size = progressModel->rowCount();

    for(int i = 0; i < size; i++) {
        if(progressModel->index(i,6).data().toInt() != 2) {
            ui->progressTableView->setRowHidden(i, true);
        }
    }
}

void MainWindow::on_resetProgressButton_clicked()
{
    int size = progressModel->rowCount();
    for(int i = 0; i < size; i++) {
        ui->progressTableView->setRowHidden(i, false);
    }
}

void MainWindow::on_createReportProgressButton_clicked()
{
    QString filenamePDF = QFileDialog::getSaveFileName(this, "Save report", QDir::currentPath(), "*.pdf");
    int size = progressModel->rowCount();
    if(filenamePDF != "") {
        QPdfWriter pdf(filenamePDF);
        QPainter painter(&pdf);
        painter.drawText(20,50,QString("Unsuccessful report"));
        for(int i = 0, x = 20, y = 500; i< size; i++) {
            int mark = progressModel->index(i, 6).data().toInt();
            if(mark == 2) {
                QString name = "Name: " + progressModel->index(i, 0).data().toString();
                QString group = "\tGroup: " + progressModel->index(i, 1).data().toString();
                painter.drawText(x,y,name + group);
                y+= 150;
            }
        }
    }
}

void MainWindow::updateGroupProgressComboBox()
{
    QVector<QString> elem;
    int size = progressModel->rowCount();
    for(int i = 0; i < size; i++) {
        uniqueGroup.insert(model->index(i,2).data().toString());
    }

    foreach (const QString &value, uniqueGroup) {
        elem.push_back(value);
    }

    for(int i = 0; i < elem.size(); i++) {
        ui->groupComboBox->addItem(elem[i]);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    on_resetProgressButton_clicked();
    int size = progressModel->rowCount();
    QString pickGroup = ui->groupComboBox->currentText();

    for(int i = 0; i < size; i++) {
        QString group = progressModel->index(i,1).data().toString();
        group = group.trimmed();
        if(group != pickGroup) {
            ui->progressTableView->setRowHidden(i, true);
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    int size = progressModel->rowCount();
    QString name = ui->nameLineEdit->text();

    for(int i = 0; i < size; i++) {
        QString nameInTable = progressModel->index(i,0).data().toString();
        //qDebug() << nameInTable.indexOf(name) << ;
        if(nameInTable.indexOf(name)) {
            ui->progressTableView->setRowHidden(i, true);
        }
    }
}

void MainWindow::updateGroupHeadmanComboBox() {
    QVector<QString> elem;
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        uniqueGroup.insert(model->index(i,2).data().toString());
    }

    foreach (const QString &value, uniqueGroup) {
        elem.push_back(value);
    }

    for(int i = 0; i < elem.size(); i++) {
        ui->groupHeadmanComboBox->addItem(elem[i]);
    }
}

void MainWindow::updateStudentsHeadmanComboBox() {
    ui->studentHeadmanComboBox->clear();
    QVector<QString>students;

    QString currentGroup = ui->groupHeadmanComboBox->currentText();

    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        if(model->index(i,2).data().toString() == currentGroup) {
            students.push_back(model->index(i, 0).data().toString());
        }
    }

    for(int i = 0; i < students.size(); i++) {
        ui->studentHeadmanComboBox->addItem(students[i]);
    }
}

void MainWindow::on_groupHeadmanComboBox_currentTextChanged(const QString &arg1)
{
    ui->studentHeadmanComboBox->clear();
    QVector<QString>students;
    QString currentGroup = arg1;
    int size = model->rowCount();
    for(int i = 0; i < size; i++) {
        if(model->index(i, 2).data().toString() == currentGroup) {
            students.push_back(model->index(i, 0).data().toString());
        }
    }

    for(int i = 0; i < students.size();i++) {
        ui->studentHeadmanComboBox->addItem(students[i]);
    }
}

void MainWindow::createHeadmanTableView() {
    headmanColName << "FullName" << "Group";

    headmanModel = new QStandardItemModel;
    for(int i = 0; i < 2; i++) {
        QStandardItem* newColumn = new QStandardItem(QString(headmanColName[i]));
        headmanModel->setHorizontalHeaderItem(i, newColumn);
    }

    ui->headmanTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->headmanTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->headmanTableView->setModel(headmanModel);
}

void MainWindow::on_pushButton_7_clicked()
{
    QList<QStandardItem*> itemList;

    QStandardItem* nameHeadman = new QStandardItem(ui->studentHeadmanComboBox->currentText());
    QStandardItem* groupHeadman = new QStandardItem(ui->groupHeadmanComboBox->currentText());

    itemList.push_back(nameHeadman);
    itemList.push_back(groupHeadman);

    headmanModel->insertRow(headmanModel->rowCount(), itemList);
    saveHeadmanTable();
    headmanModel->sort(1, Qt::AscendingOrder);
}

void MainWindow::on_headmanTableView_clicked(const QModelIndex &index)
{
    headmanRow = index.row();
    QString name = "";
    QString birth = "";
    QString group = "";
    QString photo = "";
    QString mail = "";
    QString telephone = "";
    for(int i = 0; i < model->rowCount(); i++) {
        if(headmanModel->index(headmanRow, 0).data().toString() == model->index(i, 0).data().toString()) {
            name = model->index(i,0).data().toString();
            birth = model->index(i, 1).data().toString();
            group = model->index(i, 2).data().toString();
            photo = model->index(i, 3).data().toString();
            mail = model->index(i, 4).data().toString();
            telephone = model->index(i,5).data().toString();
        }
    }
    QPixmap headmanPhoto(photo);
    ui->nameHeadmanLabel->setText(name);
    ui->birthHeadmanLabel->setText(birth);
    ui->groupHeadmanLabel->setText(group);
    ui->mailHeadmanLabel->setText(mail);
    ui->telephoneHeadmanLabel->setText(telephone);
    ui->headmanPhotoLabel->setPixmap(headmanPhoto);
}

void MainWindow::on_pushButton_8_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Удалить старосту", "Вы действительно хотите удалить старосту из списка?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        headmanModel->removeRow(headmanRow);
        ui->headmanTableView->setModel(headmanModel);
        saveHeadmanTable();
    }
}

void MainWindow::saveHeadmanTable() {
    QString textData;
    int rows = headmanModel->rowCount();
    int col = headmanModel->columnCount();

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < col; j++) {
            textData += headmanModel->data(headmanModel->index(i, j)).toString();
            textData += ";";
        }
        textData += "\n";
    }

    QString currentPathH = "C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/headmanTable/";
    currentPathH += "headmanTable.csv";
    QFile csvFile(currentPathH);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;
    }
    csvFile.close();
}

void MainWindow::readHeadmanTable() {
    QFile headmanFile("C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/headmanTable/headmanTable.csv");
    if ( !headmanFile.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&headmanFile);
        headmanModel->clear();
        for(int i = 0; i < 2; i++) {
            QStandardItem* newColumn = new QStandardItem(QString(headmanColName[i]));
            headmanModel->setHorizontalHeaderItem(i, newColumn);
        }
        while(!in.atEnd()) {
            QString line = in.readLine();
            QList<QStandardItem *> itemList;

            for(QString item : line.split(";")) {
                itemList.append(new QStandardItem(item));
            }
            headmanModel->insertRow(headmanModel->rowCount(), itemList);
        }
        headmanFile.close();
    }
}

void MainWindow::checkHeadmanTable() {
    QFile headmanFile("C:/Users/freee/OneDrive/Desktop/SibSUTIS/VP_Labs/CourseWork/source/headmanTable/headmanTable.csv");
    if(headmanFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Headman file reading...";
        createHeadmanTableView();
        readHeadmanTable();
    } else {
        qDebug() << "File not exists...";
        createHeadmanTableView();
    }
}

void MainWindow::on_progressTableView_activated(const QModelIndex &index)
{
    int row = index.row();
    for(int i = 2; i < progressModel->columnCount() - 1; i++) {
        if(progressModel->index(row, i).data().toString() > "5") {
            QString mark = "5";
            QStandardItem *markItem = new QStandardItem(mark);
            progressModel->setItem(row, i, markItem);
        } else if(progressModel->index(row, i).data().toString() < "2") {
            QString mark = "2";
            QStandardItem *markItem = new QStandardItem(mark);
            progressModel->setItem(row, i, markItem);
        }
    }
    setSessionProgress();
}

void MainWindow::closeEvent(QCloseEvent* ) {
     QMessageBox::StandardButton reply = QMessageBox::question(this, "Сохранить изменения?", "Хотите ли вы сохранить изменения?", QMessageBox::Yes | QMessageBox::No);
     if(reply == QMessageBox::Yes) {
         on_pushButton_3_clicked();
         saveProgressTable();
     } else {
         this->close();
     }
}

void MainWindow::on_action_triggered()
{
    QProcess *process = new QProcess;
     QStringList args;
     args << QLatin1String("-collectionFile")
         << QLatin1String("collection.qhc")
         << QLatin1String("-enableRemoteControl");
     process->start(QLatin1String("assistant"), args);
     if (!process->waitForStarted())
         return;
}

void MainWindow::on_pushButton_10_clicked()
{
    on_groupSearchPB_clicked();
    on_nameSearchPB_clicked();
}
