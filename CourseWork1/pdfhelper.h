#ifndef PDFHELPER_H
#define PDFHELPER_H

#include <QString>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>

class PdfHelper
{
public:
    void static SaveToPdfDialog(QWidget* parent, QString dialogTitle, QString htmlString);
};

#endif // PDFHELPER_H
