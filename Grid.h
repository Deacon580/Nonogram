#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Box.h"

// CELL_GAP değerini değiştirince border da otomatik güncellenir
static const float CELL_GAP = 20.0f;    // Örneğin 50, 20, 10 vb.

// GenerateBoxes (Grid Hesaplama):
// requestedCount: yerleştirilmek istenen kutu sayısı
// boxes: çıktı vektörü (Box nesneleri) – içeride gridX, gridY atanacak
// outCols, outRows: hesaplanan sütun ve satır sayısını dışarıya verir
void GenerateBoxes(int requestedCount, std::vector<Box>& boxes,
    int& outCols, int& outRows);

#endif // GRID_H
