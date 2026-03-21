class DME_OverhaulJsonConfigManager
{
	static const int CURRENT_PRELOAD_CONFIG_VERSION = 1;

    static ref array<string> GetDefaultTeleportPreloadObjectTypes()
    {
        return {
            "StaticObj_Underground_Corridor_Main_Gate_R",
            "Land_Underground_Corridor_Main_Right",
            "Land_Underground_Storage_Big",
            "StaticObj_Underground_Corridor_Main_Gate_L"
        };
    }

    static ref DME_OverhaulTeleportPreloadConfig LoadTeleportPreloadConfig()
    {
        string error;
        ref DME_OverhaulTeleportPreloadConfig config = new DME_OverhaulTeleportPreloadConfig();

        string configFilePath = DME_Teleport_Overhaul.PRELOAD_CONFIG_FILE;
        if (FileExist(configFilePath))
        {
            string jsonContent;
            string line;
            FileHandle file = OpenFile(configFilePath, FileMode.READ);
            if (file != -1)
            {
                while (FGets(file, line) > 0)
                {
                    jsonContent += line;
                }

                CloseFile(file);

                JsonSerializer json = new JsonSerializer();
                json.ReadFromString(config, jsonContent, error);
            }
            else
            {
                Print("Failed to open the teleport preload config file!");
                Print(error);
            }
        }
        else
        {
            CreateDefaultPreloadConfig();
            if (FileExist(configFilePath))
                return LoadTeleportPreloadConfig();

            Print("[DME_Teleport_Overhaul] ERROR: Failed to create preload config. Directory may not exist.");
        }

        bool changed = EnsurePreloadDefaults(config);
        if (changed)
            SavePreloadConfig(config);

        return config;
    }

    // Метод для загрузки конфига
    static ref DME_OverhaulTeleportConfig LoadTeleportConfig()
    {
        string error;
        ref DME_OverhaulTeleportConfig config = new DME_OverhaulTeleportConfig();

        string configFilePath = DME_Teleport_Overhaul.CONFIG_FILE;  // Путь к конфигу
        if (FileExist(configFilePath))
        {
            // Чтение содержимого файла JSON
            string jsonContent;
            string line;
            FileHandle file = OpenFile(configFilePath, FileMode.READ);
            if (file != -1)  // Проверка на успешное открытие файла
            {
                while (FGets(file, line) > 0)  // Чтение содержимого файла
                {
                    // Строки добавляются в jsonContent
                    jsonContent += line;
                }
                CloseFile(file);  // Закрытие файла

                // Десериализация JSON в объект
                JsonSerializer json = new JsonSerializer();
                json.ReadFromString(config, jsonContent, error);  // Десериализация строки в объект
            }
            else
            {
                Print("Failed to open the JSON config file!");
                Print(error);
            }
        }
        else
        {
            CreateDefaultConfig();
            if (FileExist(configFilePath))
                return LoadTeleportConfig();

            Print("[DME_Teleport_Overhaul] ERROR: Failed to create teleport config. Directory may not exist.");
        }

        ApplyPreloadConfig(config, LoadTeleportPreloadConfig());

        return config;
    }

    static void ApplyPreloadConfig(DME_OverhaulTeleportConfig teleportConfig, DME_OverhaulTeleportPreloadConfig preloadConfig)
    {
        if (!teleportConfig || !preloadConfig)
            return;

        foreach (ref DME_OverhaulTeleportEntry teleportEntry : teleportConfig.TeleportEntries)
        {
            if (!teleportEntry)
                continue;

            teleportEntry.PreloadObjectTypes = new array<string>();
            InsertUniqueObjectTypes(teleportEntry.PreloadObjectTypes, preloadConfig.GlobalPreloadObjectTypes);

            foreach (ref DME_OverhaulTeleportPreloadEntry preloadEntry : preloadConfig.TeleportPreloads)
            {
                if (!preloadEntry || preloadEntry.TeleportName != teleportEntry.TeleportName)
                    continue;

                InsertUniqueObjectTypes(teleportEntry.PreloadObjectTypes, preloadEntry.PreloadObjectTypes);

                break;
            }
        }
    }

    static void InsertUniqueObjectTypes(array<string> target, array<string> source)
    {
        if (!target || !source)
            return;

        foreach (string objectType : source)
        {
            if (objectType == string.Empty)
                continue;

            if (target.Find(objectType) > -1)
                continue;

            target.Insert(objectType);
        }
    }

    static bool EnsurePreloadDefaults(DME_OverhaulTeleportPreloadConfig config)
    {
        bool changed;

        if (!config)
            return false;

		if (config.VersionID != CURRENT_PRELOAD_CONFIG_VERSION)
		{
			config.VersionID = CURRENT_PRELOAD_CONFIG_VERSION;
			changed = true;
		}

        if (!config.GlobalPreloadObjectTypes)
        {
            config.GlobalPreloadObjectTypes = new array<string>();
            changed = true;
        }

        if (!config.TeleportPreloads)
        {
            config.TeleportPreloads = new array<ref DME_OverhaulTeleportPreloadEntry>();
            changed = true;
        }

        ref array<string> defaultObjectTypes = GetDefaultTeleportPreloadObjectTypes();
        if (config.GlobalPreloadObjectTypes.Count() == 0)
        {
            InsertUniqueObjectTypes(config.GlobalPreloadObjectTypes, defaultObjectTypes);
            changed = true;
        }

        DME_OverhaulTeleportPreloadEntry teleportOneEntry = FindOrCreatePreloadEntry(config, "Teleport 1");
        if (teleportOneEntry && teleportOneEntry.PreloadObjectTypes.Count() == 0)
        {
            InsertUniqueObjectTypes(teleportOneEntry.PreloadObjectTypes, defaultObjectTypes);
            changed = true;
        }

        return changed;
    }

    static DME_OverhaulTeleportPreloadEntry FindOrCreatePreloadEntry(DME_OverhaulTeleportPreloadConfig config, string teleportName)
    {
        if (!config || !config.TeleportPreloads)
            return null;

        foreach (DME_OverhaulTeleportPreloadEntry preloadEntry : config.TeleportPreloads)
        {
            if (preloadEntry && preloadEntry.TeleportName == teleportName)
                return preloadEntry;
        }

        DME_OverhaulTeleportPreloadEntry newEntry = new DME_OverhaulTeleportPreloadEntry();
        newEntry.TeleportName = teleportName;
        config.TeleportPreloads.Insert(newEntry);
        return newEntry;
    }

    static void SavePreloadConfig(DME_OverhaulTeleportPreloadConfig config)
    {
        if (!config)
            return;

        string jsonContent;
        JsonSerializer json = new JsonSerializer();
        json.WriteToString(config, true, jsonContent);

        DME_Teleport_Overhaul.EnsureDirectories();

        FileHandle file = OpenFile(DME_Teleport_Overhaul.PRELOAD_CONFIG_FILE, FileMode.WRITE);
        if (file != -1)
        {
            FPrint(file, jsonContent);
            CloseFile(file);
        }
    }

    // Метод для создания дефолтного конфига
    static void CreateDefaultConfig()
    {
        ref DME_OverhaulTeleportConfig defaultConfig = new DME_OverhaulTeleportConfig();

        // Пример дефолтных данных для телепорта
        DME_OverhaulTeleportEntry entry1 = new DME_OverhaulTeleportEntry();
        entry1.EnableTeleport = true;
        entry1.TeleportName = "Teleport 1";
        entry1.ObjectType = "Land_Door_germa_1";
        entry1.ObjectCoordinates = { 7758.640137, 16.184601, 14116.900391 };
        entry1.TeleportPosition = { 10.299805, 10.765999, 10.270020 };
        entry1.CheckRadius = 1.0;
        entry1.RequiredItem = "Crowbar";
        entry1.RequiredItemDamagePercent = 100;
        entry1.TeleportActiveTimeSeconds = 30;
        entry1.TeleportCooldownSeconds = 60; // Добавленный параметр
        entry1.MissingItemMessage = "You need a Crowbar to open the barn!";
        entry1.UseSurfaceSafety = true;
        defaultConfig.TeleportEntries.Insert(entry1);

        DME_OverhaulTeleportEntry entry2 = new DME_OverhaulTeleportEntry();
        entry2.EnableTeleport = false;
        entry2.TeleportName = "Teleport 2";
        entry2.ObjectType = "Land_House_1W01";
        entry2.ObjectCoordinates = { 1250.0, 15.0, 1350.0 };
        entry2.TeleportPosition = { 1300.0, 20.0, 1400.0 };
        entry2.CheckRadius = 2.5;
        entry2.RequiredItem = "Shovel";
        entry2.RequiredItemDamagePercent = 0;
        entry2.TeleportActiveTimeSeconds = 0;
        entry2.TeleportCooldownSeconds = 0; // Добавленный параметр
        entry2.MissingItemMessage = "A Shovel is required to interact with this house!";
        entry2.UseSurfaceSafety = true;
        defaultConfig.TeleportEntries.Insert(entry2);

        // Сериализация данных в строку JSON
        string jsonContent;
        JsonSerializer json = new JsonSerializer();
        json.WriteToString(defaultConfig, true, jsonContent);  // Преобразование данных в строку

        DME_Teleport_Overhaul.EnsureDirectories();

        string configFilePath = DME_Teleport_Overhaul.CONFIG_FILE;
        FileHandle file = OpenFile(configFilePath, FileMode.WRITE);
        if (file != -1)  // Проверка на успешное открытие файла
        {
            FPrint(file, jsonContent);  // Запись в файл
            CloseFile(file);  // Закрытие файла
            Print("Default teleport config created!");
        }
        else
        {
            Print("Failed to create default teleport config!");
        }
    }

    static void CreateDefaultPreloadConfig()
    {
        ref DME_OverhaulTeleportPreloadConfig defaultConfig = new DME_OverhaulTeleportPreloadConfig();
		defaultConfig.VersionID = CURRENT_PRELOAD_CONFIG_VERSION;
        InsertUniqueObjectTypes(defaultConfig.GlobalPreloadObjectTypes, GetDefaultTeleportPreloadObjectTypes());

        DME_OverhaulTeleportPreloadEntry preloadEntry1 = new DME_OverhaulTeleportPreloadEntry();
        preloadEntry1.TeleportName = "Teleport 1";
        InsertUniqueObjectTypes(preloadEntry1.PreloadObjectTypes, GetDefaultTeleportPreloadObjectTypes());
        defaultConfig.TeleportPreloads.Insert(preloadEntry1);

        DME_OverhaulTeleportPreloadEntry preloadEntry2 = new DME_OverhaulTeleportPreloadEntry();
        preloadEntry2.TeleportName = "Teleport 2";
        defaultConfig.TeleportPreloads.Insert(preloadEntry2);

        string jsonContent;
        JsonSerializer json = new JsonSerializer();
        json.WriteToString(defaultConfig, true, jsonContent);

        DME_Teleport_Overhaul.EnsureDirectories();

        FileHandle file = OpenFile(DME_Teleport_Overhaul.PRELOAD_CONFIG_FILE, FileMode.WRITE);
        if (file != -1)
        {
            FPrint(file, jsonContent);
            CloseFile(file);
            Print("Default teleport preload config created!");
        }
        else
        {
            Print("Failed to create default teleport preload config!");
        }
    }

    static string GetSerializedGlobalPreloadObjectTypes()
    {
        DME_OverhaulTeleportPreloadConfig preloadConfig = LoadTeleportPreloadConfig();
        if (!preloadConfig)
            return string.Empty;

        return DME_Teleport_Overhaul.SerializePreloadObjectTypes(preloadConfig.GlobalPreloadObjectTypes);
    }

    static void SpawnTeleportPoints() {
        DME_OverhaulTeleportConfig m_TeleportConfig = LoadTeleportConfig();
        foreach (ref DME_OverhaulTeleportEntry entry : m_TeleportConfig.TeleportEntries)
        {
            if (entry.EnableTeleport)
            {
                // Спавним объект для взаимодействия
                DME_OverhaulTeleportHelper.CreateTeleportInteractionObject(entry.ObjectType, entry.ObjectCoordinates, entry.ObjectOrientation);
            }
        }
    }
}
