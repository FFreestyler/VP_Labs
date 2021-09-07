#ifndef STARTDIALOG_MENCHSHIKOV_ISMAILOV_H
#define STARTDIALOG_MENCHSHIKOV_ISMAILOV_H


#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include "InputDialog.h"

class StartDialog_Menchshikov_Ismailov: public QPushButton {
            Q_OBJECT

public:
    StartDialog_Menchshikov_Ismailov(QWidget* pwgt = 0) : QPushButton("Нажми", pwgt)
    {
        connect(this, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    }
public slots:
    void slotButtonClicked() {
        InputDialog* pInputDialog = new InputDialog;
        if(pInputDialog->exec() == QDialog::Accepted) {
            QMessageBox::information(0,
                                     "Ваша Информация: ",
                                     "Имя: "
                                     + pInputDialog->firstName()
                                     + "\nФамиля: "
                                     + pInputDialog->lastName()
                                     );
        }
        delete pInputDialog;
    }
};

#endif // STARTDIALOG_MENCHSHIKOV_ISMAILOV_H
