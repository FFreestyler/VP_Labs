#include "pdfhelper.h"

void PdfHelper::SaveToPdfDialog(QWidget* parent, QString dialogTitle, QString htmlString)
{
    QString filename = QFileDialog::getSaveFileName(parent,
                                                    dialogTitle,
                                                    QDir::homePath(),
                                                    QString("PDF (*.pdf);;"),
                                                    new QString("PDF (*.pdf)"));

    if (filename.isEmpty()) {
        return;
    }

    QTextDocument document;
    document.setHtml(htmlString);
    document.setDefaultFont(QFont("Roboto", 11));

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(filename);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.setPageSize(printer.pageRect().size());
    document.print(&printer);
}
