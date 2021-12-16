#include "headmen.h"
#include "ui_headmen.h"

headmen::headmen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::headmen)
{
    ui->setupUi(this);
}

headmen::~headmen()
{
    delete ui;
}
