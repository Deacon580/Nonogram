#include "nonogramAlgorithm.h"
#include <algorithm>

// Nonogram cozucu fonksiyon (sadece tek ipucu icin)
void NonogramCoz(
    const std::vector<int>& satirIpuclari,
    const std::vector<int>& sutunIpuclari,
    std::vector<std::vector<int>>& ciktiTablo
) {
    int satirSayisi = satirIpuclari.size();
    int sutunSayisi = sutunIpuclari.size();

    // Cozum tablosunu sifirla (tum hucreler bosta)
    ciktiTablo.assign(satirSayisi, std::vector<int>(sutunSayisi, 0));

    // SATIRLARIN COZUMU
    for (int y = 0; y < satirSayisi; ++y) {
        int ipucu = satirIpuclari[y];
        if (ipucu == 0) continue;

        int baslangicMin = 0;
        int baslangicMax = sutunSayisi - ipucu;
        std::vector<std::vector<int>> olasiliklar;

        for (int baslangic = baslangicMin; baslangic <= baslangicMax; ++baslangic) {
            std::vector<int> satir(sutunSayisi, 0);
            for (int i = 0; i < ipucu; ++i)
                satir[baslangic + i] = 1;
            olasiliklar.push_back(satir);
        }

        for (int x = 0; x < sutunSayisi; ++x) {
            bool hepsiBir = true;
            for (const auto& secenek : olasiliklar) {
                if (secenek[x] != 1) {
                    hepsiBir = false;
                    break;
                }
            }
            if (hepsiBir)
                ciktiTablo[y][x] = 1;
        }
    }

    // SUTUNLARIN COZUMU
    for (int x = 0; x < sutunSayisi; ++x) {
        int ipucu = sutunIpuclari[x];
        if (ipucu == 0) continue;

        int baslangicMin = 0;
        int baslangicMax = satirSayisi - ipucu;
        std::vector<std::vector<int>> olasiliklar;

        for (int baslangic = baslangicMin; baslangic <= baslangicMax; ++baslangic) {
            std::vector<int> sutun(satirSayisi, 0);
            for (int i = 0; i < ipucu; ++i)
                sutun[baslangic + i] = 1;
            olasiliklar.push_back(sutun);
        }

        for (int y = 0; y < satirSayisi; ++y) {
            bool hepsiBir = true;
            for (const auto& secenek : olasiliklar) {
                if (secenek[y] != 1) {
                    hepsiBir = false;
                    break;
                }
            }
            if (hepsiBir)
                ciktiTablo[y][x] = 1;
        }
    }
}
