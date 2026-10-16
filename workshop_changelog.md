# DME Teleport Overhaul — Changelog

## 🔧 DayZ 1.30 Update

### Controls
- The teleport menu key is no longer hardcoded: it is a regular DayZ input now and can be rebound under Options → Controls → "DME Teleport" (default stays F2)
- Fixed: after closing the menu with Escape, the close button or a successful teleport, the key had to be pressed twice to open it again

### Localization
- Fixed: Simplified Chinese texts were not displayed correctly — `stringtable.csv` now uses the exact vanilla layout (quoted cells, trailing comma after the last column)
- The Traditional Chinese column now contains Traditional characters instead of Simplified ones
- New strings for the controls menu entry in all 13 languages

### DayZ 1.30
- Updated for DayZ 1.30 (`g_Game` instead of the deprecated `GetGame()`, no `ref` on local variables and return types, CF RPC enum without the legacy typo)
- Config files are loaded and saved with `JsonFileLoader.LoadFile/SaveFile` (the old `JsonLoadFile/JsonSaveFile` are deprecated); load errors are written to the server log
- Fixed: teleport configs were read only up to the first empty line

## 🚀 Full Overhaul Release

### Teleport Systems
- Two standalone teleport systems: world object teleports (hold-action interaction) and F2 UI menu teleport
- World teleports triggered via configurable objects (e.g. doors, buildings) with a 3-second hold action
- F2 teleport menu with scrollable card layout, destination images, cost/reputation display, and live cooldown timers
- 12 included destination thumbnails (Airport, City, Forest, Mountains, Tisy, Galaxy and more)
- Random teleport offset (±1m on X/Z) to prevent player stacking at destinations

### Reputation & Cost
- Full integration with DayZ Expansion Hardline reputation system
- Two reputation modes: RepMode 0 = check minimum reputation (no deduction), RepMode 1 = spend reputation as travel cost
- Dynamic UI label switches automatically between "Min Rep" and "Cost" based on RepMode

### Cooldowns
- Per-destination cooldowns with configurable duration (in seconds)
- Live countdown display in the menu with color coding (green = ready, red = on cooldown)
- Server-side cooldown tracking based on Unix timestamps (persistent across restarts)
- Anti-spam protection: 500ms minimum delay between teleport requests

### Required Item (World Teleports)
- Configurable required item per teleport entry (e.g. Crowbar)
- Item damage per use adjustable (0–100%)
- Teleport stays active after first activation for a configurable time window (no repeated item check needed)

### Player Protection During Teleport
- Player is set invisible, invulnerable, and non-solid during the teleport transition
- State is network-synced — other players cannot see the teleporting player
- AI ignores players during teleport transition (no AI targeting)
- State validation before every GUI teleport (alive, not unconscious, not restrained, not in vehicle)

### Loading Screen
- Fullscreen loading screen with random background image (3 included images)
- Animated progress bar with 12-second duration
- Smooth fade-out effect (1.5 seconds) at the end
- Game sound is muted during loading and restored afterwards
- HUD is automatically hidden during loading and restored when done
- Loading screen images can be replaced or extended (guide included in EN & PL)

### Particle Effects
- Teleport particle effect at the player's departure position (5-second duration)
- Broadcast to all nearby clients via RPC

### Player Marker
- Temporary 3D name marker at the departure position after GUI teleport (toggleable per destination)
- Marker shows the player's name for 5 minutes
- Only visible within 10 meters distance
- Duplicate detection: marker is refreshed instead of duplicated

### Prohibited Teleport Zones
- Radius zones (POI + radius) and polygon zones (3+ vertices) fully configurable
- Both directions checked: teleporting FROM a zone AND INTO a zone is blocked
- Individual warning messages per zone (supports stringtable keys)
- Example zones are auto-generated on first start

### Surface Safety
- Automatic height correction at destination (0.35m above terrain surface)
- Delayed recheck 1.2 seconds after teleport (prevents terrain streaming issues)
- Correction only applies if the player has not moved more than 5m from the expected position

### Preload System
- Global and per-teleport configurable object preloads (e.g. underground models)
- Automatic deduplication of preload lists
- Preloads are sent to the client before the teleport begins

### JSON Configuration & Profiles
- All config files are auto-generated on first server start (with example data)
- Automatic version migration (v1 → v2 → v3) when config structure changes
- Separate config files: teleport_config.json, teleport_menu_config.json, teleport_preload_config.json, ProhibitedZones.json
- Profile path: profiles/DeadmansEcho/TeleportOverhaul/

### Logging
- Dedicated teleport log file: profiles/DeadmansEcho/TeleportOverhaul/Logs/teleport.log
- Log includes: player name, Steam ID, origin, destination, teleport name

### Localization
- Full stringtable.csv with 22 string keys
- 14 languages: English, German, Czech, Russian, Polish, Hungarian, Italian, Spanish, French, Chinese (Traditional & Simplified), Japanese, Portuguese

### Dependencies
- Requires: DayZ Community Framework (JM_CF_Scripts)
- Requires: DayZ Expansion Hardline Scripts (DayZExpansion_Hardline_Scripts)
