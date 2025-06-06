#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Box.h"

static const float CELL_GAP = 20.0f;

void GenerateBoxes(int requestedCount, std::vector<Box>& boxes,
    int& outCols, int& outRows);

#endif // GRID_H
