# Přizpůsobení obrázků načítací obrazovky

Tato příručka vysvětluje, jak může správce serveru nahradit nebo rozšířit obrázky používané načítací obrazovkou teleportu v modu `DME_Teleport_Overhaul`.

## Jak mod aktuálně načítá obrázky

Pozadí načítací obrazovky je definováno přímo v [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Pokaždé, když se zobrazí načítací obrazovka teleportu, mod náhodně vybere jeden z těchto souborů.

## Možnost 1: Nahradit stávající 3 obrázky

Toto je nejjednodušší metoda a nevyžaduje žádné úpravy skriptů.

### Kroky

1. Připravte své vlastní obrázky.
2. Převeďte je do formátu `.edds`.
3. Přejmenujte je přesně na:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Nahraďte stávající soubory v [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. V případě potřeby znovu zabalte PBO modu.
6. Restartujte server i klienta.

### Výsledek

Načítací obrazovka bude fungovat stejně jako dříve, ale nyní bude používat vaše nové obrázky.

## Možnost 2: Přidat více než 3 obrázky

Pokud chcete více než tři výchozí pozadí, musíte přidat nové `.edds` soubory a upravit seznam pozadí ve skriptu.

### Kroky

1. Přidejte nové `.edds` soubory do [data](data), například:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Otevřete [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Najděte konstruktor `DME_TeleportLoadingScreen()`.
4. Rozšiřte pole `m_Backgrounds`.

Příklad:

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds",
	 "DME_Teleport_Overhaul/data/my_loading_01.edds",
	 "DME_Teleport_Overhaul/data/my_loading_02.edds",
	 "DME_Teleport_Overhaul/data/my_loading_03.edds"
};
```

5. Znovu zabalte PBO modu.
6. Restartujte server i klienta.

### Výsledek

Načítací obrazovka bude náhodně vybírat ze všech uvedených obrázků.

## Poznámky k formátu obrázků

- Načítací obrazovka používá textury `.edds`.
- Udržujte všechny cesty k obrázkům uvnitř složky modu.
- Používejte plné cesty relativní k modu, například `DME_Teleport_Overhaul/data/yourimage.edds`.
- Vyhněte se mezerám a speciálním znakům v názvech souborů.
- Zachovejte širokoúhlý poměr stran, aby pozadí na celé obrazovce vypadalo dobře.

## Důležité omezení

V tuto chvíli se obrázky načítací obrazovky nenačítají z JSON konfigurace.

To znamená:

- Nahrazení stávajících 3 obrázků vyžaduje pouze výměnu textur.
- Přidání dalších obrázků vyžaduje malou úpravu skriptu v [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Doporučený postup pro hosty

Pokud chcete jen vyměnit grafiku, použijte **Možnost 1**.

Pokud chcete větší náhodný výběr obrázků, použijte **Možnost 2**.

## Přizpůsobení obrázků teleport menu

Karty destinací v menu F2 používají samostatnou sadu obrázků oddělenou od načítací obrazovky.

Tyto obrázky jsou mapovány pomocí `Picture` ID v `teleport_menu_config.json`.

Příklad:

```json
{
	"TeleportName": "Green Mountain",
	"TeleportPos": [3700.51, 0.0, 5981.27],
	"Cost": 1200,
	"CooldownSec": 17,
	"Picture": 1,
	"Marker": 1
}
```

## Aktuální mapování obrázků menu

Aktuální `Picture` ID jsou propojena s těmito soubory:

- `0` = bez obrázku
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

## Možnost 1: Nahradit stávající obrázky teleport menu

Toto je nejjednodušší metoda, pokud chcete zachovat stejná `Picture` ID.

### Kroky

1. Připravte náhradní obrázky.
2. Převeďte je do `.edds`.
3. Nahraďte stávající soubory v [gui/assets](gui/assets), například:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Zachovejte stejné názvy souborů, pokud nechcete měnit žádnou JSON konfiguraci.
5. V případě potřeby znovu zabalte PBO modu.
6. Restartujte server i klienta.

### Výsledek

Všechny stávající destinace budou dál fungovat, ale budou zobrazovat vaše náhradní obrázky.

## Možnost 2: Přidat nové obrázky do teleport menu

Pokud chcete další ID obrázků nad rámec výchozí sady, musíte přidat nové soubory a rozšířit pole obrázků ve skriptu.

### Kroky

1. Přidejte nové `.edds` soubory do [gui/assets](gui/assets), například:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Otevřete [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Najděte pole `PICTURE_TEXTURES` uvnitř `DME_Teleport_Constants`.
4. Přidejte nové textury na konec pole.

Příklad:

```c
static const ref array<string> PICTURE_TEXTURES = {
	"",
	"DME_Teleport_Overhaul/gui/assets/airport.edds",
	"DME_Teleport_Overhaul/gui/assets/airport2.edds",
	"DME_Teleport_Overhaul/gui/assets/baloon.edds",
	"DME_Teleport_Overhaul/gui/assets/city.edds",
	"DME_Teleport_Overhaul/gui/assets/fishing.edds",
	"DME_Teleport_Overhaul/gui/assets/forest.edds",
	"DME_Teleport_Overhaul/gui/assets/forest2.edds",
	"DME_Teleport_Overhaul/gui/assets/galaxy.edds",
	"DME_Teleport_Overhaul/gui/assets/landhouse.edds",
	"DME_Teleport_Overhaul/gui/assets/landside.edds",
	"DME_Teleport_Overhaul/gui/assets/mountains.edds",
	"DME_Teleport_Overhaul/gui/assets/tisy.edds",
	"DME_Teleport_Overhaul/gui/assets/bunker.edds",
	"DME_Teleport_Overhaul/gui/assets/trader.edds"
};
```

5. Použijte nový index pole v `teleport_menu_config.json`.

Příklad:

```json
{
	"TeleportName": "Staroye Bunker",
	"TeleportPos": [9784.09, 0.0, 5462.18],
	"Cost": 1800,
	"CooldownSec": 15,
	"Picture": 13,
	"Marker": 1
}
```

6. Znovu zabalte PBO modu.
7. Restartujte server i klienta.

### Výsledek

Teleport menu nyní může zobrazovat nové vlastní ID obrázků.

## Poznámky k obrázkům teleport menu

- Obrázky menu používají `.edds` textury v [gui/assets](gui/assets).
- Používejte cesty relativní k modu, například `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Pokud pouze nahrazujete existující soubor, nemusíte měnit `teleport_menu_config.json`.
- Pokud přidáváte nové soubory obrázků, musíte také rozšířit `PICTURE_TEXTURES` v [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Možné budoucí vylepšení

Pokud budete chtít, lze to později přesunout do samostatné JSON konfigurace, aby hosté serverů mohli přidávat a odebírat obrázky načítací obrazovky bez úprav skriptů.