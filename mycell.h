#ifndef MYCELL_H
#define MYCELL_H

#include <QPushButton>
#include <QMouseEvent>

class MyCell : public QPushButton
{
    Q_OBJECT

public:
    MyCell(QWidget *parent = 0);
    int cellId;  // An integer value specifying the id of a cell object (its index in the grid object)
    int neighborMineCount;
    bool isRevealed;  // If a cell is revealed, it should be unclickable
    bool isMine;  // States if a cell contains mine
    bool isFlagged;
    bool isSimulationMine;  // When trying to find a hint during simulation, check if this cell is marked as a mine or not

signals:
    void leftClick();
    void rightClick();

protected:
    // When left or right mouse button is clicked, corresponding signal will be sent from the cell object
    void mousePressEvent(QMouseEvent *event);
};

#endif // MYCELL_H
