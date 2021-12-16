#include "headertableitemeditpage.h"
#include "ui_headertableitemeditpage.h"
#include "adddialog.h"
#include <QDialogButtonBox>
#include "itemdelegate.h"

HeaderTableItemEditPage::HeaderTableItemEditPage(QWidget *parent, HeaderTableItem* item) :
    QWidget(parent),
    ui(new Ui::HeaderTableItemEditPage)
{
    ui->setupUi(this);

    this->item_ = item;
    ui->hitEditingItem->copySettings(item);
    ui->hitEditingItem->configureVisible(false);
    ui->hitEditingItem->updateModel();
    ui->hitEditingItem->getTitleFrame()->setFrameShape(QFrame::Box);

    if (item_->getType() == HeaderTableItem::Type::Day) {
        ui->hitEditingItem->columnVisible(QString("Описание"), true);
    }

    if (item_->getType() == HeaderTableItem::Type::PersonalPlan) {
        ItemDelegate *itDelegate = new  ItemDelegate;
        ui->hitEditingItem->getTableView()->setItemDelegateForColumn(1, itDelegate);
        ui->hitEditingItem->getTableView()->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->hitEditingItem->getTableView()->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->hitEditingItem->getTableView()->setColumnWidth(1, 200);
    }

    int colInsert = 0;
    ui->layoutPage->addWidget(ui->btnBack, 0, colInsert++);
    ui->layoutPage->addWidget(ui->btnSave, 1, 0);
    if (item_->getType() == HeaderTableItem::Type::Day) {
        if (item->getDayNum() != HeaderTableItem::WeekDay::Any) {
            cmbDayType = new WideComboBox(this);
            cmbDayType->addItem(QString("Учебный"), static_cast<int>(HeaderTableItem::DayType::Education));
            cmbDayType->addItem(QString("Рабочий"), static_cast<int>(HeaderTableItem::DayType::Work));
            cmbDayType->addItem(QString("Смешанный"), static_cast<int>(HeaderTableItem::DayType::EducationAndWork));
            cmbDayType->addItem(QString("Выходной"), static_cast<int>(HeaderTableItem::DayType::DayOff));
            cmbDayType->setCurrentIndex(static_cast<int>(item_->getDayType()));

            cmbDayType->setMaximumHeight(ui->btnBack->maximumHeight());
            cmbDayType->setFont(QFont("Roboto", 12));
            cmbDayType->setToolTip(tr("Day type"));

            connect(cmbDayType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_dayTypeSelected(int)));

            ui->layoutPage->addWidget(cmbDayType, 0, colInsert++);
        }
    }
    ui->layoutPage->addItem(ui->upSpacer, 2, 0);
    ui->layoutPage->addWidget(ui->btnAdd, 3, 0);
    ui->layoutPage->addWidget(ui->btnEdit, 4, 0);
    ui->layoutPage->addWidget(ui->btnRemove, 5, 0);
    ui->layoutPage->addItem(ui->downSpacer, 6, 0);
    ui->layoutPage->addWidget(ui->hitEditingItem->getTitleFrame(), 0, colInsert++, 1, -1);
    ui->layoutPage->addWidget(ui->hitEditingItem, 1, 1, -1, -1);
}

HeaderTableItemEditPage::~HeaderTableItemEditPage()
{
    ui->layoutPage->removeWidget(ui->btnBack);
    ui->layoutPage->removeWidget(ui->btnSave);
    if (cmbDayType != nullptr) {
        ui->layoutPage->removeWidget(cmbDayType);
    }
    ui->layoutPage->removeItem(ui->upSpacer);
    ui->layoutPage->removeWidget(ui->btnAdd);
    ui->layoutPage->removeWidget(ui->btnEdit);
    ui->layoutPage->removeWidget(ui->btnRemove);
    ui->layoutPage->removeItem(ui->downSpacer);
    ui->layoutPage->removeWidget(ui->hitEditingItem->getTitleFrame());
    ui->layoutPage->removeWidget(ui->hitEditingItem);
    delete ui;
}

void HeaderTableItemEditPage::on_btnBack_clicked()
{
    if (item_->getType() == HeaderTableItem::Type::VisitPlan
            || item_->getType() == HeaderTableItem::Type::PersonalPlan) {
        if (item_->getTableModel()->rowCount() > 0) {
            QMessageBox *dlg = MessageBoxFontWrapper::SetStandardFont(
                        new QMessageBox(
                            QMessageBox::Warning,
                            tr("Cleanup page?"),
                            tr("Do you want to clear the plan page before exiting?"),
                            QMessageBox::Ok | QMessageBox::No,
                            this)
                        );
            if (dlg->exec() == QMessageBox::Ok) {
                item_->getTableModel()->removeRows(0, item_->getTableModel()->rowCount());
            }
        }
    }
    btnBackClicked();
}

void HeaderTableItemEditPage::on_btnSave_clicked()
{
    PdfHelper::SaveToPdfDialog(this, tr("Saving the day plan"), ui->hitEditingItem->getHtmlLayout());
}

void HeaderTableItemEditPage::on_dayTypeSelected(int index) {
    HeaderTableItem::DayType newType = static_cast<HeaderTableItem::DayType>(cmbDayType->itemData(index).value<int>());
    ui->hitEditingItem->setDayType(newType);
    item_->setDayType(newType);
    item_->updateDayTypeInBase();
}

void HeaderTableItemEditPage::on_btnAdd_clicked()
{
    AddDialog* dlg = new AddDialog(this, tr("Add task"), item_->getType());
    dlg->open();

    connect(dlg, &AddDialog::finished, [=](int result) {
        if (result == QDialog::Accepted) {
            item_->addRow(dlg->getFields());
            item_->updateModel();

            QAbstractItemModel* model;
            if((model = ui->hitEditingItem->getTableModel()) != nullptr) {
                ui->hitEditingItem->setSelectedRow(model->rowCount());
            }
        }

        dlg->deleteLater();
    });
}

void HeaderTableItemEditPage::on_btnEdit_clicked()
{
    int selectedRow = ui->hitEditingItem->getSelectedRowIndex();
    if (selectedRow == -1) {
        return;
    }

    AddDialog* dlg = new AddDialog(this, tr("Edit task"), item_->getType());
    dlg->setFields(ui->hitEditingItem->getRow(ui->hitEditingItem->getSelectedRowIndex()));
    dlg->open();

    connect(dlg, &AddDialog::finished, [=](int result) {
        if (result == QDialog::Accepted) {
            item_->editRow(dlg->getFields(), selectedRow);
            item_->updateModel();

            ui->hitEditingItem->setSelectedRow(selectedRow);
        }

        dlg->deleteLater();
    });
}

void HeaderTableItemEditPage::on_btnRemove_clicked()
{
    int selectedRow = ui->hitEditingItem->getSelectedRowIndex();
    item_->removeRow(ui->hitEditingItem->getSelectedRowIndex());
    item_->updateModel();
    ui->hitEditingItem->setSelectedRow(selectedRow);
}
