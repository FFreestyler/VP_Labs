#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>

class ImageButton : public QPushButton
{
    Q_OBJECT

public:
    ImageButton(QWidget *parent=0);
    ~ImageButton();

private:
    void paintEvent(QPaintEvent *event);

    int pad;
    int minSize;
};

#endif // IMAGEBUTTON_H
