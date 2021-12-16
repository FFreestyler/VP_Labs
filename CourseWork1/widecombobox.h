#ifndef WIDECOMBOBOX_H
#define WIDECOMBOBOX_H

#include <QComboBox>
#include <QAbstractItemView>

class WideComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit WideComboBox(QWidget *parent = nullptr) : QComboBox(parent) {}
    ~WideComboBox() {}
public:
    void showPopup();
};

#endif // WIDECOMBOBOX_H
