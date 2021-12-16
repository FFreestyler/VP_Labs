#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QLibraryInfo>

void loadTranslation(QString translation, QString directory = QString()) {
    QTranslator *translator = new QTranslator(qApp);
    bool isSuccess = translator->load(translation, directory);

    if (isSuccess) {
        qApp->installTranslator(translator);
    }
    else {
        delete translator;
        QMessageBox::warning(0,
                    "Error when loading translation",
                    "There was an error loading the default translation\"" + QLocale::system().name() + "\"");
    }
}
void loadStylesheet(QString filename) {
    QFile file(filename);
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(qApp->styleSheet() + file.readAll());
}
void loadIcon(QString filename) {
    QIcon appIcon = QIcon(filename);
    qApp->setWindowIcon(appIcon);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loadIcon(":/images/app_icon.ico");

    loadTranslation(QLatin1String("qt_") + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    loadTranslation(QString("QtLanguage_") + QString("ru_RU"), QString(":/translations/"));

    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));

    MainWindow w;
    w.show();

    return a.exec();
}
