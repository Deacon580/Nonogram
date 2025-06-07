
## 1. Başlangıç: Çözüm Tablosunun Hazırlanması

```cpp
int satirSayisi = satirIpuclari.size();
int sutunSayisi = sutunIpuclari.size();
ciktiTablo.assign(satirSayisi, std::vector<int>(sutunSayisi, 0));
```

Ne yapıyor?

Çözüm tablosunu baştan boş (0) olarak oluşturuyor.  
Örnek:

- Satır ipuçları: `[3, 0, 1]`
- Sütun ipuçları: `[1, 0, 3]`

→ Tablo boyutu: `3 x 3`, başlangıç hali:

```
0 0 0
0 0 0
0 0 0
```

---
## 2. Satırların İşlenmesi

```cpp
for (int y = 0; y < satirSayisi; ++y) {
    int ipucu = satirIpuclari[y];
    if (ipucu == 0) continue;
```
###  Ne yapıyor?

Her satır için ipucu varsa işliyor. Örnek: `satirIpuclari = [3, 0, 1]`

- `y = 0`, ipucu `3` → işlem yapılır
- `y = 1`, ipucu `0` → geçilir
- `y = 2`, ipucu `1` → işlem yapılır

---

### 2.1. Olasılıkların Oluşturulması (Satır)

```cpp
int baslangicMax = sutunSayisi - ipucu;
for (int baslangic = 0; baslangic <= baslangicMax; ++baslangic) {
    std::vector<int> satir(sutunSayisi, 0);
    for (int i = 0; i < ipucu; ++i)
        satir[baslangic + i] = 1;
    olasiliklar.push_back(satir);
}
```

### Ne yapıyor?

İpucu kadar `1`’i satır içinde farklı yerlerden başlayarak yerleştiriyor.

#### Örnek:

- Satır uzunluğu: `5`, ipucu: `3`
- Olası yerleşimler:

```
1 1 1 0 0
0 1 1 1 0
0 0 1 1 1
```

Toplam olasılık sayısı: `sutunSayisi - ipucu + 1 = 3`

---

### 2.2. Kesin Hücrelerin Belirlenmesi

```cpp
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
```

###  Ne yapıyor?

Her olasılıkta aynı hücre 1 mi diye kontrol eder.

#### Örnek:

Olasılıklar:

```
1 1 1 0 0
0 1 1 1 0
0 0 1 1 1
```

- Pozisyon 0: [1, 0, 0] → **değil**
- Pozisyon 1: [1, 1, 0] → **değil**
- Pozisyon 2: [1, 1, 1] → + hepsi 1  
    O halde `satır[2]` kesinlikle dolu (`1`)

---

## 3. Sütunların İşlenmesi

Kod yapısı aynı, ama satır yerine sütun üzerinde çalışıyor:

```cpp
for (int x = 0; x < sutunSayisi; ++x) {
    int ipucu = sutunIpuclari[x];
    if (ipucu == 0) continue;

    int baslangicMax = satirSayisi - ipucu;
    std::vector<std::vector<int>> olasiliklar;

    for (int baslangic = 0; baslangic <= baslangicMax; ++baslangic) {
        std::vector<int> sutun(satirSayisi, 0);
        for (int i = 0; i < ipucu; ++i)
            sutun[baslangic + i] = 1;
        olasiliklar.push_back(sutun);
    }
```

### Örnek:

- Satır sayısı: `5`, ipucu: `2`
- Olasılıklar:

```
1
1
0
0
0

0
1
1
0
0

0
0
1
1
0

0
0
0
1
1
```

---
### 3.2. Kesinlik Kontrolü

```cpp
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
```
### Örnek:
Eğer 4 olasılık varsa ve sadece 3. satırda (y = 2) hepsi 1 ise, orası kesin dolu (`1`) olarak işaretlenir.

---
## Sonuç
Bu işlemler sonunda `ciktiTablo` şu şekilde bir hal alabilir (örnek olarak):
```
0 0 1 0 0
0 0 0 0 0
1 0 0 0 0
```
Bu tablo, sadece **kesinlikle dolu olduğu anlaşılan hücreleri** gösterir. Diğer hücreler hakkında bilgi yetersizse 0 kalır.

EKLENECEK
---
