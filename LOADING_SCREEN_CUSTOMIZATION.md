# Loading Screen Image Customization

This guide explains how a server host can replace or extend the images used by the teleport loading screen in `DME_Teleport_Overhaul`.

## How the mod currently loads images

The loading screen backgrounds are defined directly in [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Each time the teleport loading screen is shown, the mod randomly picks one of these files.

## Option 1: Replace the existing 3 images

This is the easiest method and does not require any script edits.

### Steps

1. Prepare your custom images.
2. Convert them to `.edds`.
3. Rename them exactly to:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Replace the existing files in [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. Repack the mod PBO if needed.
6. Restart the server and client.

### Result

The loading screen will keep working exactly as before, but it will now use your new images.

## Option 2: Add more than 3 images

If you want more than the default three backgrounds, you must add the new `.edds` files and update the background list in the script.

### Steps

1. Add your new `.edds` files to [data](data), for example:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Open [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Find the `DME_TeleportLoadingScreen()` constructor.
4. Extend the `m_Backgrounds` array.

Example:

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

5. Repack the mod PBO.
6. Restart the server and client.

### Result

The loading screen will randomly choose from all listed images.

## Image format notes

- The loading screen uses `.edds` textures.
- Keep all image paths inside the mod folder.
- Use full mod-relative paths such as `DME_Teleport_Overhaul/data/yourimage.edds`.
- Avoid spaces and special characters in file names.
- Keep a widescreen image ratio so the full-screen background looks clean.

## Important limitation

At the moment, loading screen images are not read from a JSON config.

That means:

- Replacing the existing 3 images requires only texture replacement.
- Adding extra images requires a small script edit in [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Recommended workflow for hosts

If you only want to swap artwork, use **Option 1**.

If you want a larger random pool of images, use **Option 2**.

## Customizing the teleport menu images

The teleport destination cards in the F2 menu use a separate image set from the loading screen.

These images are mapped by `Picture` IDs in `teleport_menu_config.json`.

Example:

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

## Current menu image mapping

The current `Picture` IDs are linked to these files:

- `0` = no image
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

## Option 1: Replace existing teleport menu images

This is the easiest method if you want to keep the same `Picture` IDs.

### Steps

1. Prepare your replacement images.
2. Convert them to `.edds`.
3. Replace the existing files in [gui/assets](gui/assets), for example:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Keep the same file names if you do not want to change any JSON config.
5. Repack the mod PBO if needed.
6. Restart the server and client.

### Result

All existing destination entries will continue to work, but they will display your replacement artwork.

## Option 2: Add new teleport menu images

If you want additional image IDs beyond the default set, you must add the new files and extend the picture array in script.

### Steps

1. Add your new `.edds` files to [gui/assets](gui/assets), for example:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Open [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Find the `PICTURE_TEXTURES` array inside `DME_Teleport_Constants`.
4. Add your new textures at the end of the array.

Example:

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

5. Use the new array index in `teleport_menu_config.json`.

Example:

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

6. Repack the mod PBO.
7. Restart the server and client.

### Result

The teleport menu can now display the new custom image IDs.

## Teleport menu image notes

- Menu images use `.edds` textures in [gui/assets](gui/assets).
- Use mod-relative paths such as `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- If you only replace an existing file, you do not need to change `teleport_menu_config.json`.
- If you add new image files, you must also extend `PICTURE_TEXTURES` in [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Possible future improvement

If you want, this can later be moved into a separate JSON config so server hosts can add and remove loading screen images without editing scripts.
