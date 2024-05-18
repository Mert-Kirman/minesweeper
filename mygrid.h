#ifndef MYGRID_H
#define MYGRID_H

#include <QGridLayout>
#include <QLabel>

class MyGrid : public QGridLayout
{
    Q_OBJECT

public:
    MyGrid(const int &rowCount, const int &columnCount, const int &mineCount);
    int revealedCellCount;
    QLabel *scoreLabel;

public slots:
    // If left mouse button is clicked, the cell will be revealed
    void revealCell();
    // If right mouse button is clicked, the cell will be flagged/unflagged
    void flagCell();
};

#endif // MYGRID_H

