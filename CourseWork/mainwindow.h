#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <infostud.h>

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QFile>
#include <QDate>
#include <QIODevice>
#include <QSqlError>
#include <QTextCodec>
#include <QListWidget>
#include <QDataStream>
#include <QTextStream>
#include <QVector>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QPdfWriter>
#include <QPainter>
#include <QItemDelegate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addStud_clicked();

    void on_pushButton_clicked();

    void on_DataBase_clicked(const QModelIndex &index);

    void updateGroupStudentComboBox();

    void on_delStud_clicked();

    void on_addEvent_clicked();

    void on_eventDataBase_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_addPhotoButton_clicked();

    void on_prevPhotoButton_clicked();

    void on_nextPhotoButton_clicked();

    void createStudentFolder();

    void updateEventStudentComboBox();

    void on_eventStudentComboBox_currentIndexChanged(int index);

    void on_pushButton_3_clicked();

    void on_eventStudentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void createProgressTableView();

    void fillProgressTable();

    void saveProgressTable();

    void checkProgressFile();

    void readProgressTable();

    void on_saveProgressButton_clicked();

    void setSessionProgress();
    
    void on_sessionResultProgressButton_clicked();

    void on_unsuccessfulProgressButton_clicked();

    void on_resetProgressButton_clicked();

    void on_createReportProgressButton_clicked();

    void updateGroupProgressComboBox();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void createHeadmanTableView();

    void updateGroupHeadmanComboBox();

    void updateStudentsHeadmanComboBox();

    void on_groupHeadmanComboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_headmanTableView_clicked(const QModelIndex &index);

    void saveHeadmanTable();

    void readHeadmanTable();

    void checkHeadmanTable();

    void on_deletePhotoButton_clicked();

    void on_progressTableView_activated(const QModelIndex &index);

    void on_groupSearchPB_clicked();

    void on_nameSearchPB_clicked();

    void closeEvent(QCloseEvent* );

    void on_pushButton_9_clicked();

    void onButtonSendProgress();

    void onButtonSendData();

    void createAllStudentsFolder();

    void findEqual();

    void on_action_triggered();

    void on_pushButton_10_clicked();

signals:
    void sendData(QString name, QString birth, QString group, QString mail, QString telephone, QString photo);

    void sendDataProgress(QString os, QString math, QString prog, QString geom, QString sess);

    void sendDataEvent(QVector<QString> inf);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;

    QSqlDatabase eventDB;
    QSqlQuery *eventQuery;
    QSqlTableModel *eventModel;

    QStandardItemModel* progressModel;

    QStandardItemModel* headmanModel;
    QStandardItemModel* headmanCsvModel;

    QSet<QString> uniqueGroup;

    QVector<QString> images;
    QVector<QString> studentName;
    QVector<QString> studentGroup;
    QVector<QString> columnName;
    QVector<QString> headmanColName;

    int row = 1;
    int eventRow = 1;
    int counter = 1;
    int headmanRow = 1;

    infoStud *infoStudent = new infoStud();
};

#endif // MAINWINDOW_H
