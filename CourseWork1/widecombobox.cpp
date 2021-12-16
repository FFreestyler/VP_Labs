#include "widecombobox.h"

void WideComboBox::showPopup() {
    this->view()->setMinimumWidth(this->view()->sizeHintForColumn(0));
    QComboBox::showPopup();
}
