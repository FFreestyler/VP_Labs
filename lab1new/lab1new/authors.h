#ifndef AUTHORS_H
#define AUTHORS_H

#include <QMainWindow>

namespace Ui {
class authors;
}

class authors : public QMainWindow
{
    Q_OBJECT

public:
    explicit authors(QWidget *parent = 0);
    ~authors();

private:
    Ui::authors *ui;
};

#endif // AUTHORS_H
