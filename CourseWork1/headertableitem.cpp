#include "headertableitem.h"
#include "ui_headertableitem.h"
#include "headertableitemeditpage.h"

HeaderTableItem::HeaderTableItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeaderTableItem)
{
    ui->setupUi(this);
    ui->tblData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tblData->horizontalHeader()->setFont(ui->tblData->font());

    setWeekNum(QDateTime::currentDateTime().date().weekNumber());
    setYearNum(QDateTime::currentDateTime().date().year());
    ui->frameHeader->setStyleSheet("QFrame {background-color: white;}");
}

HeaderTableItem::~HeaderTableItem()
{
    delete ui;
}

void HeaderTableItem::setTitle(QString title) {
    ui->lblHeader->setText(title);
}

QString HeaderTableItem::getTitle() {
    return ui->lblHeader->text();
}

void HeaderTableItem::setDay(QString day) {
    ui->lblDay->setText(day);
}

QString HeaderTableItem::getDay() {
    return ui->lblDay->text();
}

void HeaderTableItem::setDayType(HeaderTableItem::DayType type) {
    dayType_ = type;

    switch (dayType_) {
    case DayType::Work:
        ui->frameHeader->setStyleSheet("QFrame {background-color: #E57373;}");
        break;
    case DayType::Education:
        ui->frameHeader->setStyleSheet("QFrame {background-color: #64B5F6;}");
        break;
    case DayType::EducationAndWork:
        ui->frameHeader->setStyleSheet("QFrame {background-color: #9575CD;}");
        break;
    case DayType::DayOff:
        ui->frameHeader->setStyleSheet("QFrame {background-color: white;}");
        break;
    default:
        break;
    }
}

HeaderTableItem::DayType HeaderTableItem::getDayType() {
    return dayType_;
}

QString HeaderTableItem::getDayTypeStr() {
    switch (dayType_) {
    case DayType::Work:
        return QString("Рабочий");
    case DayType::Education:
        return QString("Учебный");
    case DayType::EducationAndWork:
        return QString("Смешанный");
    case DayType::DayOff:
        return QString("Выходной");
    default:
        return QString();
    }
}

void HeaderTableItem::updateDayTypeInBase() {
    if (day_ != HeaderTableItem::WeekDay::None || day_ != HeaderTableItem::WeekDay::Any) {
        QSqlQuery* query = new QSqlQuery();
        query->prepare("REPLACE INTO ТипДня(Тип, День) VALUES(?, ?)");
        query->bindValue(0, static_cast<int>(dayType_));
        query->bindValue(1, getDate());
        query->exec();
        query->clear();
        delete query;
    }
}

void HeaderTableItem::setTableModel(QAbstractItemModel* model) {
    ui->tblData->setModel(model);
    QSqlTableModel* sqlModel = reinterpret_cast<QSqlTableModel*>(ui->tblData->model());
    if (type_ == HeaderTableItem::Type::Day) {
        ui->tblData->hideColumn(sqlModel->fieldIndex((QString("Описание"))));
        ui->tblData->hideColumn(sqlModel->fieldIndex((QString("Год"))));
        ui->tblData->hideColumn(sqlModel->fieldIndex((QString("Неделя"))));
        ui->lblDay->setMinimumWidth(ui->tblData->columnWidth(0) - 1);
    }
    else {
        dayVisible(false);
    }
}

QAbstractItemModel* HeaderTableItem::getTableModel() {
    return ui->tblData->model();
}

void HeaderTableItem::setType(HeaderTableItem::Type type) {
    if (type_ == HeaderTableItem::Type::None) {
        type_ = type;
        if (type_ == HeaderTableItem::Type::Day) {
            QDate weekStart = QDate::currentDate();
            if (weekStart.dayOfWeek() != 1) {
                weekStart = weekStart.addDays(-weekStart.dayOfWeek() + 1);
            }

            if (day_ != HeaderTableItem::WeekDay::None) {
                setDay(QString::number(weekStart.addDays(static_cast<int>(day_) - 1).day()));
            }
        }
    }
}

