#include "mygrid.h"

MyGrid::MyGrid() : QGridLayout() {
    setSpacing(0);
    setContentsMargins(0, 0, 0, 0); // No margin around the layout
}
