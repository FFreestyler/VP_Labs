#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "menchshikov_ismailov.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    MainWindow w;
    w.show();

    return a.exec();
}