HeaderTableItem::Type HeaderTableItem::getType() {
    return type_;
}

void HeaderTableItem::setDayNum(HeaderTableItem::WeekDay day) {
    if (day_ == HeaderTableItem::WeekDay::None) {
        day_ = day;
        if (day_ == HeaderTableItem::WeekDay::Any) {
            dayVisible(false);
        }
    }
}

HeaderTableItem::WeekDay HeaderTableItem::getDayNum() {
    return day_;
}

QString HeaderTableItem::getDayDBName() {
    switch (day_) {
    case HeaderTableItem::WeekDay::None:
        return QString("");
    case HeaderTableItem::WeekDay::Monday:
        return QString("Понедельник");
    case HeaderTableItem::WeekDay::Tuesday:
        return QString("Вторник");
    case HeaderTableItem::WeekDay::Wednesday:
        return QString("Среда");
    case HeaderTableItem::WeekDay::Thursday:
        return QString("Четверг");
    case HeaderTableItem::WeekDay::Friday:
        return QString("Пятница");
    case HeaderTableItem::WeekDay::Saturday:
        return QString("Суббота");
    case HeaderTableItem::WeekDay::Sunday:
        return QString("Воскресенье");
    case HeaderTableItem::WeekDay::Any:
        return QString("Любой");
    default:
        return QString("");
    }
}

QStringList HeaderTableItem::getAllTasks() {
    QStringList tasks;

    QSqlTableModel* model = reinterpret_cast<QSqlTableModel*>(ui->tblData->model());

    QSqlQuery query = model->query();
    if (type_ == Type::Day) {
        query.prepare("SELECT Время,Задание,Описание FROM " + model->tableName());
    }
    else {
        query.prepare("SELECT * FROM " + model->tableName());
    }
    query.exec();

    if (type_ == Type::Day) {
        while(query.next()) {
            tasks << query.value("Время").value<QString>() + " " + query.value("Задание").value<QString>() + " " + query.value("Описание").value<QString>();
        }
    }
    else {

    }

    query.clear();

    return tasks;
}

void HeaderTableItem::setWeekNum(int week) {
    weekNumber_ = week;
}

int HeaderTableItem::getWeekNum() {
    return weekNumber_;
}

void HeaderTableItem::setYearNum(int year) {
    yearNumber_ = year;
}

int HeaderTableItem::getYearNum() {
    return yearNumber_;
}

void HeaderTableItem::setDate(QDate date) {
    date_ = date;
}

QDate HeaderTableItem::getDate() {
    return date_;
}

void HeaderTableItem::copySettings(HeaderTableItem* item) {
    this->type_ = Type::None;
    this->day_ = WeekDay::None;

    setType(item->getType());
    setDayNum(item->getDayNum());

    setYearNum(item->getYearNum());
    setWeekNum(item->getWeekNum());

    setTitle(item->getTitle());
    setDay(item->getDay());
    setTableModel(item->getTableModel());

    setDayType(item->getDayType());
}

int HeaderTableItem::getSelectedRowIndex() {
    return ui->tblData->currentIndex().row();
}

void HeaderTableItem::setSelectedRow(int index) {
    QModelIndex rowIndex = ui->tblData->model()->index(index, 0);
    ui->tblData->scrollTo(rowIndex);
    ui->tblData->setCurrentIndex(rowIndex);
}

