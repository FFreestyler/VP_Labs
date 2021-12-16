#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent, QString title, HeaderTableItem::Type type) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);

    setWindowTitle(title);

    setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(checkAccepted()));

    comboBoxSavedStyle = ui->cmbTask->styleSheet();
    type_ = type;

    ui->dtTime->setDateTime(QDateTime::currentDateTime());

    switch (type) {
    case HeaderTableItem::Type::Day:
        ui->dtTime->setDisplayFormat("HH:mm");
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
    {
        auto insertTasksFromTable = [this](QString table){
            QStringList tasks;
            QSqlQuery* query = new QSqlQuery();
            query->prepare("SELECT * FROM " + table);
            query->exec();
            while (query->next()) {
                QString data;
                for (int i = 0; query->value(i).isValid(); i++) {
                    data.append(query->value(i).value<QString>() + " | ");
                }
                data.remove(data.count() - 3, 3);
                tasks.append(data);
            }
            query->clear();
            ui->cmbTask->addItems(tasks);
            ui->cmbTask->insertSeparator(ui->cmbTask->count());
            delete query;
        };

        insertTasksFromTable("Учебные");
        insertTasksFromTable("Рабочие");
        insertTasksFromTable("Личные");
        insertTasksFromTable("Места");
        ui->cmbTask->removeItem(ui->cmbTask->count()-1);

//        QStringList tasks;
//        QSqlQuery* query = new QSqlQuery();
//        query->prepare("SELECT Задание FROM Учебные");
//        query->exec();
//        while (query->next()) {
//            tasks.append(query->value(0).value<QString>());
//        }
//        query->clear();
//        int sep1 = tasks.count();
//        query->prepare("SELECT Задание FROM Рабочие");
//        query->exec();
//        while (query->next()) {
//            tasks.append(query->value(0).value<QString>());
//        }
//        query->clear();
//        int sep2 = tasks.count();
//        query->prepare("SELECT Задание FROM Личные");
//        query->exec();
//        while (query->next()) {
//            tasks.append(query->value(0).value<QString>());
//        }
//        query->clear();
//        int sep3 = tasks.count();
//        query->prepare("SELECT Задание FROM Место");
//        query->exec();
//        while (query->next()) {
//            tasks.append(query->value(0).value<QString>());
//        }
//        query->clear();
//        int sep4 = tasks.count();
//        delete query;
//        ui->cmbTask->addItems(tasks);

//        if (sep1) {
//            ui->cmbTask->insertSeparator(sep1);
//        }

//        if (sep2) {
//            ui->cmbTask->insertSeparator(sep1 + sep2);
//        }

//        if (sep3) {
//            ui->cmbTask->insertSeparator(sep1 + sep2 + sep3);
//        }

//        if (sep4) {
//            ui->cmbTask->insertSeparator(sep1 + sep2 + sep3 + sep4);
//        }
    }
        count = 3;
        break;
    case HeaderTableItem::Type::Education:
        ui->lblTask->setText("Предмет");
        ui->lblDescription->setText("Задание");
        ui->lblTime->setText("Срок сдачи");
        count = 3;
        hideComboBoxArrow(true);
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
        break;
    case HeaderTableItem::Type::Work:
        ui->lblTask->setText("Задание");
        ui->lblTime->setText("Срок сдачи");
        count = 2;
        ui->pteDescription->setVisible(false);
        ui->lblDescription->setVisible(false);
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
        hideComboBoxArrow(true);
        break;
    case HeaderTableItem::Type::Personal:
        ui->lblTask->setText("Задание");
        count = 1;
        ui->pteDescription->setVisible(false);
        ui->lblDescription->setVisible(false);
        ui->dtTime->setVisible(false);
        ui->lblTime->setVisible(false);
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
        hideComboBoxArrow(true);
        break;
    case HeaderTableItem::Type::Places:
        ui->lblTask->setText("Задание");
        count = 1;
        ui->pteDescription->setVisible(false);
        ui->lblDescription->setVisible(false);
        ui->dtTime->setVisible(false);
        ui->lblTime->setVisible(false);
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
        hideComboBoxArrow(true);
        break;
    case HeaderTableItem::Type::VisitPlan:
    {
        QStringList tasks;
        QSqlQuery* query = new QSqlQuery();
        query->prepare("SELECT Задание FROM Места");
        query->exec();
        while (query->next()) {
            tasks.append(query->value(0).value<QString>());
        }
        query->clear();
        delete query;
        ui->cmbTask->addItems(tasks);
    }
        ui->pteDescription->setVisible(false);
        ui->lblDescription->setVisible(false);
        ui->lblImage->setVisible(false);
        ui->frameImage->setVisible(false);
        count = 2;
        break;
    case HeaderTableItem::Type::PersonalPlan:
    {
        QStringList tasks;
        QSqlQuery* query = new QSqlQuery();
        query->prepare("SELECT Задание FROM Личные");
        query->exec();
        while (query->next()) {
            tasks.append(query->value(0).value<QString>());
        }
        query->clear();
        delete query;
        ui->cmbTask->addItems(tasks);
    }
        ui->pteDescription->setVisible(false);
        ui->lblDescription->setVisible(false);
        ui->dtTime->setVisible(false);
        ui->lblTime->setVisible(false);
        count = 3;
        break;
    default:
        break;
    }

    QApplication::processEvents();
    adjustSize();
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setTask(QString task) {
    ui->cmbTask->setCurrentText(task);
}

