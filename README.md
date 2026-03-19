# 🚪 DME_Teleport_Overhaul — DayZ Mod

[![DayZ](https://img.shields.io/badge/DayZ-1.29+-blue)](https://dayz.com)
[![Enforce Script](https://img.shields.io/badge/Enforce-Script-orange)](https://community.bistudio.com/wiki/DayZ:Enforce_Script_Syntax)
[![Maintainer](https://img.shields.io/badge/Maintainer-Psyern-green)](https://github.com/Psyern)

> A standalone DayZ teleport system with world teleports, an F2 UI teleport menu, loading screens, CF RPCs, localized GUI strings, and modular profile-based JSON configuration.

## ✨ Features

- World interaction teleports placed through configured objects
- F2 teleport menu with destination cards, cooldowns, and reputation cost support
- Optional temporary player-name marker for GUI teleports
- Full-screen loading screen with random `.edds` backgrounds and progress bar
- Muted audio during loading with smooth fade-out and automatic HUD hide/show
- Server-authoritative teleport execution with delayed client loading screen flow
- Surface safety correction and post-teleport ground recheck
- Modular preload configuration with global and per-teleport object preloads
- Prohibited teleport zones with radius and polygon support
- Auto-generated profile configs with example data and migration support
- Localized GUI and warning messages through `stringtable.csv`
- Teleport logging to a dedicated profile log file

## 📁 Profile Structure

The mod uses the following standalone profile structure:

```text
profiles/
└── DeadmansEcho/
    └── TeleportOverhaul/
        ├── teleport_config.json
        ├── teleport_preload_config.json
        ├── teleport_menu_config.json
        ├── ProhibitedZones.json
        └── Logs/
            └── teleport.log
```

All of these files are created automatically on server start if they are missing. Existing files are also checked and updated when new config fields or versions are introduced.

## ⚙️ World Teleport Config

File: `profiles/DeadmansEcho/TeleportOverhaul/teleport_config.json`

Example:

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

### Fields

- `EnableTeleport` — Enables or disables the teleport entry
- `TeleportName` — Unique teleport identifier
- `ObjectType` — World object type used for the visible teleport object
- `ObjectCoordinates` — World position of the interaction object
- `ObjectOrientation` — Rotation of the visible object
- `TeleportPosition` — Teleport destination position
- `CheckRadius` — Interaction distance check
- `RequiredItem` — Item required in the player’s hands
- `RequiredItemDamagePercent` — Damage applied to the required item on use
- `TeleportActiveTimeSeconds` — Time window where the teleport remains active after first activation
- `TeleportCooldownSeconds` — Cooldown for reuse
- `MissingItemMessage` — Message shown when the required item is missing
- `UseSurfaceSafety` — Lifts the destination to safe ground level when needed
- `PreloadObjectTypes` — Optional per-entry preload fallback list

## 🗺️ UI Teleport Menu Config

File: `profiles/DeadmansEcho/TeleportOverhaul/teleport_menu_config.json`

Example:

```json
{
    "Version": 3,
    "RepMode": 1,
    "Destinations": [
        {
            "TeleportName": "Green Mountain",
            "TeleportPos": [3700.51, 0.0, 5981.27],
            "Cost": 1200,
            "CooldownSec": 1700,
            "Picture": 1,
            "Marker": 1
        },
        {
            "TeleportName": "Krasno Airfield",
            "TeleportPos": [11880.40, 0.0, 12460.74],
            "Cost": 1800,
            "CooldownSec": 1500,
            "Picture": 2,
            "Marker": 1
        }
    ]
}
```

### Fields

- `Version` — Config version used for migration
- `RepMode` — `0` = minimum reputation required, `1` = spend reputation on travel
- `Destinations` — Menu destinations shown in the F2 teleport UI
- `Picture` — Index of the GUI thumbnail texture
- `Marker` — `1` enables a temporary player-name marker after a GUI teleport, `0` disables it

### RepMode

- `RepMode: 0` — the `Cost` value is treated as the minimum reputation a player must have to use the teleport; reputation is checked but not removed.
- `RepMode: 1` — the `Cost` value is treated as a travel cost; the player must have enough reputation and that amount is deducted when the teleport is used successfully.

Example for `RepMode: 0`:

```json
{
    "Version": 3,
    "RepMode": 0,
    "Destinations": [
        {
            "TeleportName": "Green Mountain",
            "TeleportPos": [3700.51, 0.0, 5981.27],
            "Cost": 1200,
            "CooldownSec": 17,
            "Picture": 1,
            "Marker": 1
        },
        {
            "TeleportName": "Krasno Airfield",
            "TeleportPos": [11880.40, 0.0, 12460.74],
            "Cost": 1800,
            "CooldownSec": 15,
            "Picture": 2,
            "Marker": 1
        }
    ]
}
```

### Marker Behavior

- Markers are created only for teleports triggered through the F2 GUI menu
- A marker shows the teleported player’s name at the original position where the player stood before teleporting
- Marker lifetime is 5 minutes
- Marker becomes visible only when another player is within 10 meters of the marker position
- `Marker = 1` is the default during migration from older menu config versions

### Picture ID Mapping

Use the following `Picture` values in `teleport_menu_config.json`:

- `0` — no image
- `1` — `gui/assets/airport.edds`
- `2` — `gui/assets/airport2.edds`
- `3` — `gui/assets/baloon.edds`
- `4` — `gui/assets/city.edds`
- `5` — `gui/assets/fishing.edds`
- `6` — `gui/assets/forest.edds`
- `7` — `gui/assets/forest2.edds`
- `8` — `gui/assets/galaxy.edds`
- `9` — `gui/assets/landhouse.edds`
- `10` — `gui/assets/landside.edds`
- `11` — `gui/assets/mountains.edds`
- `12` — `gui/assets/tisy.edds`

### Suggested Usage

- `1` / `2` — airfields, military routes, or travel hubs
- `4` — towns, large settlements, or trader areas
- `5` — fishing spots, coastal routes, or harbor travel
- `6` / `7` / `11` — forests, remote camps, and mountain destinations
- `9` / `10` — residential areas, compounds, or underground access points
- `12` — high-tier military or endgame destinations
- `3` / `8` — special event teleports, fantasy locations, or custom server landmarks

## 🧩 Preload Config

File: `profiles/DeadmansEcho/TeleportOverhaul/teleport_preload_config.json`

Example:

```json
{
    "VersionID": 1,
    "GlobalPreloadObjectTypes": [
        "StaticObj_Underground_Corridor_Main_Gate_R",
        "Land_Underground_Corridor_Main_Right",
        "Land_Underground_Storage_Big",
        "StaticObj_Underground_Corridor_Main_Gate_L"
    ],
    "TeleportPreloads": [
        {
            "TeleportName": "Teleport 1",
            "PreloadObjectTypes": [
                "StaticObj_Underground_Corridor_Main_Gate_R",
                "Land_Underground_Corridor_Main_Right",
                "Land_Underground_Storage_Big",
                "StaticObj_Underground_Corridor_Main_Gate_L"
            ]
        }
    ]
}
```

### Behavior

- `GlobalPreloadObjectTypes` applies to all world teleports and to UI menu teleports
- `TeleportPreloads` adds extra preload types by `TeleportName`
- Duplicate object types are merged automatically
- The file is auto-created, version-checked, and backfilled on server start

## 🚫 Prohibited Teleport Zones

File: `profiles/DeadmansEcho/TeleportOverhaul/ProhibitedZones.json`

Example:

```json
{
    "VersionID": 1,
    "ProhibitedTeleportZones": [
        {
            "POI": [2731.57, 0.0, 1255.94],
            "Radius": 250.0,
            "WarningMessage": "#STR_DME_TELEPORT_WARNING_RADIUS_EXAMPLE",
            "PolygonZoneEnable": false,
            "PolygonVectors": []
        },
        {
            "POI": [],
            "Radius": 0.0,
            "WarningMessage": "#STR_DME_TELEPORT_WARNING_POLYGON_EXAMPLE",
            "PolygonZoneEnable": true,
            "PolygonVectors": [
                [3616.18, 0.0, 2041.39],
                [3662.84, 0.0, 2486.61],
                [4444.4, 0.0, 2331.07],
                [4331.63, 0.0, 2134.71]
            ]
        }
    ]
}
```

### Notes

- Radius zones use `POI` + `Radius`
- Polygon zones use `PolygonZoneEnable = true` and at least 3 `PolygonVectors`
- These checks are enforced server-side for UI/F2 teleports
- `WarningMessage` supports direct text or `#STR_...` stringtable keys
- Example zones are inserted automatically when the file is missing or empty

## 🖼️ Loading Screen

The loading screen uses the following backgrounds:

- `data/dme1.edds`
- `data/dme2.edds`
- `data/dme3.edds`

Behavior:

- Duration: 12 seconds
- Full-screen layout with progress bar
- Sound volume set to `0` during loading
- Smooth fade-out before the end
- HUD hidden while the loading screen is active
- Used by both world teleports and UI menu teleports

## 🌐 Localization

File: `stringtable.csv`

The mod now ships with localized GUI and warning keys, including:

- Menu title and buttons
- Status messages
- Reputation and cooldown labels
- Prohibited zone warning messages

Custom warning messages in JSON can also use stringtable keys such as:

- `#STR_DME_TELEPORT_WARNING_ZONE`
- `#STR_DME_TELEPORT_WARNING_DESTINATION`
- `#STR_DME_TELEPORT_WARNING_RADIUS_EXAMPLE`
- `#STR_DME_TELEPORT_WARNING_POLYGON_EXAMPLE`

## 🛡️ Teleport Safety

To reduce issues with incomplete terrain or underground/interior destinations, the mod uses multiple safety layers:

- Optional `UseSurfaceSafety` correction on server-side teleport position
- Post-teleport height recheck after a short delay
- Object preloading before teleport begins
- Delayed server teleport after the client loading screen finishes

## 🔌 Dependencies

Required:

- DayZ Standalone
- DayZ Community Framework (`JM_CF_Scripts`)
- DayZ Expansion Hardline Scripts (`DayZExpansion_Hardline_Scripts`)

## 🧠 Technical Notes

- Language: Enforce Script
- Network transport: CF RPC
- Script modules: `3_Game`, `4_World`, `5_Mission`
- Design goal: server-authoritative teleport flow
- Log file: `profiles/DeadmansEcho/TeleportOverhaul/Logs/teleport.log`

## 🚀 Installation

1. Add the mod to both server and client load order
2. Ensure `JM_CF_Scripts` is loaded before this mod
3. Ensure `DayZExpansion_Hardline_Scripts` is available for reputation-based menu travel
4. Start the server once to auto-generate profile configs
5. Adjust `teleport_config.json`, `teleport_menu_config.json`, `teleport_preload_config.json`, and `ProhibitedZones.json` as needed

## 👤 Credits

- Author: Psyern
- Built as a standalone overhaul and merge of multiple DayZ teleport system ideas
