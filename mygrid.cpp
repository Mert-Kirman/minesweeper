#include "mygrid.h"
#include "mycell.h"

MyGrid::MyGrid(const int &rowCount, const int &columnCount, const int &mineCount) : QGridLayout() {
    setSpacing(0);
    setContentsMargins(0, 0, 0, 0); // No margin around the layout

    // Create the grid with the specified size
    for(int i = 0; i < rowCount * columnCount; i++) {
        int rowIndex = i / columnCount;
        int colIndex = i % columnCount;

        MyCell *newCell = new MyCell();
        // Connect cell signals to grid slots
        connect(newCell, &MyCell::leftClick, this, &MyGrid::revealCell);
        connect(newCell, &MyCell::rightClick, this, &MyGrid::flagCell);
        addWidget(newCell, rowIndex, colIndex, 1, 1);
    }

    // Place mines in random cells
    int currentMineCount = 0;
    while(currentMineCount != mineCount) {
        int newMineIndex = rand() % this->count();
        // Type cast the item to MyCell object in order to be able to reach its members
        MyCell *cell = qobject_cast<MyCell*>(this->itemAt(newMineIndex)->widget());
        if(cell->isMine == false) {
            cell->isMine = true;
            currentMineCount++;
        }
    }

    // Score starts with 0
    revealedCellCount = 0;
    scoreLabel = new QLabel("Score: " + QString::number(revealedCellCount));
}

void MyGrid::revealCell() {
    MyCell *currentCell = qobject_cast<MyCell*>(sender());

    if(currentCell->isMine) {
        currentCell->isRevealed = true;

        QIcon icon(":/images/cell_images/mine.png");
        currentCell->setIcon(icon);
        // GAME OVER ----> IMPLEMENT THIS METHOD
    }
    else {
        currentCell->isRevealed = true;
        this->revealedCellCount++;
        this->scoreLabel->setText("Score: " + QString::number(revealedCellCount));
        //findNeighborMineCount();  IMPLEMENT THIS METHOD
        QIcon icon(":/images/cell_images/1.png");
        currentCell->setIcon(icon);
    }

    // If a cell is revealed, it cannot be clicked again
    disconnect(currentCell, &MyCell::leftClick, this, &MyGrid::revealCell);
    disconnect(currentCell, &MyCell::rightClick, this, &MyGrid::flagCell);
}

void MyGrid::flagCell() {
    MyCell *currentCell = qobject_cast<MyCell*>(sender());

    if(currentCell->isFlagged) {
        currentCell->isFlagged = false;  // Unflag this cell
        QIcon icon(":/images/cell_images/empty.png");
        currentCell->setIcon(icon);
    }
    else {
        currentCell->isFlagged = true;
        QIcon icon(":/images/cell_images/flag.png");
        currentCell->setIcon(icon);
    }
}
