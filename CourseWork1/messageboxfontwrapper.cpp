#include "messageboxfontwrapper.h"

QMessageBox* MessageBoxFontWrapper::SetStandardFont(QMessageBox* messageBox)
{
    QFont font("Roboto", 12);
    messageBox->setFont(font);
    return messageBox;
}

QMessageBox* MessageBoxFontWrapper::ExecStandardFont(QMessageBox* messageBox)
{
    SetStandardFont(messageBox);
    messageBox->exec();
    return messageBox;
}
