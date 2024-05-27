#include <QMessageBox>
#include "mygrid.h"
#include "mycell.h"

MyGrid::MyGrid(const int &rowCount, const int &columnCount, const int &mineCount) : QGridLayout() {
    setSpacing(0);
    setContentsMargins(0, 0, 0, 0); // No margin around the layout

    this->rowCount = rowCount;
    this->columnCount = columnCount;
    this->startMineCount = mineCount;
    this->hintShowed = false;
    this->hintId = -1;

    // Create the grid with the specified size
    for(int i = 0; i < rowCount * columnCount; i++) {
        int rowIndex = i / columnCount;
        int colIndex = i % columnCount;

        MyCell *newCell = new MyCell();
        // Give the cell an id
        newCell->cellId = i;
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
    // After any reveal, the green hint image should disappear
    if(this->hintShowed) {
        this->hintShowed = false;
        MyCell *hintedCell = qobject_cast<MyCell*>(this->itemAt(this->hintId)->widget());
        QIcon icon(":/images/cell_images/empty.png");
        hintedCell->setIcon(icon);
    }

    MyCell *currentCell = qobject_cast<MyCell*>(sender());

    if(currentCell->isMine) {  // Game over since mine is revealed
        currentCell->isRevealed = true;

        QIcon icon(":/images/cell_images/mine.png");
        currentCell->setIcon(icon);
        gameLost();
    }
    else {  // Reveal the mine count surrounding the cell
        this->findNeighborMineCount(currentCell);

        // Choose image name according to the neighbor mine count
        QString image_name;
        switch (currentCell->neighborMineCount) {
            case 0:
                revealEmptyCells(currentCell);
                break;
            case 1:
                image_name = ":/images/cell_images/1.png";
                break;
            case 2:
                image_name = ":/images/cell_images/2.png";
                break;
            case 3:
                image_name = ":/images/cell_images/3.png";
                break;
            case 4:
                image_name = ":/images/cell_images/4.png";
                break;
            case 5:
                image_name = ":/images/cell_images/5.png";
                break;
            case 6:
                image_name = ":/images/cell_images/6.png";
                break;
            case 7:
                image_name = ":/images/cell_images/7.png";
                break;
            case 8:
                image_name = ":/images/cell_images/8.png";
                break;
            default:
                break;
        }

        if(currentCell->neighborMineCount > 0) {
            currentCell->isRevealed = true;
            this->revealedCellCount++;

            QIcon icon(image_name);
            currentCell->setIcon(icon);

            // If a cell is revealed, it cannot be clicked again
            disconnect(currentCell, &MyCell::leftClick, this, &MyGrid::revealCell);
            disconnect(currentCell, &MyCell::rightClick, this, &MyGrid::flagCell);
        }

        this->scoreLabel->setText("Score: " + QString::number(revealedCellCount));

        // If all cells are revealed, game is won
        if(this->revealedCellCount == this->rowCount * this->columnCount - this->startMineCount) {
            gameWon();
        }
    }
}

void MyGrid::flagCell() {
    MyCell *currentCell = qobject_cast<MyCell*>(sender());

    if(currentCell->isFlagged) {
        // Unflag this cell
        currentCell->isFlagged = false;

        // If a cell is unflagged, it can be left clicked
        connect(currentCell, &MyCell::leftClick, this, &MyGrid::revealCell);

        // Set the default unrevealed cell icon
        QIcon icon(":/images/cell_images/empty.png");
        currentCell->setIcon(icon);
    }
    else {
        // Reset the hint state if the flagged current cell is also the hint that was suggested previously
        if(currentCell->cellId == this->hintId && this->hintShowed) {
            this->hintShowed = false;
            this->hintId = -1;
        }

        currentCell->isFlagged = true;

        // If a cell is flagged, it cannot be left clicked
        disconnect(currentCell, &MyCell::leftClick, this, &MyGrid::revealCell);

        // Set flag icon
        QIcon icon(":/images/cell_images/flag.png");
        currentCell->setIcon(icon);
    }
}

void MyGrid::findNeighborMineCount(MyCell *currentCell) {
    int currentCellId = currentCell->cellId;
    int currentCellRowIndex = currentCellId / columnCount;
    int currentCellColumnIndex = currentCellId % columnCount;
    int mineCount = 0;

    // Directions (Will be used to check boundaries)
    int up = currentCellRowIndex - 1;
    int right = currentCellColumnIndex + 1;
    int down = currentCellRowIndex + 1;
    int left = currentCellColumnIndex - 1;

    // Indexes(Id) of neighbor cells to check (The following will be used only if the corresponding directions are within row and column boundaries)
    int upNeighborIndex = currentCellId - this->columnCount;
    int rightNeighborIndex = currentCellId + 1;
    int downNeighborIndex = currentCellId + this->columnCount;
    int leftNeighborIndex = currentCellId - 1;
    int upRigthNeighborIndex =  upNeighborIndex + 1;
    int downRightNeighborIndex = downNeighborIndex + 1;
    int upLeftNeighborIndex = upNeighborIndex - 1;
    int downLeftNeighborIndex = downNeighborIndex - 1;

    // Check neighbor cells
    MyCell *cell;
    if(up >= 0) {
        cell = qobject_cast<MyCell*>(this->itemAt(upNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(right < this->columnCount) {
        cell = qobject_cast<MyCell*>(this->itemAt(rightNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(down < this->rowCount) {
        cell = qobject_cast<MyCell*>(this->itemAt(downNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(left >= 0) {
        cell = qobject_cast<MyCell*>(this->itemAt(leftNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(up >= 0 && right < this->columnCount) {
        cell = qobject_cast<MyCell*>(this->itemAt(upRigthNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(down < this->rowCount && right < this->columnCount) {
        cell = qobject_cast<MyCell*>(this->itemAt(downRightNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(up >= 0 && left >= 0) {
        cell = qobject_cast<MyCell*>(this->itemAt(upLeftNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    if(down < this->rowCount && left >= 0) {
        cell = qobject_cast<MyCell*>(this->itemAt(downLeftNeighborIndex)->widget());
        if(cell->isMine) {
            mineCount++;
        }
    }

    currentCell->neighborMineCount = mineCount;
}

void MyGrid::revealEmptyCells(MyCell *currentCell) {
    // Base case: current cell should be an empty cell
    findNeighborMineCount(currentCell);
    if(currentCell->neighborMineCount != 0 || currentCell->isMine || currentCell->isRevealed) {
        if(currentCell->neighborMineCount != 0 && !currentCell->isMine && !currentCell->isRevealed) {
            // Reveal this 'border' neighbor cell
            currentCell->leftClick();
        }
        return;
    }

    // Reveal current cell
    currentCell->isRevealed = true;
    this->revealedCellCount++;
    QString imageName = ":/images/cell_images/0.png";
    QIcon icon(imageName);
    currentCell->setIcon(icon);
    disconnect(currentCell, &MyCell::leftClick, this, &MyGrid::revealCell);
    disconnect(currentCell, &MyCell::rightClick, this, &MyGrid::flagCell);

    // Look for empty neighbor cells
    int currentCellId = currentCell->cellId;
    int currentCellRowIndex = currentCellId / columnCount;
    int currentCellColumnIndex = currentCellId % columnCount;

    // Directions (Will be used to check boundaries)
    int up = currentCellRowIndex - 1;
    int right = currentCellColumnIndex + 1;
    int down = currentCellRowIndex + 1;
    int left = currentCellColumnIndex - 1;

    // Indexes(Id) of neighbor cells to check (The following will be used only if the corresponding directions are within row and column boundaries)
    int upNeighborIndex = currentCellId - this->columnCount;
    int rightNeighborIndex = currentCellId + 1;
    int downNeighborIndex = currentCellId + this->columnCount;
    int leftNeighborIndex = currentCellId - 1;
    int upRigthNeighborIndex =  upNeighborIndex + 1;
    int downRightNeighborIndex = downNeighborIndex + 1;
    int upLeftNeighborIndex = upNeighborIndex - 1;
    int downLeftNeighborIndex = downNeighborIndex - 1;

    // Check neighbor cells
    MyCell *neighborCell;
    if(up >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(rightNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(down < this->rowCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(leftNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(up >= 0 && right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upRigthNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(down < this->rowCount && right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downRightNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(up >= 0 && left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upLeftNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }

    if(down < this->rowCount && left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downLeftNeighborIndex)->widget());
        revealEmptyCells(neighborCell);
    }
}

void MyGrid::gameLost() {
    QIcon mineIcon(":/images/cell_images/mine.png");
    QIcon wrongFlag(":/images/cell_images/wrong-flag.png");

    MyCell *cell;
    for(int i = 0; i < this->rowCount * this->columnCount; i++) {
        cell = qobject_cast<MyCell*>(this->itemAt(i)->widget());

        // Disconnect cell signals to grid slots
        disconnect(cell, &MyCell::leftClick, this, &MyGrid::revealCell);
        disconnect(cell, &MyCell::rightClick, this, &MyGrid::flagCell);

        if(cell->isMine && !cell->isFlagged) {
            cell->setIcon(mineIcon);
        }
        if(!cell->isMine && cell->isFlagged) {
            cell->setIcon(wrongFlag);
        }
    }

    QMessageBox msg;
    msg.setText("You lose!");
    msg.exec();
}

void MyGrid::gameWon() {
    QIcon mineIcon(":/images/cell_images/mine.png");

    MyCell *cell;
    for(int i = 0; i < this->rowCount * this->columnCount; i++) {
        cell = qobject_cast<MyCell*>(this->itemAt(i)->widget());

        // Disconnect cell signals to grid slots
        disconnect(cell, &MyCell::leftClick, this, &MyGrid::revealCell);
        disconnect(cell, &MyCell::rightClick, this, &MyGrid::flagCell);

        // Show mines that were not flagged by the player
        if(cell->isMine && !cell->isFlagged) {
            cell->setIcon(mineIcon);
        }
    }

    QMessageBox msg;
    msg.setText("You win!");
    msg.exec();
}

void MyGrid::restart() {
    // Set score to zero
    this->revealedCellCount = 0;
    this->scoreLabel->setText("Score: " + QString::number(revealedCellCount));
    this->hintShowed = false;

    // Set all cells as unrevealed
    MyCell *cell;
    QIcon icon(":/images/cell_images/empty.png");
    for(int i = 0; i < rowCount * columnCount; i++) {
        cell = qobject_cast<MyCell*>(this->itemAt(i)->widget());
        cell->isMine = false;
        cell->isFlagged = false;
        cell->isRevealed = false;
        cell->neighborMineCount = -1;
        cell->isSimulationMine = false;
        cell->setIcon(icon);

        // Connect cell signals to grid slots
        connect(cell, &MyCell::leftClick, this, &MyGrid::revealCell);
        connect(cell, &MyCell::rightClick, this, &MyGrid::flagCell);
    }

    // Place mines in random cells
    int currentMineCount = 0;
    while(currentMineCount != this->startMineCount) {
        int newMineIndex = rand() % this->count();
        // Type cast the item to MyCell object in order to be able to reach its members
        MyCell *cell = qobject_cast<MyCell*>(this->itemAt(newMineIndex)->widget());
        if(cell->isMine == false) {
            cell->isMine = true;
            currentMineCount++;
        }
    }
}

void MyGrid::showHint() {
    // If hint button is clicked a second time, reveal the hint
    if(this->hintShowed) {
        MyCell *hintedCell = qobject_cast<MyCell*>(this->itemAt(this->hintId)->widget());
        hintedCell->leftClick();

        this->hintShowed = false;

        return;
    }

    // Hint is clicked for the first time
    this->hintId = findHint();

    // If a hint is found, make it green
    if(this->hintId != -1) {
        MyCell *hintedCell = qobject_cast<MyCell*>(this->itemAt(this->hintId)->widget());

        // If this cell was flagged before, make it left clickable again
        if(hintedCell->isFlagged) {
            hintedCell->isFlagged = false;
            connect(hintedCell, &MyCell::leftClick, this, &MyGrid::revealCell);
        }

        QIcon hintIcon(":/images/cell_images/hint.png");
        hintedCell->setIcon(hintIcon);
        this->hintShowed = true;
    }
}

int MyGrid::findHint() {
    MyCell *currentCell;

    // Find revealed cells which have at least 1 mine in their neighbors
    for(int i = 0; i < rowCount * columnCount; i++) {
        currentCell = qobject_cast<MyCell*>(this->itemAt(i)->widget());

        // Pass any unrevealed cells
        if(!currentCell->isRevealed || currentCell->neighborMineCount < 1) {
            continue;
        }

        // Look for empty neighbor cells
        int currentCellId = currentCell->cellId;
        int currentCellRowIndex = currentCellId / columnCount;
        int currentCellColumnIndex = currentCellId % columnCount;

        // Directions (Will be used to check boundaries)
        int up = currentCellRowIndex - 1;
        int right = currentCellColumnIndex + 1;
        int down = currentCellRowIndex + 1;
        int left = currentCellColumnIndex - 1;

        // Indexes(Id) of neighbor cells to check (The following will be used only if the corresponding directions are within row and column boundaries)
        int upNeighborIndex = currentCellId - this->columnCount;
        int rightNeighborIndex = currentCellId + 1;
        int downNeighborIndex = currentCellId + this->columnCount;
        int leftNeighborIndex = currentCellId - 1;
        int upRigthNeighborIndex =  upNeighborIndex + 1;
        int downRightNeighborIndex = downNeighborIndex + 1;
        int upLeftNeighborIndex = upNeighborIndex - 1;
        int downLeftNeighborIndex = downNeighborIndex - 1;

        int unrevealedCellCount = findUnrevealedCellCount(currentCell, 0);
        int simulationMineCount = findUnrevealedCellCount(currentCell, 1);

        // Can suggest at least 1 candidate cell
        if(currentCell->neighborMineCount - simulationMineCount == 0 && unrevealedCellCount - simulationMineCount > 0) {
            // Check neighbor cells
            MyCell *neighborCell;
            if(up >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(rightNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(down < this->rowCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(leftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(up >= 0 && right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upRigthNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(down < this->rowCount && right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downRightNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(up >= 0 && left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upLeftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }

            if(down < this->rowCount && left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downLeftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    return neighborCell->cellId;
                }
            }
        }

        if(currentCell->neighborMineCount == unrevealedCellCount) {
            // Check neighbor cells
            MyCell *neighborCell;
            if(up >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(rightNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(down < this->rowCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(leftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(up >= 0 && right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upRigthNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(down < this->rowCount && right < this->columnCount) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downRightNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(up >= 0 && left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(upLeftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }

            if(down < this->rowCount && left >= 0) {
                neighborCell = qobject_cast<MyCell*>(this->itemAt(downLeftNeighborIndex)->widget());
                if(!neighborCell->isRevealed && !neighborCell->isSimulationMine) {
                    neighborCell->isSimulationMine = true;
                    i = 0;
                }
            }
        }
    }

    return -1;
}

int MyGrid::findUnrevealedCellCount(MyCell *currentCell, int operationId) {
    // If operation id is 0, unrevealed neighbor cells will be counted; if id is 1, count of neighbors that contain a mine will be counted.
    int count = 0;

    // Look for empty neighbor cells
    int currentCellId = currentCell->cellId;
    int currentCellRowIndex = currentCellId / columnCount;
    int currentCellColumnIndex = currentCellId % columnCount;

    // Directions (Will be used to check boundaries)
    int up = currentCellRowIndex - 1;
    int right = currentCellColumnIndex + 1;
    int down = currentCellRowIndex + 1;
    int left = currentCellColumnIndex - 1;

    // Indexes(Id) of neighbor cells to check (The following will be used only if the corresponding directions are within row and column boundaries)
    int upNeighborIndex = currentCellId - this->columnCount;
    int rightNeighborIndex = currentCellId + 1;
    int downNeighborIndex = currentCellId + this->columnCount;
    int leftNeighborIndex = currentCellId - 1;
    int upRigthNeighborIndex =  upNeighborIndex + 1;
    int downRightNeighborIndex = downNeighborIndex + 1;
    int upLeftNeighborIndex = upNeighborIndex - 1;
    int downLeftNeighborIndex = downNeighborIndex - 1;

    // Check neighbor cells
    MyCell *neighborCell;
    if(up >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(rightNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(down < this->rowCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(leftNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(up >= 0 && right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upRigthNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(down < this->rowCount && right < this->columnCount) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downRightNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(up >= 0 && left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(upLeftNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    if(down < this->rowCount && left >= 0) {
        neighborCell = qobject_cast<MyCell*>(this->itemAt(downLeftNeighborIndex)->widget());
        if(operationId == 0 && !neighborCell->isRevealed) {
            count++;
        }
        else if(operationId == 1 && neighborCell->isSimulationMine) {
            count++;
        }
    }

    return count;
}
