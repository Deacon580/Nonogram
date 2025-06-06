#include "Grid.h"
#include <algorithm>
#include <cmath>
#include "raylib.h"

// GenerateBoxes: Kutuları ızgaraya yerleştirir, sadece gridX/gridY atar, center hesaplatılmaz
void GenerateBoxes(int requestedCount, std::vector<Box>& boxes,
    int& outCols, int& outRows)
{
    boxes.clear();
    boxes.reserve(requestedCount);

    // Pencere boyutundan bağımsız, yalnızca count ve CELL_GAP tabanlı hesap:
    // pitch = hücre merkezden merkeze mesafe
    float pitch = GHOST_SIZE.x + CELL_GAP;

    // Önce sütun sayısını bul: ekrana sığabilecek en fazla sütun (maxCols)
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int maxCols = std::max(1, static_cast<int>(std::floor((screenW + CELL_GAP) / pitch)));
    int cols = std::min(requestedCount, maxCols);
    int rows = (requestedCount + cols - 1) / cols; // ceil(requestedCount/cols)

    // Eğer satır sayısı dikeyde taşarsa, satırı düşür, sütunu yeniden hesapla:
    int maxRows = std::max(1, static_cast<int>(std::floor((screenH + CELL_GAP) / pitch)));
    if (rows > maxRows)
    {
        rows = maxRows;
        cols = static_cast<int>(std::ceil(requestedCount / static_cast<float>(rows)));
        cols = static_cast<int>(ClampF(static_cast<float>(cols), 1.0f, static_cast<float>(maxCols)));
        rows = (requestedCount + cols - 1) / cols;
    }

    // Şimdi cols×rows, kullanılan ızgara boyutu:
    outCols = cols;
    outRows = rows;

    // Her bir kutunun gridX/gridY’si:
    // Son satır eksik kutu olabilir, ama gridX/gridY sıralı atanır
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
