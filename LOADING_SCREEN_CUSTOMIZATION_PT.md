# Personalização das imagens da tela de carregamento

Este guia explica como um administrador de servidor pode substituir ou ampliar as imagens usadas pela tela de carregamento de teleporte em `DME_Teleport_Overhaul`.

## Como o mod carrega as imagens atualmente

Os fundos da tela de carregamento estão definidos diretamente em [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c):

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Cada vez que a tela de carregamento do teleporte é exibida, o mod escolhe aleatoriamente um desses arquivos.

## Opção 1: Substituir as 3 imagens existentes

Este é o método mais fácil e não exige nenhuma edição de script.

### Passos

1. Prepare suas imagens personalizadas.
2. Converta-as para o formato `.edds`.
3. Renomeie-as exatamente para:
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Substitua os arquivos existentes em [data](data):
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. Reempacote o PBO do mod, se necessário.
6. Reinicie o servidor e o cliente.

### Resultado

A tela de carregamento continuará funcionando exatamente como antes, mas agora usará suas novas imagens.

## Opção 2: Adicionar mais de 3 imagens

Se você quiser mais do que os três fundos padrão, precisa adicionar novos arquivos `.edds` e atualizar a lista de fundos no script.

### Passos

1. Adicione seus novos arquivos `.edds` em [data](data), por exemplo:
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Abra [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Encontre o construtor `DME_TeleportLoadingScreen()`.
4. Expanda o array `m_Backgrounds`.

Exemplo:

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

5. Reempacote o PBO do mod.
6. Reinicie o servidor e o cliente.

### Resultado

A tela de carregamento escolherá aleatoriamente entre todas as imagens listadas.

## Observações sobre o formato das imagens

- A tela de carregamento usa texturas `.edds`.
- Mantenha todos os caminhos das imagens dentro da pasta do mod.
- Use caminhos completos relativos ao mod, como `DME_Teleport_Overhaul/data/yourimage.edds`.
- Evite espaços e caracteres especiais nos nomes dos arquivos.
- Mantenha uma proporção widescreen para que o fundo em tela cheia fique limpo.

## Limitação importante

No momento, as imagens da tela de carregamento não são lidas a partir de uma configuração JSON.

Isso significa que:

- Substituir as 3 imagens existentes exige apenas a troca das texturas.
- Adicionar imagens extras exige uma pequena edição no script em [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Fluxo de trabalho recomendado para hosts

Se você quer apenas trocar a arte, use a **Opção 1**.

Se você quer um conjunto aleatório maior de imagens, use a **Opção 2**.

## Personalizando as imagens do menu de teleporte

Os cartões de destino no menu F2 usam um conjunto de imagens separado da tela de carregamento.

Essas imagens são mapeadas por IDs `Picture` em `teleport_menu_config.json`.

Exemplo:

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

## Mapeamento atual das imagens do menu

Os IDs `Picture` atuais estão vinculados a estes arquivos:

- `0` = sem imagem
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

## Opção 1: Substituir as imagens existentes do menu de teleporte

Este é o método mais fácil se você quiser manter os mesmos IDs `Picture`.

### Passos

1. Prepare suas imagens de substituição.
2. Converta-as para `.edds`.
3. Substitua os arquivos existentes em [gui/assets](gui/assets), por exemplo:
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Mantenha os mesmos nomes de arquivo se não quiser alterar nenhuma configuração JSON.
5. Reempacote o PBO do mod, se necessário.
6. Reinicie o servidor e o cliente.

### Resultado

Todas as entradas de destino existentes continuarão funcionando, mas exibirão suas imagens de substituição.

## Opção 2: Adicionar novas imagens ao menu de teleporte

Se você quiser IDs de imagem adicionais além do conjunto padrão, precisará adicionar os novos arquivos e expandir o array de imagens no script.

### Passos

1. Adicione seus novos arquivos `.edds` em [gui/assets](gui/assets), por exemplo:
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Abra [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Encontre o array `PICTURE_TEXTURES` dentro de `DME_Teleport_Constants`.
4. Adicione suas novas texturas ao final do array.

Exemplo:

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

5. Use o novo índice do array em `teleport_menu_config.json`.

Exemplo:

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

6. Reempacote o PBO do mod.
7. Reinicie o servidor e o cliente.

### Resultado

O menu de teleporte agora pode exibir os novos IDs de imagem personalizados.

## Observações sobre as imagens do menu de teleporte

- As imagens do menu usam texturas `.edds` em [gui/assets](gui/assets).
- Use caminhos relativos ao mod, como `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Se você apenas substituir um arquivo existente, não precisa alterar `teleport_menu_config.json`.
- Se adicionar novos arquivos de imagem, também precisará expandir `PICTURE_TEXTURES` em [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Possível melhoria futura

Se você quiser, isso poderá mais tarde ser movido para uma configuração JSON separada, para que hosts possam adicionar e remover imagens da tela de carregamento sem editar scripts.