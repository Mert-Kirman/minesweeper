#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include "mygrid.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget *cw = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb = new QHBoxLayout;  // Layout for holding score label, restart and hint buttons

    // Enter row, column and mine count
    int rowCount = 10;
    int columnCount = 10;
    int mineCount = 2;
    MyGrid *grid = new MyGrid(rowCount, columnCount, mineCount);

    QPushButton *restart = new QPushButton("Restart");
    QObject::connect(restart, &QPushButton::clicked, grid, &MyGrid::restart);

    QPushButton *hint = new QPushButton("Hint");

    hb->addWidget(grid->scoreLabel);
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
