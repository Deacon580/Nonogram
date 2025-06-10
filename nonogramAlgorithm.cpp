#include "nonogramAlgorithm.h"
#include <algorithm>

// Yardımcı: Belirtilen uzunlukta ve toplam genişlikte tüm geçerli satır/sütun dizilerini üret
void GenerateLinePermutations(int length, const std::vector<int>& clues, int index,
    std::vector<int>& current, std::vector<std::vector<int>>& results) {
    if (index >= clues.size()) {
        // Geriye kalan boşlukları ekle
        while (current.size() < length)
            current.push_back(0);
        results.push_back(current);
        // Geri al
        while (!current.empty() && current.back() == 0)
            current.pop_back();
        return;
    }

    int remainingLength = length - current.size();
    int minRequired = clues[index] + (clues.size() - index - 1); // blok + en az 1 boşluk

    if (remainingLength < minRequired)
        return;

    // Boşluk koy ve sonra bloğu yerleştir
    int maxLeadingZeros = remainingLength - minRequired;
    for (int zeros = 0; zeros <= maxLeadingZeros; ++zeros) {
        for (int z = 0; z < zeros; ++z)
            current.push_back(0);
        for (int f = 0; f < clues[index]; ++f)
            current.push_back(1);
        if (index < clues.size() - 1)
            current.push_back(0); // sonraki bloğa boşluk
        GenerateLinePermutations(length, clues, index + 1, current, results);
        // Geri al
        while (!current.empty() && current.back() == 0)
            current.pop_back();
        for (int f = 0; f < clues[index]; ++f)
            current.pop_back();
    }
}

void NonogramCoz(
    const std::vector<std::vector<int>>& satirIpuclari,
    const std::vector<std::vector<int>>& sutunIpuclari,
    std::vector<std::vector<int>>& ciktiTablo
) {
    int satirSayisi = satirIpuclari.size();
    int sutunSayisi = sutunIpuclari.size();

    ciktiTablo.assign(satirSayisi, std::vector<int>(sutunSayisi, -1)); // -1 = bilinmiyor

    // Satırlar
    for (int y = 0; y < satirSayisi; ++y) {
        std::vector<std::vector<int>> permutations;
        std::vector<int> current;
        GenerateLinePermutations(sutunSayisi, satirIpuclari[y], 0, current, permutations);

        for (int x = 0; x < sutunSayisi; ++x) {
            bool alwaysFilled = true;
            bool alwaysEmpty = true;
            for (const auto& p : permutations) {
                if (p[x] != 1) alwaysFilled = false;
                if (p[x] != 0) alwaysEmpty = false;
            }
            if (alwaysFilled)
                ciktiTablo[y][x] = 1;
            else if (alwaysEmpty)
                ciktiTablo[y][x] = 0;
        }
    }

    // Sütunlar
    for (int x = 0; x < sutunSayisi; ++x) {
        std::vector<std::vector<int>> permutations;
        std::vector<int> current;
        GenerateLinePermutations(satirSayisi, sutunIpuclari[x], 0, current, permutations);

        for (int y = 0; y < satirSayisi; ++y) {
            bool alwaysFilled = true;
            bool alwaysEmpty = true;
            for (const auto& p : permutations) {
                if (p[y] != 1) alwaysFilled = false;
                if (p[y] != 0) alwaysEmpty = false;
            }
            if (alwaysFilled)
                ciktiTablo[y][x] = 1;
            else if (alwaysEmpty)
                ciktiTablo[y][x] = 0;
        }
    }
}
