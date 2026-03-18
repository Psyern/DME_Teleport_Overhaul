class DME_Teleport_Overhaul
{
    static const string ROOT_DIRECTORY = "$profile:DeadmansEcho/TeleportOverhaul";
    static const string CONFIG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/teleport_config.json";
    static const string PRELOAD_CONFIG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/teleport_preload_config.json";
    static const string LOG_DIRECTORY = "$profile:DeadmansEcho/TeleportOverhaul/Logs";
    static const string LOG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/Logs/teleport.log";
    static const string RPC_NAMESPACE = "DME_Teleport_Overhaul";
    static const string RPC_SHOW_LOADING_SCREEN = "ShowTeleportLoadingScreen";
    static const int LOADING_SCREEN_DURATION_MS = 12000;
    static const float TELEPORT_SURFACE_OFFSET = 0.35;
    static const int TELEPORT_POSITION_RECHECK_DELAY_MS = 1200;

    static void EnsureDirectories()
    {
        ref array<string> directories = { ROOT_DIRECTORY, LOG_DIRECTORY };

        foreach (string directory : directories)
        {
            if (!FileExist(directory))
            {
                MakeDirectory(directory);
                Print("[DME_Teleport_Overhaul] Created folder: " + directory);
            }
        }
    }

    static string SerializePreloadObjectTypes(array<string> preloadObjectTypes)
    {
        string serialized;

        if (!preloadObjectTypes)
            return serialized;

        foreach (int index, string objectType : preloadObjectTypes)
        {
            if (objectType == string.Empty)
                continue;

            if (serialized != string.Empty)
                serialized += "|";

            serialized += objectType;
        }

        return serialized;
    }
}
