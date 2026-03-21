# Personalización de imágenes de la pantalla de carga

Esta guía explica cómo un administrador de servidor puede reemplazar o ampliar las imágenes utilizadas por la pantalla de carga del teletransporte en `DME_Teleport_Overhaul`.

## Cómo carga actualmente las imágenes el mod

Los fondos de la pantalla de carga están definidos directamente en [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Cada vez que se muestra la pantalla de carga del teletransporte, el mod elige aleatoriamente uno de estos archivos.

## Opción 1: Reemplazar las 3 imágenes existentes

Este es el método más sencillo y no requiere ninguna edición de scripts.

### Pasos

1. Prepara tus imágenes personalizadas.
2. Conviértelas al formato `.edds`.
3. Cámbiales el nombre exactamente a:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Reemplaza los archivos existentes en [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. Vuelve a empaquetar el PBO del mod si es necesario.
6. Reinicia el servidor y el cliente.

### Resultado

La pantalla de carga seguirá funcionando exactamente igual que antes, pero ahora usará tus nuevas imágenes.

## Opción 2: Añadir más de 3 imágenes

Si quieres más de los tres fondos predeterminados, debes añadir nuevos archivos `.edds` y actualizar la lista de fondos en el script.

### Pasos

1. Añade tus nuevos archivos `.edds` a [data](data), por ejemplo:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Abre [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Busca el constructor `DME_TeleportLoadingScreen()`.
4. Amplía el array `m_Backgrounds`.

Ejemplo:

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

5. Vuelve a empaquetar el PBO del mod.
6. Reinicia el servidor y el cliente.

### Resultado

La pantalla de carga elegirá aleatoriamente entre todas las imágenes listadas.

## Notas sobre el formato de imagen

- La pantalla de carga usa texturas `.edds`.
- Mantén todas las rutas de imagen dentro de la carpeta del mod.
- Usa rutas completas relativas al mod como `DME_Teleport_Overhaul/data/yourimage.edds`.
- Evita espacios y caracteres especiales en los nombres de archivo.
- Mantén una relación de aspecto panorámica para que el fondo a pantalla completa se vea limpio.

## Limitación importante

Actualmente, las imágenes de la pantalla de carga no se leen desde una configuración JSON.

Eso significa que:

- Reemplazar las 3 imágenes existentes solo requiere reemplazar las texturas.
- Añadir imágenes extra requiere una pequeña edición del script en [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Flujo de trabajo recomendado para hosts

Si solo quieres cambiar el arte, usa la **Opción 1**.

Si quieres un conjunto aleatorio de imágenes más grande, usa la **Opción 2**.

## Personalizar las imágenes del menú de teletransporte

Las tarjetas de destino en el menú F2 usan un conjunto de imágenes separado del de la pantalla de carga.

Estas imágenes se asignan mediante IDs `Picture` en `teleport_menu_config.json`.

Ejemplo:

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

## Mapeo actual de imágenes del menú

Los IDs `Picture` actuales están vinculados a estos archivos:

- `0` = sin imagen
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

## Opción 1: Reemplazar las imágenes existentes del menú de teletransporte

Este es el método más sencillo si quieres mantener los mismos IDs `Picture`.

### Pasos

1. Prepara tus imágenes de reemplazo.
2. Conviértelas a `.edds`.
3. Reemplaza los archivos existentes en [gui/assets](gui/assets), por ejemplo:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Mantén los mismos nombres de archivo si no quieres cambiar ninguna configuración JSON.
5. Vuelve a empaquetar el PBO del mod si es necesario.
6. Reinicia el servidor y el cliente.

### Resultado

Todas las entradas de destino existentes seguirán funcionando, pero mostrarán tus imágenes de reemplazo.

## Opción 2: Añadir nuevas imágenes al menú de teletransporte

Si quieres IDs de imagen adicionales más allá del conjunto predeterminado, debes añadir nuevos archivos y ampliar el array de imágenes en el script.

### Pasos

1. Añade tus nuevos archivos `.edds` a [gui/assets](gui/assets), por ejemplo:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Abre [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Busca el array `PICTURE_TEXTURES` dentro de `DME_Teleport_Constants`.
4. Añade tus nuevas texturas al final del array.

Ejemplo:

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

5. Usa el nuevo índice del array en `teleport_menu_config.json`.

Ejemplo:

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

6. Vuelve a empaquetar el PBO del mod.
7. Reinicia el servidor y el cliente.

### Resultado

El menú de teletransporte ahora podrá mostrar los nuevos IDs de imagen personalizados.

## Notas sobre las imágenes del menú de teletransporte

- Las imágenes del menú usan texturas `.edds` en [gui/assets](gui/assets).
- Usa rutas relativas al mod como `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Si solo reemplazas un archivo existente, no necesitas cambiar `teleport_menu_config.json`.
- Si añades nuevos archivos de imagen, también debes ampliar `PICTURE_TEXTURES` en [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Posible mejora futura

Si quieres, esto más adelante podría trasladarse a una configuración JSON separada para que los hosts puedan añadir o eliminar imágenes de la pantalla de carga sin editar scripts.