#include "mycell.h"

MyCell::MyCell(QWidget *parent) : QPushButton(parent) {
    isRevealed = false;
    isMine = false;
    isFlagged = false;
    isSimulationMine = false;
    neighborMineCount = -1;

    // Set the empty cell icon at start
    QIcon icon(":/images/cell_images/empty.png");
    setIcon(icon);

    // Adjust the cell sizes to fit images
    QSize iconSize(15, 15);
    setIconSize(iconSize);
    setFixedSize(iconSize);

    setFlat(true);
    setStyleSheet("border: none; padding: 0px;");
    update();
}

void MyCell::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        emit leftClick();
    }
    else if(event->button() == Qt::RightButton) {
        emit rightClick();
    }
}