QString AddDialog::getTask() {
    return ui->cmbTask->currentText().trimmed();
}

void AddDialog::setDescription(QString description) {
    ui->pteDescription->setPlainText(description);
}

QString AddDialog::getDescription() {
    return ui->pteDescription->toPlainText().trimmed();
}

void AddDialog::setTime(QDateTime time) {
    ui->dtTime->setDateTime(time);
}

QDateTime AddDialog::getTime() {
    return ui->dtTime->dateTime();
}

void AddDialog::setImage(QString filename) {
    ui->imgImage->setPixmap(QPixmap(filename));
    ui->imgImage->setAccessibleName(filename);
}

QString AddDialog::getImage() {
    if (ui->imgImage->pixmap() != nullptr) {
        return ui->imgImage->accessibleName();
    }

    return QString();
}

int AddDialog::getFieldCount() {
    return count;
}

QMap<Field, QVariant> AddDialog::getFields() {
    QMap<Field, QVariant> data;
    data[Field::Task] = getTask();
    data[Field::Description] = getDescription();
    data[Field::Date] = getTime();
    data[Field::Image] = getImage();

    return data;
}

void AddDialog::setFields(QMap<Field, QVariant> data) {
    setTask(data[Field::Task].value<QString>());
    setDescription(data[Field::Description].value<QString>());
    setTime(data[Field::Date].value<QDateTime>());
    setImage(data[Field::Image].value<QString>());
}

void AddDialog::checkAccepted() {
    QMessageBox* notFilledFieldsMessage = MessageBoxFontWrapper::SetStandardFont(
                new QMessageBox(QMessageBox::Information,
                                tr("Not all fields are filled"),
                                tr("Fields must not be empty"),
                                QMessageBox::StandardButton::NoButton,
                                this)
                );

    if (ui->cmbTask->currentText().trimmed().isEmpty()) {
        notFilledFieldsMessage->exec();
        return;
    }

    if (type_ == HeaderTableItem::Type::Education) {
        if (ui->pteDescription->toPlainText().trimmed().isEmpty()) {
            notFilledFieldsMessage->exec();
            return;
        }
    }

    delete notFilledFieldsMessage;

    accept();
}

void AddDialog::hideComboBoxArrow(bool state) {
    if (state) {
        ui->cmbTask->setStyleSheet("QComboBox::drop-down {width: 0px; border-left-width: 0px;}");
    }
    else {
        ui->cmbTask->setStyleSheet(comboBoxSavedStyle);
    }
}

void AddDialog::on_btnLoadImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load image"),
                                                    QDir::homePath(),
                                                    QString("Image (*.bmp *.gif *.jpeg *.jpg *.png *.svg *.svgz *.tga *.tif *.tiff *.wbmp *.webp);;"),
                                                    new QString("Image (*.bmp *.gif *.jpeg *.jpg *.png *.svg *.svgz *.tga *.tif *.tiff *.wbmp *.webp)"));
    setImage(filename);
}
