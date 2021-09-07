#include "authors.h"
#include "ui_authors.h"

authors::authors(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::authors)
{
    ui->setupUi(this);
}

authors::~authors()
{
    delete ui;
}
