# 加载界面图片自定义

本指南说明服务器主机如何替换或扩展 `DME_Teleport_Overhaul` 中传送加载界面所使用的图片。

## 模组当前如何加载图片

加载界面的背景直接定义在 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) 中：

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

每次显示传送加载界面时，模组都会从这些文件中随机选择一个。

## 方案 1：替换现有的 3 张图片

这是最简单的方法，不需要修改任何脚本。

### 步骤

1. 准备你的自定义图片。
2. 将它们转换为 `.edds` 格式。
3. 将文件名严格改为：
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. 替换 [data](data) 中现有的文件：
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. 如有需要，重新打包模组 PBO。
6. 重启服务器和客户端。

### 结果

加载界面会继续像以前一样工作，但现在会使用你的新图片。

## 方案 2：添加超过 3 张图片

如果你想使用多于默认三张的背景图，就必须添加新的 `.edds` 文件，并在脚本中更新背景列表。

### 步骤

1. 将新的 `.edds` 文件添加到 [data](data)，例如：
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. 打开 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c)。
3. 找到 `DME_TeleportLoadingScreen()` 构造函数。
4. 扩展 `m_Backgrounds` 数组。

示例：

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

5. 重新打包模组 PBO。
6. 重启服务器和客户端。

### 结果

加载界面将会从所有列出的图片中随机选择。

## 图片格式说明

- 加载界面使用 `.edds` 纹理。
- 所有图片路径都应放在模组文件夹内。
- 使用完整的模组相对路径，例如 `DME_Teleport_Overhaul/data/yourimage.edds`。
- 避免在文件名中使用空格和特殊字符。
- 保持宽屏比例，以便全屏背景显示更美观。

## 重要限制

目前，加载界面的图片不会从 JSON 配置中读取。

这意味着：

- 替换现有 3 张图片只需要替换纹理文件。
- 添加额外图片需要对 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) 做少量脚本修改。

## 推荐给服务器主机的工作流程

如果你只是想更换美术资源，请使用 **方案 1**。

如果你想要更大的随机图片池，请使用 **方案 2**。

## 自定义传送菜单图片

F2 菜单中的传送目的地卡片使用的是一套独立于加载界面的图片资源。

这些图片通过 `teleport_menu_config.json` 中的 `Picture` ID 进行映射。

示例：

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

## 当前菜单图片映射

当前的 `Picture` ID 对应以下文件：

- `0` = 无图片
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

## 方案 1：替换现有传送菜单图片

如果你想保留相同的 `Picture` ID，这是最简单的方法。

### 步骤

1. 准备替换图片。
2. 将其转换为 `.edds`。
3. 替换 [gui/assets](gui/assets) 中现有的文件，例如：
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. 如果你不想修改任何 JSON 配置，请保持相同的文件名。
5. 如有需要，重新打包模组 PBO。
6. 重启服务器和客户端。

### 结果

所有现有目的地条目都会继续工作，但会显示你的替换图片。

## 方案 2：为传送菜单添加新图片

如果你想使用超出默认集合的额外图片 ID，就需要添加新文件，并扩展脚本中的图片数组。

### 步骤

1. 将新的 `.edds` 文件添加到 [gui/assets](gui/assets)，例如：
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. 打开 [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c)。
3. 找到 `DME_Teleport_Constants` 中的 `PICTURE_TEXTURES` 数组。
4. 将新纹理添加到数组末尾。

示例：

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

5. 在 `teleport_menu_config.json` 中使用新的数组索引。

示例：

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

6. 重新打包模组 PBO。
7. 重启服务器和客户端。

### 结果

传送菜单现在就可以显示新的自定义图片 ID。

## 传送菜单图片说明

- 菜单图片使用 [gui/assets](gui/assets) 中的 `.edds` 纹理。
- 使用模组相对路径，例如 `DME_Teleport_Overhaul/gui/assets/yourimage.edds`。
- 如果你只是替换现有文件，则无需修改 `teleport_menu_config.json`。
- 如果你添加新的图片文件，还必须扩展 [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c) 中的 `PICTURE_TEXTURES`。

## 未来可能的改进

如果你愿意，今后也可以将这一部分移到单独的 JSON 配置中，这样服务器主机无需编辑脚本就能添加或删除加载界面图片。