# Настройка изображений экрана загрузки

Это руководство объясняет, как владелец сервера может заменить или расширить набор изображений, используемых экраном загрузки телепорта в `DME_Teleport_Overhaul`.

## Как мод сейчас загружает изображения

Фоны экрана загрузки определены напрямую в [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Каждый раз, когда показывается экран загрузки телепорта, мод случайным образом выбирает один из этих файлов.

## Вариант 1: Заменить существующие 3 изображения

Это самый простой способ, и он не требует изменений в скриптах.

### Шаги

1. Подготовьте свои изображения.
2. Конвертируйте их в формат `.edds`.
3. Переименуйте их строго в:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Замените существующие файлы в [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. При необходимости перепакуйте PBO мода.
6. Перезапустите сервер и клиент.

### Результат

Экран загрузки продолжит работать как и раньше, но теперь будет использовать ваши новые изображения.

## Вариант 2: Добавить больше 3 изображений

Если вы хотите использовать больше трёх стандартных фонов, вам нужно добавить новые файлы `.edds` и обновить список фонов в скрипте.

### Шаги

1. Добавьте новые `.edds` файлы в [data](data), например:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Откройте [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Найдите конструктор `DME_TeleportLoadingScreen()`.
4. Расширьте массив `m_Backgrounds`.

Пример:

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

5. Перепакуйте PBO мода.
6. Перезапустите сервер и клиент.

### Результат

Экран загрузки будет случайным образом выбирать изображения из всего списка.

## Примечания по формату изображений

- Экран загрузки использует текстуры `.edds`.
- Храните все пути к изображениям внутри папки мода.
- Используйте полные пути относительно мода, например `DME_Teleport_Overhaul/data/yourimage.edds`.
- Избегайте пробелов и специальных символов в именах файлов.
- Используйте широкоформатное соотношение сторон, чтобы полноэкранный фон выглядел аккуратно.

## Важное ограничение

В данный момент изображения экрана загрузки не считываются из JSON-конфига.

Это означает следующее:

- Замена существующих 3 изображений требует только замены текстур.
- Добавление дополнительных изображений требует небольшой правки скрипта в [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Рекомендуемый рабочий процесс для хостов

Если вы хотите только заменить изображения, используйте **Вариант 1**.

Если вы хотите более крупный случайный набор изображений, используйте **Вариант 2**.

## Настройка изображений меню телепорта

Карточки пунктов назначения в меню F2 используют отдельный набор изображений, не связанный с экраном загрузки.

Эти изображения сопоставляются через ID `Picture` в `teleport_menu_config.json`.

Пример:

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

## Текущее сопоставление изображений меню

Текущие ID `Picture` связаны со следующими файлами:

- `0` = без изображения
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

## Вариант 1: Заменить существующие изображения меню телепорта

Это самый простой способ, если вы хотите сохранить те же ID `Picture`.

### Шаги

1. Подготовьте изображения для замены.
2. Конвертируйте их в `.edds`.
3. Замените существующие файлы в [gui/assets](gui/assets), например:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Оставьте те же имена файлов, если не хотите менять JSON-конфиг.
5. При необходимости перепакуйте PBO мода.
6. Перезапустите сервер и клиент.

### Результат

Все существующие пункты назначения продолжат работать, но будут отображать ваши новые изображения.

## Вариант 2: Добавить новые изображения в меню телепорта

Если вам нужны дополнительные ID изображений сверх стандартного набора, необходимо добавить новые файлы и расширить массив изображений в скрипте.

### Шаги

1. Добавьте новые `.edds` файлы в [gui/assets](gui/assets), например:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Откройте [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Найдите массив `PICTURE_TEXTURES` внутри `DME_Teleport_Constants`.
4. Добавьте новые текстуры в конец массива.

Пример:

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

5. Используйте новый индекс массива в `teleport_menu_config.json`.

Пример:

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

6. Перепакуйте PBO мода.
7. Перезапустите сервер и клиент.

### Результат

Теперь меню телепорта сможет отображать новые пользовательские ID изображений.

## Примечания по изображениям меню телепорта

- Изображения меню используют `.edds`-текстуры в [gui/assets](gui/assets).
- Используйте пути относительно мода, например `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Если вы только заменяете существующий файл, менять `teleport_menu_config.json` не нужно.
- Если вы добавляете новые файлы изображений, также нужно расширить `PICTURE_TEXTURES` в [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Возможное будущее улучшение

При желании это позже можно перенести в отдельный JSON-конфиг, чтобы владельцы серверов могли добавлять и удалять изображения экрана загрузки без редактирования скриптов.