QMap<Field, QVariant>  HeaderTableItem::getRow(int index) {
    QSqlTableModel* model = reinterpret_cast<QSqlTableModel*>(ui->tblData->model());

    QMap<Field, QVariant> data;
    switch (getType()) {
    case HeaderTableItem::Type::Day:
        data[Field::Date] = QDateTime::fromString(model->index(index, 0).data().value<QString>(), "hh:mm");
        data[Field::Task] = model->index(index, 1).data().value<QString>();
        data[Field::Description] = model->index(index, 2).data().value<QString>();
        break;
    case HeaderTableItem::Type::Education:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        data[Field::Description] = model->index(index, 1).data().value<QString>();
        data[Field::Date] = QDateTime::fromString(model->index(index, 2).data().value<QString>(), "dd.MM.yyyy HH:mm");
        break;
    case HeaderTableItem::Type::Work:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        data[Field::Date] = QDateTime::fromString(model->index(index, 1).data().value<QString>(), "dd.MM.yyyy HH:mm");
        break;
    case HeaderTableItem::Type::Personal:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        break;
    case HeaderTableItem::Type::Places:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        break;
    case HeaderTableItem::Type::VisitPlan:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        data[Field::Date] = QDateTime::fromString(model->index(index, 1).data().value<QString>(), "dd.MM.yyyy HH:mm");
        break;
    case HeaderTableItem::Type::PersonalPlan:
        data[Field::Task] = model->index(index, 0).data().value<QString>();
        data[Field::Image] = model->index(index, 0).data().value<QString>();
        break;
    default:
        break;
    }

    return data;
}

