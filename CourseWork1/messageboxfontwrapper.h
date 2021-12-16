#ifndef MESSAGEBOXFONTWRAPPER_H
#define MESSAGEBOXFONTWRAPPER_H

#include <QMessageBox>
#include <QFont>

class MessageBoxFontWrapper
{
public:
    static QMessageBox* SetStandardFont(QMessageBox* messageBox);
    static QMessageBox* ExecStandardFont(QMessageBox* messageBox);
};

#endif // MESSAGEBOXFONTWRAPPER_H
