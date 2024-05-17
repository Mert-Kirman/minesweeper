#ifndef MYCELL_H
#define MYCELL_H

#include <QPushButton>

class MyCell : public QPushButton
{
    Q_OBJECT

public:
    MyCell(QWidget *parent = 0);
};

#endif // MYCELL_H
