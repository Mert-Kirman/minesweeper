#ifndef MYGRID_H
#define MYGRID_H

#include <QGridLayout>
#include <QLabel>
#include "mycell.h"

class MyGrid : public QGridLayout
{
    Q_OBJECT

public:
    MyGrid(const int &rowCount, const int &columnCount, const int &mineCount);
    int revealedCellCount;
    int rowCount;
    int columnCount;
    int startMineCount;  // The mine count when the game is first started
    QLabel *scoreLabel;
    bool hintShowed;
    int hintId;  // Id of the cell shown as a hint

public slots:
    // If left mouse button is clicked, the cell will be revealed
    void revealCell();
    // If right mouse button is clicked, the cell will be flagged/unflagged
    void flagCell();
    // Function that restarts the game when the restart button is pressed
    void restart();
    // Function that suggest an unrevealed cell that does not contain a mine at first click. At second click that cell is revealed
    void showHint();

private:
    // Function that returns the number mines surrounding a specified cell
    void findNeighborMineCount(MyCell *currentCell);
    // Function that recursively reveals all adjacent empty cells
    void revealEmptyCells(MyCell *currentCell);
    // A mine is clicked, game ends and all mines are shown
    void gameLost();
    // Function that is called when all cells except mines are revealed (revealedCellCount = rowCount * columnCount - startMineCount)
    void gameWon();
    // Function that returns the Id of a cell that can be hinted
    int findHint();
    // Function that returns the count of unrevealed neighbor cells
    int findUnrevealedCellCount(MyCell *currentCell, int operationId);
    bool gameIsLost;  // Deactivate the hint button
};

#endif // MYGRID_H

