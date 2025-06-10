#ifndef NONOGRAMALGORITHM_H
#define NONOGRAMALGORITHM_H

#include <vector>

void NonogramCoz(
    const std::vector<std::vector<int>>& rowClues,
    const std::vector<std::vector<int>>& colClues,
    std::vector<std::vector<int>>& outputGrid
);

#endif // NONOGRAMALGORITHM_H
