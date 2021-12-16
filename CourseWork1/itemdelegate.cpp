#include "itemdelegate.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QStandardItemModel>
#include <QTextEdit>
#include <iostream>

ItemDelegate::ItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QTextEdit *path = new QTextEdit(parent);
    return path;
}


void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QTextEdit *path = static_cast<QTextEdit*>(editor);
    path->setText(value);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTextEdit *path = static_cast<QTextEdit*>(editor);

    QString data = path->toPlainText();
    model->setData(index, data);
}

void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPixmap image = index.data(Qt::DecorationRole).value<QPixmap>();

    if (!image.isNull()) {
        int width = std::min(option.rect.width(), image.width());
        int height = std::min(image.height(), 200);
        image = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(option.rect.x() + (option.rect.width() / 2) - (image.width() / 2), option.rect.y(), image.width(), image.height(), image);
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    QPixmap image = index.data(Qt::DecorationRole).value<QPixmap>();

    if (!image.isNull()) {
        int width = std::min(option.rect.width(), image.width());
        int height = std::min(image.height(), 200);
        image = image.scaled(width, height, Qt::KeepAspectRatio);
        return image.size();
    }

    return option.rect.size();
}
