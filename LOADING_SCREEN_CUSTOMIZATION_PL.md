# Dostosowywanie obrazów ekranu ładowania

W niniejszym przewodniku wyjaśniono, w jaki sposób administrator serwera może zastąpić lub rozszerzyć zestaw obrazów używanych na ekranie ładowania teleportu w modzie `DME_Teleport_Overhaul`.

## Jak mod obecnie ładuje obrazy

Tła ekranu ładowania są zdefiniowane bezpośrednio w pliku [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
     „DME_Teleport_Overhaul/data/dme1.edds”,
     „DME_Teleport_Overhaul/data/dme2.edds”,
     „DME_Teleport_Overhaul/data/dme3.edds”
};
```

Za każdym razem, gdy wyświetlany jest ekran ładowania teleportu, mod losowo wybiera jeden z tych plików.

## Opcja 1: Zastąp istniejące 3 obrazy

Jest to najprostsza metoda i nie wymaga żadnych zmian w skryptach.

### Kroki

1. Przygotuj swoje własne obrazy.
2. Przekonwertuj je do formatu `.edds`.
3. Zmień ich nazwy dokładnie na:
    - `dme1.edds`
	- `dme2.edds`
    - `dme3.edds`
4. Zastąp istniejące pliki w [data](data):
    - [data/dme1.edds](data/dme1.edds)
    - [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. W razie potrzeby spakuj ponownie mod PBO.
6. Uruchom ponownie serwer i klienta.

### Wynik

Ekran ładowania będzie działał dokładnie tak samo jak wcześniej, ale teraz będzie używał twoich nowych obrazów.

## Opcja 2: Dodaj więcej niż 3 obrazy

Jeśli chcesz mieć więcej niż trzy domyślne tła, musisz dodać nowe pliki `.edds` i zaktualizować listę tła w skrypcie.

### Kroki

1. Dodaj nowe pliki `.edds` do [data](data), na przykład:
    - `data/my_loading_01.edds`
    - `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Otwórz plik [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Znajdź konstruktor `DME_TeleportLoadingScreen()`.
4. Rozszerz tablicę `m_Backgrounds`.

Przykład:

```c
m_Backgrounds = {
	 „DME_Teleport_Overhaul/data/dme1.edds”,
     „DME_Teleport_Overhaul/data/dme2.edds”,
     „DME_Teleport_Overhaul/data/dme3.edds”,
     „DME_Teleport_Overhaul/data/my_loading_01.edds”,
	 „DME_Teleport_Overhaul/data/my_loading_02.edds”,
     „DME_Teleport_Overhaul/data/my_loading_03.edds”
};
```

5. Spakuj ponownie mod PBO.
6. Uruchom ponownie serwer i klienta.

### Wynik

Ekran ładowania będzie losowo wybierał spośród wszystkich wymienionych obrazów.

## Uwagi dotyczące formatu obrazów

- Ekran ładowania wykorzystuje tekstury `.edds`.
- Umieść wszystkie ścieżki do obrazów w folderze modu.
- Używaj pełnych ścieżek względnych względem modu, takich jak `DME_Teleport_Overhaul/data/yourimage.edds`.
- Unikaj spacji i znaków specjalnych w nazwach plików.
- Zachowaj proporcje obrazu dostosowane do ekranu panoramicznego, aby tło na pełnym ekranie wyglądało schludnie.

## Ważne ograniczenie

W tej chwili obrazy ekranu ładowania nie są odczytywane z pliku konfiguracyjnego JSON.

Oznacza to, że:

- Zastąpienie istniejących 3 obrazów wymaga jedynie wymiany tekstur.
- Dodanie dodatkowych obrazów wymaga niewielkiej edycji skryptu w pliku [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Zalecany sposób postępowania dla gospodarzy

Jeśli chcesz tylko zamienić grafiki, użyj **Opcji 1**.

Jeśli chcesz mieć większy losowy zbiór obrazów, użyj **Opcji 2**.

## Dostosowywanie obrazów menu teleportacji

Karty miejsc docelowych teleportacji w menu F2 używają oddzielnego zestawu obrazów, innego niż ten z ekranu ładowania.

Obrazy te są przypisane do identyfikatorów `Picture` w pliku `teleport_menu_config.json`.

Przykład:

```json
{
    „TeleportName”: „Green Mountain”,
    „TeleportPos”: [3700.51, 0.0, 5981.27],
    „Cost”: 1200,
    „CooldownSec”: 17,
    „Picture”: 1,
    „Marker”: 1
}
```

## Aktualne przypisanie obrazów w menu

Obecne identyfikatory `Picture` są powiązane z następującymi plikami:

- `0` = brak obrazu
- `1` = `gui/assets/airport.edds`
- `2` = `gui/assets/airport2.edds`
- `3` = `gui/assets/baloon.edds`
- `4` = `gui/assets/city.edds`
- `5` = `gui/assets/fishing.edds`
- `6` = `gui/assets/forest.edds`
- `7` = `gui/assets/forest2.edds`
- `8` = `gui/assets/galaxy.edds`
- `9` = `gui/assets/landhouse.edds`
- `10` = `gui/assets/landside.edds`
- `11` = `gui/assets/mountains.edds`
- `12` = `gui/assets/tisy.edds`

## Opcja 1: Zastąp istniejące obrazy menu teleportacji

Jest to najprostsza metoda, jeśli chcesz zachować te same identyfikatory `Picture`.

### Kroki

1. Przygotuj obrazy zastępcze.
2. Przekonwertuj je do formatu `.edds`.
3. Zastąp istniejące pliki w [gui/assets](gui/assets), na przykład:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Zachowaj te same nazwy plików, jeśli nie chcesz zmieniać żadnej konfiguracji JSON.
5. W razie potrzeby spakuj ponownie mod PBO.
6. Uruchom ponownie serwer i klienta.

### Wynik

Wszystkie istniejące wpisy miejsc docelowych będą nadal działać, ale będą wyświetlać Twoje zastępcze grafiki.

## Opcja 2: Dodaj nowe obrazy do menu teleportacji

Jeśli chcesz mieć dodatkowe identyfikatory obrazów poza domyślnym zestawem, musisz dodać nowe pliki i rozszerzyć tablicę obrazów w skrypcie.

### Kroki

1. Dodaj nowe pliki `.edds` do katalogu [gui/assets](gui/assets), na przykład:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Otwórz plik [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Znajdź tablicę `PICTURE_TEXTURES` wewnątrz `DME_Teleport_Constants`.
4. Dodaj nowe tekstury na końcu tablicy.

Przykład:

```c
static const ref array<string> PICTURE_TEXTURES = {
    „”,
    „DME_Teleport_Overhaul/gui/assets/airport.edds”,
    „DME_Teleport_Overhaul/gui/assets/airport2.edds”,
	„DME_Teleport_Overhaul/gui/assets/baloon.edds”,
    „DME_Teleport_Overhaul/gui/assets/city.edds”,
    „DME_Teleport_Overhaul/gui/assets/fishing.edds”,
    „DME_Teleport_Overhaul/gui/assets/forest.edds”,
	„DME_Teleport_Overhaul/gui/assets/forest2.edds”,
    „DME_Teleport_Overhaul/gui/assets/galaxy.edds”,
    „DME_Teleport_Overhaul/gui/assets/landhouse.edds”,
    „DME_Teleport_Overhaul/gui/assets/landside.edds”,
	„DME_Teleport_Overhaul/gui/assets/mountains.edds”,
    „DME_Teleport_Overhaul/gui/assets/tisy.edds”,
    „DME_Teleport_Overhaul/gui/assets/bunker.edds”,
    „DME_Teleport_Overhaul/gui/assets/trader.edds”
};
```

5. Użyj nowego indeksu tablicy w pliku `teleport_menu_config.json`.

Przykład:

```json
{
    „TeleportName”: „Staroye Bunker”,
    „TeleportPos”: [9784.09, 0.0, 5462.18],
	„Cost”: 1800,
    „CooldownSec”: 15,
    „Picture”: 13,
    „Marker”: 1
}
```

6. Spakuj ponownie mod PBO.
7. Uruchom ponownie serwer i klienta.

### Wynik

Menu teleportacji może teraz wyświetlać nowe niestandardowe identyfikatory obrazów.

## Uwagi dotyczące obrazów menu teleportacji

- Obrazy menu wykorzystują tekstury `.edds` znajdujące się w [gui/assets](gui/assets).
- Używaj ścieżek względnych względem modu, takich jak `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Jeśli tylko zastępujesz istniejący plik, nie musisz zmieniać pliku `teleport_menu_config.json`.
- Jeśli dodajesz nowe pliki obrazów, musisz również rozszerzyć `PICTURE_TEXTURES` w [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Możliwe przyszłe ulepszenia

Jeśli chcesz, można to później przenieść do oddzielnej konfiguracji JSON, aby administratorzy serwerów mogli dodawać i usuwać obrazy ekranu ładowania bez edytowania skryptów.
