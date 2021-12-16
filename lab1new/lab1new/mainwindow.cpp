#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authors.h"
#include "ui_authors.h"
#include "QFileDialog"
#include <QTextEdit>
#include <QTextDocumentWriter>

using namespace std;

void MainWindow::authorsLab1() {
    authors* massage = new authors();
    massage -> show();
}

void MainWindow::slotOpenFile() {
    QString filename = QFileDialog::getOpenFileName(0, "Открыть файл",
                                                    QDir::currentPath(),
                                                    "*.cpp *.txt");
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       ui->textEdit->setPlainText(file.readAll());
    }
}

void MainWindow::slotSaveFile() {
    QString filename = QFileDialog::getSaveFileName(0, "Сохранить файл",
                                                    QDir::currentPath(),
                                                    "*.cpp *.txt");
    QTextDocumentWriter writer;
    writer.setFileName(filename);
    writer.write(ui->textEdit->document());
}

void MainWindow::slotClearFile() {
    ui->textEdit->clear();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action, SIGNAL(triggered()),this, SLOT(authorsLab1()));

    QAction* pactOpen = new QAction("file open action", 0);
    pactOpen->setText("&Открыть");
    pactOpen->setShortcut(QKeySequence("CTRL+O"));
    pactOpen->setToolTip("Открытие документа");
    pactOpen->setStatusTip("Открыть файл");
    pactOpen->setWhatsThis("Открыть файл");
    //pactOpen->setIcon(QPixmap(".png"));
    connect(pactOpen, SIGNAL(triggered()), SLOT(slotOpenFile()));
    QMenu* menuFile = new QMenu("&Файл");
    menuFile->addAction(pactOpen);
    menuBar()->addMenu(menuFile);

    QAction* pactSave = new QAction("file save action", 0);
    pactSave->setText("&Сохранить");
    pactSave->setShortcut(QKeySequence("CTRL+S"));
    pactSave->setToolTip("Сохранение документа");
    pactSave->setStatusTip("Сохранить файл");
    pactSave->setWhatsThis("Сохранить файл");
    connect(pactSave, SIGNAL(triggered()), SLOT(slotSaveFile()));
    menuFile->addAction(pactSave);
    menuBar()->addMenu(menuFile);

    QAction* pactClear = new QAction("file save action", 0);
    pactClear->setText("&Очистить");
    pactClear->setToolTip("Очистить");
    pactClear->setStatusTip("Очистить текст");
    pactClear->setWhatsThis("Очистить текст");
    connect(pactClear, SIGNAL(triggered()), SLOT(slotClearFile()));
    menuFile->addAction(pactClear);
    menuBar()->addMenu(menuFile);

    ui->mainToolBar->addAction(pactOpen);
    ui->mainToolBar->addAction(pactSave);
    ui->mainToolBar->addAction(pactClear);
}

MainWindow::~MainWindow()
{
    delete ui;
}
