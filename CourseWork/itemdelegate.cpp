#include "itemdelegate.h"
#include <QLineEdit>
#include <QIntValidator>

ItemDelegate::ItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setCalendarPopup(true);
    //editor->setValidator(new QIntValidator);
    return editor;
}


void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QDateEdit *line = static_cast<QDateEdit*>(editor);
    //line->setText(value);
    line->setDate(QDate::fromString(value));
}

void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *line = static_cast<QDateEdit*>(editor);

    QString data = line->date().toString("dd.MM.yyyy");
    model->setData(index, data);
}
