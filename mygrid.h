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
    QLabel *scoreLabel;

public slots:
    // If left mouse button is clicked, the cell will be revealed
    void revealCell();
    // If right mouse button is clicked, the cell will be flagged/unflagged
    void flagCell();

private:
    // Function that returns the number mines surrounding a specified cell
    void findNeighborMineCount(MyCell *currentCell);
};

#endif // MYGRID_H

