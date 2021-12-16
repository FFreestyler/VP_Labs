#include "imagebutton.h"

#include <QSize>
#include <QSizePolicy>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QtGlobal>

ImageButton::ImageButton(QWidget *parent)
    : QPushButton(parent)
{
    pad = 4;
    minSize = 8;

    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
}

ImageButton::~ImageButton()
{

}

void ImageButton::paintEvent(QPaintEvent*)
{
    QStylePainter painter(this);

    QStyleOptionButton opt;
    this->initStyleOption(&opt);

    QRect r = opt.rect;
    int iconSize = qMax(qMin(r.height(), r.width()) - 2 * this->pad, this->minSize);
    opt.iconSize = QSize(iconSize, iconSize);

    painter.drawControl(QStyle::CE_PushButton, opt);
}
