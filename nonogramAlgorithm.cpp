#include "nonogramAlgorithm.h"


int yyz[5][2] = { { 3, 1 }, { 3, 1 }, { 3, 1 }, { 3, 1 }, { 3, 1 } };
int xzy[5] = { 4, 4, 4, 4, 4 };
int gridy[5][5] = { 0 };

void test(int y[5][2], int x[5], int grid[5][5]) {
    int t = 0;
    int r = 0;

    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < y[k][i]; j++) {
                grid[t][r] = 1;
                t++;
            }
            if (t < 5) {
                grid[t][r] = 0;
                t++;
            }
            else t = 0;
        }
        r++;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << grid[j][i] << ", ";
        }
        std::cout << "\n";
    }
}
