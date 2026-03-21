# 載入畫面圖片自訂

本指南說明伺服器主機如何替換或擴充 `DME_Teleport_Overhaul` 中傳送載入畫面所使用的圖片。

## 模組目前如何載入圖片

載入畫面的背景直接定義在 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) 中：

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

每次顯示傳送載入畫面時，模組都會從這些檔案中隨機選擇一個。

## 方案 1：替換現有的 3 張圖片

這是最簡單的方法，不需要修改任何腳本。

### 步驟

1. 準備你的自訂圖片。
2. 將它們轉換為 `.edds` 格式。
3. 將檔名精確改為：
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. 替換 [data](data) 中現有的檔案：
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. 如有需要，重新封裝模組 PBO。
6. 重新啟動伺服器與客戶端。

### 結果

載入畫面會像以前一樣正常運作，但現在會使用你的新圖片。

## 方案 2：新增超過 3 張圖片

如果你想使用多於預設三張的背景圖，就必須新增新的 `.edds` 檔案，並在腳本中更新背景清單。

### 步驟

1. 將新的 `.edds` 檔案加入 [data](data)，例如：
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. 開啟 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c)。
3. 找到 `DME_TeleportLoadingScreen()` 建構函式。
4. 擴充 `m_Backgrounds` 陣列。

範例：

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

5. 重新封裝模組 PBO。
6. 重新啟動伺服器與客戶端。

### 結果

載入畫面會從所有列出的圖片中隨機挑選。

## 圖片格式說明

- 載入畫面使用 `.edds` 紋理。
- 所有圖片路徑都應保留在模組資料夾內。
- 使用完整的模組相對路徑，例如 `DME_Teleport_Overhaul/data/yourimage.edds`。
- 避免在檔名中使用空格與特殊字元。
- 保持寬螢幕比例，讓全螢幕背景顯示更美觀。

## 重要限制

目前載入畫面的圖片不會從 JSON 設定中讀取。

這表示：

- 替換現有 3 張圖片只需要替換紋理檔案。
- 新增額外圖片需要對 [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) 進行少量腳本修改。

## 建議給主機的工作流程

如果你只是想更換美術素材，請使用 **方案 1**。

如果你想要更大的隨機圖片池，請使用 **方案 2**。

## 自訂傳送選單圖片

F2 選單中的傳送目的地卡片使用的是一套獨立於載入畫面的圖片資源。

這些圖片透過 `teleport_menu_config.json` 中的 `Picture` ID 進行對應。

範例：

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

## 目前選單圖片對應

目前的 `Picture` ID 對應以下檔案：

- `0` = 無圖片
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

## 方案 1：替換現有的傳送選單圖片

如果你想保留相同的 `Picture` ID，這是最簡單的方法。

### 步驟

1. 準備替換圖片。
2. 將其轉換為 `.edds`。
3. 替換 [gui/assets](gui/assets) 中現有的檔案，例如：
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. 如果你不想修改任何 JSON 設定，請保留相同的檔名。
5. 如有需要，重新封裝模組 PBO。
6. 重新啟動伺服器與客戶端。

### 結果

所有現有目的地條目都會繼續運作，但會顯示你的替換圖片。

## 方案 2：為傳送選單新增圖片

如果你想使用超出預設集合的額外圖片 ID，就需要新增檔案，並擴充腳本中的圖片陣列。

### 步驟

1. 將新的 `.edds` 檔案加入 [gui/assets](gui/assets)，例如：
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. 開啟 [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c)。
3. 找到 `DME_Teleport_Constants` 中的 `PICTURE_TEXTURES` 陣列。
4. 將新紋理加入陣列尾端。

範例：

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

5. 在 `teleport_menu_config.json` 中使用新的陣列索引。

範例：

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

6. 重新封裝模組 PBO。
7. 重新啟動伺服器與客戶端。

### 結果

傳送選單現在就能顯示新的自訂圖片 ID。

## 傳送選單圖片說明

- 選單圖片使用 [gui/assets](gui/assets) 中的 `.edds` 紋理。
- 使用模組相對路徑，例如 `DME_Teleport_Overhaul/gui/assets/yourimage.edds`。
- 如果你只是替換現有檔案，就不需要修改 `teleport_menu_config.json`。
- 如果你新增新的圖片檔案，也必須擴充 [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c) 中的 `PICTURE_TEXTURES`。

## 未來可能的改進

如果你願意，之後也可以把這部分移到獨立的 JSON 設定中，讓伺服器主機無需編輯腳本就能新增或移除載入畫面圖片。