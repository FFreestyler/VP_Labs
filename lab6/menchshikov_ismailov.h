#ifndef MENCHSHIKOV_ISMAILOV_H
#define MENCHSHIKOV_ISMAILOV_H

#include <QWidget>
#include <QFile>
#include <QAbstractButton>

namespace Ui {
class Menchshikov_Ismailov;
}

class Menchshikov_Ismailov : public QWidget
{
    Q_OBJECT

public:
    explicit Menchshikov_Ismailov(QWidget *parent = 0);
    ~Menchshikov_Ismailov();

private:
    Ui::Menchshikov_Ismailov *ui;

public slots:
    void recieveData(QString str);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // MENCHSHIKOV_ISMAILOV_H
