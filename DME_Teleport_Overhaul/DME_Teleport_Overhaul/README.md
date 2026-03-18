# 🚪 DME_Teleport_Overhaul — DayZ Mod

[![DayZ](https://img.shields.io/badge/DayZ-1.29+-blue)](https://dayz.com)
[![Enforce Script](https://img.shields.io/badge/Enforce-Script-orange)](https://community.bistudio.com/wiki/DayZ:Enforce_Script_Syntax)
[![Maintainer](https://img.shields.io/badge/Maintainer-Psyern-green)](https://github.com/Psyern)

> **Ein standalone Teleport-System für DayZ Standalone mit Loading-Screen, Progress-Bar, CF-RPCs und modularer JSON-Konfiguration.**

## ✨ Features

- Interaktive Teleport-Punkte über Weltobjekte
- 12 Sekunden Loading-Screen mit zufälligem Hintergrund aus `.edds`-Dateien
- Progress-Bar über Vollbild-Layout
- Audio-Mute während des Loading-Screens mit sanftem Fade-Out
- DayZ Community Framework RPC-Anbindung
- Standalone Profilstruktur unter `profiles/DeadmansEcho/TeleportOverhaul`
- Serverseitige Boden-Sicherheitskorrektur gegen Spawn unter der Map
- Modulare Preload-Konfiguration pro Teleport über separate JSON-Datei
- Teleport-Logging in eigene Log-Datei

## 📁 Profilstruktur

Der Mod arbeitet mit folgender Profilstruktur:

```text
profiles/
└── DeadmansEcho/
    └── TeleportOverhaul/
        ├── teleport_config.json
        ├── teleport_preload_config.json
        └── Logs/
            └── teleport.log
```

## ⚙️ Hauptkonfiguration

Datei: `profiles/DeadmansEcho/TeleportOverhaul/teleport_config.json`

Beispiel:

```json
{
    "TeleportEntries": [
        {
            "EnableTeleport": 1,
            "TeleportName": "Teleport 1",
            "ObjectType": "Land_Door_germa_1",
            "ObjectCoordinates": [7758.64013671875, 16.184600830078126, 14116.900390625],
            "ObjectOrientation": [0.0, 0.0, 0.0],
            "TeleportPosition": [10.2998046875, 10.765998840332032, 10.27001953125],
            "CheckRadius": 1.0,
            "RequiredItem": "Crowbar",
            "RequiredItemDamagePercent": 100.0,
            "TeleportActiveTimeSeconds": 30,
            "TeleportCooldownSeconds": 60,
            "MissingItemMessage": "You need a Crowbar to open the barn!",
            "UseSurfaceSafety": 1,
            "PreloadObjectTypes": []
        }
    ]
}
```

### Felder

- `EnableTeleport` — Aktiviert oder deaktiviert den Teleport
- `TeleportName` — Eindeutiger Name des Teleports
- `ObjectType` — Weltobjekt, an dem der Teleport hängt
- `ObjectCoordinates` — Position des Interaktionsobjekts
- `ObjectOrientation` — Rotation des sichtbaren Objekts
- `TeleportPosition` — Zielposition des Teleports
- `CheckRadius` — Distanzprüfung für Interaktion
- `RequiredItem` — Benötigter Gegenstand in der Hand
- `RequiredItemDamagePercent` — Schaden am benötigten Gegenstand
- `TeleportActiveTimeSeconds` — Zeitfenster, in dem der Teleport nach Aktivierung offen bleibt
- `TeleportCooldownSeconds` — Cooldown für erneute Nutzung
- `MissingItemMessage` — Nachricht bei fehlendem Item
- `UseSurfaceSafety` — Hebt Zielposition auf Bodenniveau an, falls nötig
- `PreloadObjectTypes` — Fallback-Feld, falls Preload direkt in der Hauptdatei gepflegt werden soll

## 🧩 Modulare Preload-Konfiguration

Datei: `profiles/DeadmansEcho/TeleportOverhaul/teleport_preload_config.json`

Beispiel:

```json
{
    "TeleportPreloads": [
        {
            "TeleportName": "Teleport 1",
            "PreloadObjectTypes": [
                "Land_Underground_Stairs_Exit",
                "Land_Underground_Stairs_Block"
            ]
        }
    ]
}
```

### Zweck

Diese Datei hält `PreloadObjectTypes` getrennt von der eigentlichen Teleport-Logik. Das ist sinnvoll, wenn:

- Teleport-Ziele oft wechseln
- Preload-Daten separat gepflegt werden sollen
- verschiedene Mapper oder Admins an unterschiedlichen Bereichen arbeiten

Der Mod merged die Einträge über `TeleportName` automatisch in die aktiven Teleport-Konfigurationen.

## 🖼️ Loading-Screen

Der Loading-Screen verwendet die Hintergrundbilder aus:

- `data/dme1.edds`
- `data/dme2.edds`
- `data/dme3.edds`

Verhalten:

- Anzeige bei Teleport-Start
- Dauer: 12 Sekunden
- Ton auf 0 während des Screens
- weiches Fade-Out am Ende
- HUD wird währenddessen ausgeblendet

## 🛡️ Teleport-Sicherheit

Um Probleme mit nicht vollständig geladenem Terrain oder Innenräumen zu reduzieren, nutzt der Mod aktuell zwei Schutzmechanismen:

- `UseSurfaceSafety` hebt die Zielposition serverseitig auf sichere Bodenhöhe an
- eine Nachprüfung korrigiert die Spielerposition kurz nach dem Teleport erneut

Zusätzlich können über `teleport_preload_config.json` gezielt Objektklassen vorgeladen werden.

## 🔌 Abhängigkeiten

Benötigt:

- DayZ Standalone
- DayZ Community Framework (`JM_CF_Scripts`)

## 🧠 Technische Hinweise

- Sprache: Enforce Script
- Netzwerk: CF RPC
- Scripts: `3_Game`, `4_World`, `5_Mission`
- Fokus: serverautoritatives Teleport-System
- Logging: `profiles/DeadmansEcho/TeleportOverhaul/Logs/teleport.log`

## 🚀 Installation

1. Mod in den Server- und Client-Loadorder aufnehmen
2. Sicherstellen, dass CF geladen wird
3. Server starten
4. Die Profil-Dateien werden bei Bedarf automatisch erstellt
5. Teleports in `teleport_config.json` konfigurieren
6. Optional Preload-Daten in `teleport_preload_config.json` pflegen

## 📌 Roadmap / Ideen

- genauere Zielvalidierung pro Teleport
- komfortablere Preload-Pflege für komplexe Interior-Ziele
- optional unterschiedliche Loading-Screen-Dauern pro Teleport
- zusätzliche visuelle Teleport-Effekte

## 👤 Credits

- Author: Psyern
- Basierend auf einem überarbeiteten Teleport-System für DayZ Standalone
