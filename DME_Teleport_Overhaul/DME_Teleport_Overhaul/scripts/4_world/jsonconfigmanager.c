class JsonConfigManager
{
    static ref LO_TeleportPreloadConfig LoadTeleportPreloadConfig()
    {
        string error;
        ref LO_TeleportPreloadConfig config = new LO_TeleportPreloadConfig();

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
            return LoadTeleportPreloadConfig();
        }

        return config;
    }

    // Метод для загрузки конфига
    static ref LO_TeleportConfig LoadTeleportConfig()
    {
        string error;
        ref LO_TeleportConfig config = new LO_TeleportConfig();

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
            // Если конфиг не найден, создаем новый
            CreateDefaultConfig();
            return LoadTeleportConfig();  // Загружаем конфиг после его создания
        }

        ApplyPreloadConfig(config, LoadTeleportPreloadConfig());

        return config;
    }

    static void ApplyPreloadConfig(LO_TeleportConfig teleportConfig, LO_TeleportPreloadConfig preloadConfig)
    {
        if (!teleportConfig || !preloadConfig)
            return;

        foreach (ref LO_TeleportEntry teleportEntry : teleportConfig.TeleportEntries)
        {
            if (!teleportEntry)
                continue;

            foreach (ref LO_TeleportPreloadEntry preloadEntry : preloadConfig.TeleportPreloads)
            {
                if (!preloadEntry || preloadEntry.TeleportName != teleportEntry.TeleportName)
                    continue;

                teleportEntry.PreloadObjectTypes = new array<string>();
                foreach (string objectType : preloadEntry.PreloadObjectTypes)
                {
                    teleportEntry.PreloadObjectTypes.Insert(objectType);
                }

                break;
            }
        }
    }

    // Метод для создания дефолтного конфига
    static void CreateDefaultConfig()
    {
        ref LO_TeleportConfig defaultConfig = new LO_TeleportConfig();

        // Пример дефолтных данных для телепорта
        LO_TeleportEntry entry1 = new LO_TeleportEntry();
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

        LO_TeleportEntry entry2 = new LO_TeleportEntry();
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
        ref LO_TeleportPreloadConfig defaultConfig = new LO_TeleportPreloadConfig();

        LO_TeleportPreloadEntry preloadEntry1 = new LO_TeleportPreloadEntry();
        preloadEntry1.TeleportName = "Teleport 1";
        defaultConfig.TeleportPreloads.Insert(preloadEntry1);

        LO_TeleportPreloadEntry preloadEntry2 = new LO_TeleportPreloadEntry();
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

    static void SpawnTeleportPoints() {
        LO_TeleportConfig m_TeleportConfig = LoadTeleportConfig();
        foreach (ref LO_TeleportEntry entry : m_TeleportConfig.TeleportEntries)
        {
            if (entry.EnableTeleport)
            {
                // Спавним объект для взаимодействия
                TeleportHelper.CreateTeleportInteractionObject(entry.ObjectType, entry.ObjectCoordinates, entry.ObjectOrientation);
            }
        }
    }
}
