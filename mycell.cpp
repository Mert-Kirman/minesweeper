#include "mycell.h"

MyCell::MyCell(QWidget *parent) : QPushButton(parent) {
    QIcon icon(":/images/cell_images/empty.png");
    setIcon(icon);

    QSize iconSize(15, 15);
    setIconSize(iconSize);
    setFixedSize(iconSize);

    setFlat(true);
    setStyleSheet("border: none; padding: 0px;");
    update();
}
