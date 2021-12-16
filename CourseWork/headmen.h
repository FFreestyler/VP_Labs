#ifndef HEADMEN_H
#define HEADMEN_H

#include <QWidget>

namespace Ui {
class headmen;
}

class headmen : public QWidget
{
    Q_OBJECT

public:
    explicit headmen(QWidget *parent = 0);
    ~headmen();

private:
    Ui::headmen *ui;
};

#endif // HEADMEN_H
