#ifndef SDIPROGRAM_H
#define SDIPROGRAM_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QtWidgets>
#include <QAction>
#include <QColorDialog>
#include "docwindow_menchshikov_ismailov.h"
#include "sdiprogram.h"

class SDIProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit SDIProgram(QWidget *pwgt = 0) : QMainWindow(pwgt) {
        QMenu* pmnuFile = new QMenu("&File");
        QAction* pmnuHelp = new QAction("&Help");
        DocWindow_Menchshikov_Ismailov* pdoc = new DocWindow_Menchshikov_Ismailov;
        pmnuFile->addAction("&Open...", pdoc, SLOT(slotLoad()), QKeySequence("CTRL+O"));
        pmnuFile->addAction("&Save", pdoc, SLOT(slotSave()), QKeySequence("CTRL+S"));
        pmnuFile->addAction("&Save As...", pdoc, SLOT(slotSaveAs()));
        pmnuFile->addAction("Color...", pdoc, SLOT(slotColor()));
        pmnuHelp->setShortcut(QKeySequence("F1"));

        menuBar()->addMenu(pmnuFile);
        menuBar()->addAction(pmnuHelp);

        setCentralWidget(pdoc);

        connect(pdoc,
                SIGNAL(changeWindowTitle(const QString&)),
                SLOT(slotChangeWindowTitle(const QString&))
                );
        connect(
                pmnuHelp,
                SIGNAL(triggered()),
                SLOT(slotAbout())
                );

        statusBar()->showMessage("Ready", 2000);
    }

public slots:
    void slotAbout(){
        QMessageBox::about(this, "Application", "ИП-916\n Меньщиков\n Исмаилов");
    }
    void slotChangeWindowTitle(const QString& str) {
        setWindowTitle(str);
    }
};

#endif // SDIPROGRAM_H
