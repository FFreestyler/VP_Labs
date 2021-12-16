#include "docwindow_menchshikov_ismailov.h"
#include <QTextEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QColorDialog>

DocWindow_Menchshikov_Ismailov::DocWindow_Menchshikov_Ismailov(QWidget* pwgt) : QTextEdit(pwgt)
{

}

void DocWindow_Menchshikov_Ismailov::slotLoad() {
    QString str = QFileDialog::getOpenFileName();
    if(str.isEmpty()) {
        return;
    }

    QFile file(str);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        setPlainText(stream.readAll());
        file.close();
        m_strFileName=str;
        emit changeWindowTitle(m_strFileName);
    }
}

void DocWindow_Menchshikov_Ismailov::slotSaveAs() {
    QString str = QFileDialog::getSaveFileName(0, m_strFileName);
    if(!str.isEmpty()) {
        m_strFileName = str;
        slotSave();
        return;
    }
}

void DocWindow_Menchshikov_Ismailov::slotSave() {
    if(m_strFileName.isEmpty()) {
        slotSaveAs();
        return;
    }

    QFile file(m_strFileName);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << toPlainText();

        file.close();
        emit changeWindowTitle(m_strFileName);
    }
}

void DocWindow_Menchshikov_Ismailov::slotColor() {
    QColor newColor = QColorDialog::getColor();
    setTextColor(newColor);
}
