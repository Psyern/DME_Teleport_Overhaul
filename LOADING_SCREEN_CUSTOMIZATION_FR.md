# Personnalisation des images de l’écran de chargement

Ce guide explique comment un hébergeur de serveur peut remplacer ou étendre les images utilisées par l’écran de chargement de téléportation dans `DME_Teleport_Overhaul`.

## Comment le mod charge actuellement les images

Les arrière-plans de l’écran de chargement sont définis directement dans [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c) :

```c
m_Backgrounds = {
	 "DME_Teleport_Overhaul/data/dme1.edds",
	 "DME_Teleport_Overhaul/data/dme2.edds",
	 "DME_Teleport_Overhaul/data/dme3.edds"
};
```

Chaque fois que l’écran de chargement de téléportation s’affiche, le mod choisit aléatoirement l’un de ces fichiers.

## Option 1 : Remplacer les 3 images existantes

C’est la méthode la plus simple et elle ne nécessite aucune modification de script.

### Étapes

1. Préparez vos images personnalisées.
2. Convertissez-les au format `.edds`.
3. Renommez-les exactement ainsi :
	- `dme1.edds`
	- `dme2.edds`
	- `dme3.edds`
4. Remplacez les fichiers existants dans [data](data) :
	- [data/dme1.edds](data/dme1.edds)
	- [data/dme2.edds](data/dme2.edds)
	- [data/dme3.edds](data/dme3.edds)
5. Repackez le PBO du mod si nécessaire.
6. Redémarrez le serveur et le client.

### Résultat

L’écran de chargement continuera de fonctionner exactement comme avant, mais utilisera désormais vos nouvelles images.

## Option 2 : Ajouter plus de 3 images

Si vous voulez plus que les trois arrière-plans par défaut, vous devez ajouter de nouveaux fichiers `.edds` et mettre à jour la liste des arrière-plans dans le script.

### Étapes

1. Ajoutez vos nouveaux fichiers `.edds` dans [data](data), par exemple :
	- `data/my_loading_01.edds`
	- `data/my_loading_02.edds`
	- `data/my_loading_03.edds`
2. Ouvrez [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).
3. Trouvez le constructeur `DME_TeleportLoadingScreen()`.
4. Étendez le tableau `m_Backgrounds`.

Exemple :

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

5. Repackez le PBO du mod.
6. Redémarrez le serveur et le client.

### Résultat

L’écran de chargement choisira aléatoirement parmi toutes les images listées.

## Remarques sur le format des images

- L’écran de chargement utilise des textures `.edds`.
- Conservez tous les chemins d’images à l’intérieur du dossier du mod.
- Utilisez des chemins relatifs au mod complets, par exemple `DME_Teleport_Overhaul/data/yourimage.edds`.
- Évitez les espaces et les caractères spéciaux dans les noms de fichiers.
- Gardez un format large afin que l’arrière-plan plein écran soit propre.

## Limitation importante

À l’heure actuelle, les images de l’écran de chargement ne sont pas lues depuis une configuration JSON.

Cela signifie que :

- Remplacer les 3 images existantes ne nécessite qu’un remplacement de texture.
- Ajouter des images supplémentaires nécessite une petite modification du script dans [scripts/5_mission/missiongameplay.c](scripts/5_mission/missiongameplay.c).

## Workflow recommandé pour les hébergeurs

Si vous souhaitez seulement remplacer les illustrations, utilisez **l’Option 1**.

Si vous souhaitez un plus grand pool d’images aléatoires, utilisez **l’Option 2**.

## Personnalisation des images du menu de téléportation

Les cartes de destination du menu F2 utilisent un ensemble d’images distinct de celui de l’écran de chargement.

Ces images sont associées à des identifiants `Picture` dans `teleport_menu_config.json`.

Exemple :

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

## Correspondance actuelle des images du menu

Les identifiants `Picture` actuels sont liés aux fichiers suivants :

- `0` = aucune image
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

## Option 1 : Remplacer les images existantes du menu de téléportation

C’est la méthode la plus simple si vous souhaitez conserver les mêmes identifiants `Picture`.

### Étapes

1. Préparez vos images de remplacement.
2. Convertissez-les en `.edds`.
3. Remplacez les fichiers existants dans [gui/assets](gui/assets), par exemple :
   - [gui/assets/airport.edds](gui/assets/airport.edds)
   - [gui/assets/airport2.edds](gui/assets/airport2.edds)
   - [gui/assets/city.edds](gui/assets/city.edds)
   - [gui/assets/forest.edds](gui/assets/forest.edds)
4. Conservez les mêmes noms de fichiers si vous ne voulez modifier aucune configuration JSON.
5. Repackez le PBO du mod si nécessaire.
6. Redémarrez le serveur et le client.

### Résultat

Toutes les destinations existantes continueront à fonctionner, mais afficheront vos images de remplacement.

## Option 2 : Ajouter de nouvelles images au menu de téléportation

Si vous voulez des identifiants d’image supplémentaires au-delà du jeu par défaut, vous devez ajouter les nouveaux fichiers et étendre le tableau d’images dans le script.

### Étapes

1. Ajoutez vos nouveaux fichiers `.edds` dans [gui/assets](gui/assets), par exemple :
   - `gui/assets/bunker.edds`
   - `gui/assets/trader.edds`
2. Ouvrez [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).
3. Trouvez le tableau `PICTURE_TEXTURES` dans `DME_Teleport_Constants`.
4. Ajoutez vos nouvelles textures à la fin du tableau.

Exemple :

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

5. Utilisez le nouvel index du tableau dans `teleport_menu_config.json`.

Exemple :

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

6. Repackez le PBO du mod.
7. Redémarrez le serveur et le client.

### Résultat

Le menu de téléportation pourra maintenant afficher les nouveaux identifiants d’images personnalisées.

## Remarques sur les images du menu de téléportation

- Les images du menu utilisent des textures `.edds` dans [gui/assets](gui/assets).
- Utilisez des chemins relatifs au mod comme `DME_Teleport_Overhaul/gui/assets/yourimage.edds`.
- Si vous remplacez seulement un fichier existant, vous n’avez pas besoin de modifier `teleport_menu_config.json`.
- Si vous ajoutez de nouveaux fichiers d’images, vous devez également étendre `PICTURE_TEXTURES` dans [scripts/3_game/actionteleportstandalone.c](scripts/3_game/actionteleportstandalone.c).

## Amélioration future possible

Si vous le souhaitez, cela pourra plus tard être déplacé vers une configuration JSON séparée, afin que les hébergeurs puissent ajouter ou supprimer des images d’écran de chargement sans modifier les scripts.