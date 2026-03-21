# Anpassung der Ladebildschirm-Bilder

Diese Anleitung erklärt, wie ein Server-Host die Bilder ersetzen oder erweitern kann, die vom Teleport-Ladebildschirm in `DME_Teleport_Overhaul` verwendet werden.

## Wie der Mod die Bilder aktuell lädt

Die Hintergründe des Ladebildschirms sind direkt in [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) definiert:

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Jedes Mal, wenn der Teleport-Ladebildschirm angezeigt wird, wählt der Mod zufällig eine dieser Dateien aus.

## Option 1: Die vorhandenen 3 Bilder ersetzen

Dies ist die einfachste Methode und erfordert keine Script-Änderungen.

### Schritte

1. Bereite deine eigenen Bilder vor.
2. Konvertiere sie in das Format `.edds`.
3. Benenne sie exakt um in:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Ersetze die vorhandenen Dateien in [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. Packe den Mod-PBO bei Bedarf neu.
6. Starte Server und Client neu.

### Ergebnis

Der Ladebildschirm funktioniert weiterhin wie zuvor, verwendet nun aber deine neuen Bilder.

## Option 2: Mehr als 3 Bilder hinzufügen

Wenn du mehr als die drei Standard-Hintergründe verwenden möchtest, musst du neue `.edds`-Dateien hinzufügen und die Hintergrundliste im Script erweitern.

### Schritte

1. Füge deine neuen `.edds`-Dateien in [data](data) hinzu, zum Beispiel:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Öffne [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Suche den Konstruktor `DME_TeleportLoadingScreen()`.
4. Erweitere das Array `m_Backgrounds`.

Beispiel:

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

5. Packe den Mod-PBO neu.
6. Starte Server und Client neu.

### Ergebnis

Der Ladebildschirm wählt dann zufällig aus allen aufgeführten Bildern aus.

## Hinweise zum Bildformat

- Der Ladebildschirm verwendet `.edds`-Texturen.
- Halte alle Bildpfade innerhalb des Mod-Ordners.
- Verwende vollständige mod-relative Pfade wie `DME_Teleport_Overhaul/data/yourimage.edds`.
- Vermeide Leerzeichen und Sonderzeichen in Dateinamen.
- Verwende ein Breitbild-Seitenverhältnis, damit der Vollbild-Hintergrund sauber dargestellt wird.

## Wichtige Einschränkung

Aktuell werden die Ladebildschirm-Bilder nicht aus einer JSON-Konfiguration geladen.

Das bedeutet:

- Das Ersetzen der vorhandenen 3 Bilder erfordert nur einen Texturtausch.
- Das Hinzufügen zusätzlicher Bilder erfordert eine kleine Script-Änderung in [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Empfohlener Workflow für Hosts

Wenn du nur die Grafiken austauschen möchtest, nutze **Option 1**.

Wenn du einen größeren zufälligen Bildpool möchtest, nutze **Option 2**.

## Anpassung der Teleport-Menü-Bilder

Die Zielkarten im F2-Teleport-Menü verwenden einen separaten Bildsatz, unabhängig vom Ladebildschirm.

Diese Bilder werden über `Picture`-IDs in `teleport_menu_config.json` zugeordnet.

Beispiel:

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

## Aktuelle Zuordnung der Menü-Bilder

Die aktuellen `Picture`-IDs sind mit folgenden Dateien verknüpft:

- `0` = kein Bild
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

## Option 1: Vorhandene Teleport-Menü-Bilder ersetzen

Dies ist die einfachste Methode, wenn du dieselben `Picture`-IDs beibehalten möchtest.

### Schritte

1. Bereite deine Ersatzbilder vor.
2. Konvertiere sie in `.edds`.
3. Ersetze die vorhandenen Dateien in [gui/assets](gui/assets), zum Beispiel:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Behalte dieselben Dateinamen bei, wenn du keine JSON-Konfiguration ändern willst.
5. Packe den Mod-PBO bei Bedarf neu.
6. Starte Server und Client neu.

### Ergebnis

Alle vorhandenen Zieleinträge funktionieren weiter, zeigen aber deine Ersatzgrafiken an.

## Option 2: Neue Teleport-Menü-Bilder hinzufügen

Wenn du zusätzliche Bild-IDs über den Standardumfang hinaus verwenden willst, musst du neue Dateien hinzufügen und das Bild-Array im Script erweitern.

### Schritte

1. Füge deine neuen `.edds`-Dateien in [gui/assets](gui/assets) hinzu, zum Beispiel:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Öffne [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Suche das Array `PICTURE_TEXTURES` innerhalb von `DME_Teleport_Constants`.
4. Füge deine neuen Texturen am Ende des Arrays hinzu.

Beispiel:

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

5. Verwende den neuen Array-Index in `teleport_menu_config.json`.

Beispiel:

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

6. Packe den Mod-PBO neu.
7. Starte Server und Client neu.

### Ergebnis

Das Teleport-Menü kann nun die neuen benutzerdefinierten Bild-IDs anzeigen.

## Hinweise zu Teleport-Menü-Bildern

- Menü-Bilder verwenden `.edds`-Texturen in [gui/assets](gui/assets).
- Verwende mod-relative Pfade wie `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Wenn du nur eine vorhandene Datei ersetzt, musst du `teleport_menu_config.json` nicht ändern.
- Wenn du neue Bilddateien hinzufügst, musst du auch `PICTURE_TEXTURES` in [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c) erweitern.

## Mögliche zukünftige Verbesserung

Wenn du möchtest, kann das später in eine separate JSON-Konfiguration ausgelagert werden, damit Server-Hosts Bilder des Ladebildschirms hinzufügen oder entfernen können, ohne Scripts zu bearbeiten.