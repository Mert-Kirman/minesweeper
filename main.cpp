#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include "mygrid.h"
#include "mycell.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget *cw = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb = new QHBoxLayout;

    MyGrid *grid = new MyGrid();
    int a = 20;
    int b = 20;
    for(int i = 0; i < a * b; i++) {
        int row = i / b;
        int col = i % b;
        MyCell *newCell = new MyCell();
        grid->addWidget(newCell, row, col, 1, 1);
    }

    QLabel *score = new QLabel("Score: 100");
    QPushButton *restart = new QPushButton("Restart");
    QPushButton *hint = new QPushButton("Hint");

    hb->addWidget(score);
    hb->addWidget(restart);
    hb->addWidget(hint);

    vb->addLayout(hb);
    vb->addLayout(grid);

    cw->setLayout(vb);
    cw->setWindowTitle("minesweeper");

    QIcon icon(":/images/cell_images/mine.png");
    cw->setWindowIcon(icon);
    cw->show();

    return app.exec();
}
