#ifndef DOCWINDOW_MENCHSHIKOV_ISMAILOV_H
#define DOCWINDOW_MENCHSHIKOV_ISMAILOV_H

#include <QTextEdit>

class DocWindow_Menchshikov_Ismailov : public QTextEdit
{
    Q_OBJECT
private:
    QString m_strFileName;

public:
    DocWindow_Menchshikov_Ismailov(QWidget * pwgt = 0);

signals:
    void changeWindowTitle(const QString&);

public slots:
    void slotLoad();
    void slotSave();
    void slotSaveAs();
    void slotColor();
};

#endif // DOCWINDOW_MENCHSHIKOV_ISMAILOV_H
