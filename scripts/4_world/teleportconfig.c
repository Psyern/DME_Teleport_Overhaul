// Файл: scripts/4_World/teleportconfig.c

class LO_TeleportConfig
{
    ref array<ref LO_TeleportEntry> TeleportEntries;

    void LO_TeleportConfig()
    {
        TeleportEntries = new array<ref LO_TeleportEntry>();
    }
}

class LO_TeleportPreloadConfig
{
    ref array<ref LO_TeleportPreloadEntry> TeleportPreloads;

    void LO_TeleportPreloadConfig()
    {
        TeleportPreloads = new array<ref LO_TeleportPreloadEntry>();
    }
}

class LO_TeleportPreloadEntry
{
    string TeleportName;
    ref array<string> PreloadObjectTypes;

    void LO_TeleportPreloadEntry()
    {
        TeleportName = "";
        PreloadObjectTypes = new array<string>();
    }
}

class LO_TeleportEntry
{
    bool EnableTeleport;                    // Включен ли телепорт
    string TeleportName;                    // Имя телепорта, уникальное для каждого телепорта
    string ObjectType;                      // Тип объекта (например, "Land_Barn_Wooden")
    vector ObjectCoordinates;               // Координаты объекта в мире
    vector ObjectOrientation;               // Ориентация объекта (углы)
    vector TeleportPosition;                // Позиция для телепортации игрока
    float CheckRadius;                      // Радиус проверки
    string RequiredItem;                    // Предмет, который должен быть в руках у игрока
    float RequiredItemDamagePercent;        // Урон предмету в руках при использовании, в процентах. 0 - нет урона, 100 - уничтожится, 25 - на 4 использования хватит и т.п.
    int TeleportActiveTimeSeconds;          // Время в секундах, которое телепорт будет активен после первого использования и не будет требовать предмет (0 - на каждое использование требуется предмет)
    int TeleportCooldownSeconds;            // Кулдаун для этого телепорта в секундах (0 = нет кулдауна)
    string MissingItemMessage;              // Сообщение об отсутствии предмета
    bool UseSurfaceSafety;                  // Корректировать позицию до поверхности, если точка телепорта ниже земли
    ref array<string> PreloadObjectTypes;   // Список объектов для клиентского PreloadObject перед телепортом

    void LO_TeleportEntry()
    {
        EnableTeleport = true;
		TeleportName = "";
        CheckRadius = 5.0;
        
        // Инициализация координат с использованием вектора
        ObjectCoordinates = "0 0 0";          
        ObjectOrientation = "0 0 0";          
        TeleportPosition = "0 0 0";           

        RequiredItem = "";
        RequiredItemDamagePercent = 0;
        TeleportActiveTimeSeconds = 0;
        TeleportCooldownSeconds = 0; // По умолчанию нет кулдауна
        MissingItemMessage = "";
        UseSurfaceSafety = true;
        PreloadObjectTypes = new array<string>();
    }
}
