#include "Grid.h"
#include <algorithm>
#include <cmath>
#include "raylib.h"

void GenerateBoxes(int requestedCount, std::vector<Box>& boxes,
    int& outCols, int& outRows)
{
    boxes.clear();
    boxes.reserve(requestedCount);

    float pitch = GHOST_SIZE.x + CELL_GAP;

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int maxCols = std::max(1, static_cast<int>(std::floor((screenW + CELL_GAP) / pitch)));
    int cols = std::min(requestedCount, maxCols);
    int rows = (requestedCount + cols - 1) / cols; 

    int maxRows = std::max(1, static_cast<int>(std::floor((screenH + CELL_GAP) / pitch)));
    if (rows > maxRows)
    {
        rows = maxRows;
        cols = static_cast<int>(std::ceil(requestedCount / static_cast<float>(rows)));
        cols = static_cast<int>(ClampF(static_cast<float>(cols), 1.0f, static_cast<float>(maxCols)));
        rows = (requestedCount + cols - 1) / cols;
    }

    outCols = cols;
    outRows = rows;

   
    int placed = 0;
    for (int r = 0; r < rows; ++r)
    {
        int remaining = requestedCount - placed;
        int rowBoxes = std::min(cols, remaining);
        for (int c = 0; c < rowBoxes; ++c)
        {
            Box b;
            b.gridX = c;
            b.gridY = r;
            InitBox(b);
            boxes.push_back(b);
            placed++;
        }
    }
}
