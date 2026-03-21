# ローディング画面画像のカスタマイズ

このガイドでは、サーバーホストが `DME_Teleport_Overhaul` のテレポート用ローディング画面で使用される画像を置き換えたり追加したりする方法を説明します。

## 現在このModが画像を読み込む方法

ローディング画面の背景は [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) に直接定義されています。

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

テレポートのローディング画面が表示されるたびに、Modはこれらのファイルのうち1つをランダムに選択します。

## オプション1: 既存の3枚の画像を置き換える

これは最も簡単な方法で、スクリプトの編集は必要ありません。

### 手順

1. カスタム画像を用意します。
2. `.edds` 形式に変換します。
3. 正確に次の名前に変更します:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. [data](data) 内の既存ファイルを置き換えます:
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. 必要に応じてModのPBOを再パックします。
6. サーバーとクライアントを再起動します。

### 結果

ローディング画面はこれまで通り動作し、新しい画像が使用されます。

## オプション2: 3枚より多くの画像を追加する

デフォルトの3枚より多くの背景を使いたい場合は、新しい `.edds` ファイルを追加し、スクリプト内の背景リストを更新する必要があります。

### 手順

1. 新しい `.edds` ファイルを [data](data) に追加します。例:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) を開きます。
3. `DME_TeleportLoadingScreen()` コンストラクタを探します。
4. `m_Backgrounds` 配列を拡張します。

例:

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

5. ModのPBOを再パックします。
6. サーバーとクライアントを再起動します。

### 結果

ローディング画面は一覧にあるすべての画像からランダムに選択するようになります。

## 画像形式に関する注意

- ローディング画面は `.edds` テクスチャを使用します。
- すべての画像パスはModフォルダ内に置いてください。
- `DME_Teleport_Overhaul/data/yourimage.edds` のようなMod相対パスを使用してください。
- ファイル名にはスペースや特殊文字を避けてください。
- フルスクリーン背景がきれいに表示されるよう、ワイド画面の比率を維持してください。

## 重要な制限

現時点では、ローディング画面の画像はJSON設定から読み込まれません。

つまり:

- 既存の3枚の画像を置き換えるだけならテクスチャの差し替えだけで済みます。
- 画像を追加するには [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) の小さなスクリプト編集が必要です。

## ホスト向けの推奨ワークフロー

画像を差し替えるだけなら **オプション1** を使ってください。

より多くのランダム画像プールが必要なら **オプション2** を使ってください。

## テレポートメニュー画像のカスタマイズ

F2メニューの目的地カードは、ローディング画面とは別の画像セットを使用します。

これらの画像は `teleport_menu_config.json` 内の `Picture` ID で割り当てられます。

例:

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

## 現在のメニュー画像マッピング

現在の `Picture` ID は次のファイルに対応しています。

- `0` = 画像なし
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

## オプション1: 既存のテレポートメニュー画像を置き換える

同じ `Picture` ID を維持したい場合、これが最も簡単な方法です。

### 手順

1. 置き換え用の画像を準備します。
2. `.edds` に変換します。
3. [gui/assets](gui/assets) 内の既存ファイルを置き換えます。例:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. JSON設定を変更したくない場合は同じファイル名を維持してください。
5. 必要に応じてModのPBOを再パックします。
6. サーバーとクライアントを再起動します。

### 結果

既存の目的地エントリはそのまま動作し、置き換えた画像が表示されます。

## オプション2: 新しいテレポートメニュー画像を追加する

標準セット以上の画像IDを追加したい場合は、新しいファイルを追加し、スクリプト内の画像配列を拡張する必要があります。

### 手順

1. 新しい `.edds` ファイルを [gui/assets](gui/assets) に追加します。例:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c) を開きます。
3. `DME_Teleport_Constants` 内の `PICTURE_TEXTURES` 配列を探します。
4. 新しいテクスチャを配列の末尾に追加します。

例:

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

5. 新しい配列インデックスを `teleport_menu_config.json` で使用します。

例:

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

6. ModのPBOを再パックします。
7. サーバーとクライアントを再起動します。

### 結果

テレポートメニューで新しいカスタム画像IDを表示できるようになります。

## テレポートメニュー画像に関する注意

- メニュー画像は [gui/assets](gui/assets) 内の `.edds` テクスチャを使用します。
- `DME_Teleport_Overhaul/gui/assets/yourimage.edds` のようなMod相対パスを使用してください。
- 既存ファイルを置き換えるだけなら `teleport_menu_config.json` を変更する必要はありません。
- 新しい画像ファイルを追加する場合は、[scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c) 内の `PICTURE_TEXTURES` も拡張する必要があります。

## 将来的な改善案

必要であれば、将来的にこれを別のJSON設定に移し、サーバーホストがスクリプトを編集せずにローディング画面の画像を追加・削除できるようにすることも可能です。