void HeaderTableItem::addRow(QMap<Field, QVariant> data) {
    QAbstractItemModel* model = ui->tblData->model();
    int newRowIndex = model->rowCount();

    model->insertRow(newRowIndex);

    switch (getType()) {
    case HeaderTableItem::Type::Day:
        model->setData(model->index(newRowIndex, 0), data[Field::Date].value<QDateTime>().time().toString("hh:mm"));
        model->setData(model->index(newRowIndex, 1), data[Field::Task].value<QString>());
        model->setData(model->index(newRowIndex, 2), data[Field::Description].value<QString>());
        model->setData(model->index(newRowIndex, 3), getYearNum());
        model->setData(model->index(newRowIndex, 4), getWeekNum());
        break;
    case HeaderTableItem::Type::Education:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        model->setData(model->index(newRowIndex, 1), data[Field::Description].value<QString>());
        model->setData(model->index(newRowIndex, 2), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::Work:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        model->setData(model->index(newRowIndex, 1), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::Personal:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        break;
    case HeaderTableItem::Type::Places:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        break;
    case HeaderTableItem::Type::VisitPlan:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        model->setData(model->index(newRowIndex, 1), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::PersonalPlan:
        model->setData(model->index(newRowIndex, 0), data[Field::Task].value<QString>());
        model->setData(model->index(newRowIndex, 1), data[Field::Image].value<QString>());
        model->setData(model->index(newRowIndex, 1), QPixmap(data[Field::Image].value<QString>()), Qt::DecorationRole);
        break;
    default:
        break;
    }

    model->submit();
}

void HeaderTableItem::editRow(QMap<Field, QVariant> data, int row) {
    QAbstractItemModel* model = ui->tblData->model();

    switch (getType()) {
    case HeaderTableItem::Type::Day:
        model->setData(model->index(row, 0), data[Field::Date].value<QDateTime>().time().toString("hh:mm"));
        model->setData(model->index(row, 1), data[Field::Task].value<QString>());
        model->setData(model->index(row, 2), data[Field::Description].value<QString>());
        break;
    case HeaderTableItem::Type::Education:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        model->setData(model->index(row, 1), data[Field::Description].value<QString>());
        model->setData(model->index(row, 2), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::Work:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        model->setData(model->index(row, 1), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::Personal:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        break;
    case HeaderTableItem::Type::Places:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        break;
    case HeaderTableItem::Type::VisitPlan:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        model->setData(model->index(row, 1), data[Field::Date].value<QDateTime>().toString("dd.MM.yyyy HH:mm"));
        break;
    case HeaderTableItem::Type::PersonalPlan:
        model->setData(model->index(row, 0), data[Field::Task].value<QString>());
        model->setData(model->index(row, 1), data[Field::Image].value<QString>());
        break;
    default:
        break;
    }

    model->submit();
}

void HeaderTableItem::removeRow(int row) {
    ui->tblData->model()->removeRow(row);
}

QFrame* HeaderTableItem::getTitleFrame() {
    return ui->frameHeader;
}

QTableView* HeaderTableItem::getTableView() {
    return ui->tblData;
}

void HeaderTableItem::updateModel() {
    QSqlTableModel* model = reinterpret_cast<QSqlTableModel*>(ui->tblData->model());
    if (type_ == HeaderTableItem::Type::Day) {
        model->setFilter("Год = " + QString::number(getYearNum()) + " AND Неделя = " + QString::number(getWeekNum()));
    }

    model->select();
}

void HeaderTableItem::columnVisible(QString columnName, bool state) {
    QSqlTableModel* model = reinterpret_cast<QSqlTableModel*>(ui->tblData->model());
    int columnIndex = model->fieldIndex(columnName);
    if (columnIndex != -1 && state) {
        ui->tblData->showColumn(columnIndex);
    }
    else {
        ui->tblData->hideColumn(columnIndex);
    }
}

void HeaderTableItem::dayVisible(bool state) {
    ui->lblDay->setVisible(state);
    ui->lnDayHeader->setVisible(state);
}

void HeaderTableItem::configureVisible(bool state) {
    ui->btnConfigure->setVisible(state);
}

QString HeaderTableItem::getHtmlLayout() {
    QString htmlLayout;

    if (type_ == Type::Day) {
        htmlLayout.append(R"(<h1 align="center">)" + getDate().toString("dd MMMM yyyy") + "</h1>");
    }

    htmlLayout.append(R"(<h1 align="center">)" + ui->lblHeader->text() + "</h1>");

    if (type_ == Type::Day) {
        htmlLayout.append(R"(<h2 align="center">)" + tr("Day type:") + " " + getDayTypeStr() + "</h2>");
    }

    htmlLayout.append(R"(<div height="10px"></div>)");

    QAbstractItemModel* model = ui->tblData->model();

    QString tableHeader;
    tableHeader.append(R"(<tr bgcolor="#ffffff">)");
    for (int i = 0; i < model->columnCount(); i++) {
        QString headerColumn = model->headerData(i, Qt::Horizontal).value<QString>();
        if (type_ == Type::Day) {
            if (headerColumn == "Год" || headerColumn == "Неделя") {
                continue;
            }
        }
        tableHeader.append("<th>" + headerColumn + "</th>");
    }
    tableHeader.append("</tr>");

    QString tableBody;
    for (int i = 0; i < model->rowCount(); i++) {
        tableBody.append(R"(<tr bgcolor="#ffffff">)");
        for (int j = 0; j < model->columnCount(); j++) {
            QString headerColumn = model->headerData(j, Qt::Horizontal).value<QString>();
            if (type_ == Type::Day) {
                if (headerColumn == "Год" || headerColumn == "Неделя") {
                    continue;
                }
            }
            tableBody.append(R"(<td align="center">)");
            if (headerColumn == "Изображение") {
                tableBody.append(R"(<img height="200" src=")");
            }
            tableBody.append(model->index(i, j).data().value<QString>());
            if (headerColumn == "Изображение") {
                tableBody.append(R"(">)");
            }
            tableBody.append("</td>");
        }
        tableBody.append("</tr>");
    }

    QString table;
    table.append(R"(<table width="100%" cellpadding="6" align="center" border="0" bgcolor="#000000">)");
    table.append(tableHeader);
    table.append(tableBody);
    table.append("</table>");

    htmlLayout.append(table);

    return htmlLayout;
}

void HeaderTableItem::on_btnConfigure_clicked()
{
    btnCofifguredClicked(this);
}
