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

class DME_Teleport_Constants
{
    static const int DME_TELEPORT_MENU = 35002;
    static const string CONFIG_FOLDER = "$profile:DeadmansEcho/TeleportOverhaul";
    static const string CONFIG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/teleport_menu_config.json";
    static const string PROHIBITED_ZONES_FILE = "$profile:DeadmansEcho/TeleportOverhaul/ProhibitedZones.json";
    static const int TRAVEL_REQUEST_COOLDOWN_MS = 500;

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
        "DME_Teleport_Overhaul/gui/assets/tisy.edds"
    };

    static string GetPictureTexture(int id)
    {
        if (!PICTURE_TEXTURES)
            return string.Empty;

        if (id < 0 || id >= PICTURE_TEXTURES.Count())
            return string.Empty;

        return PICTURE_TEXTURES[id];
    }
};

class DME_Teleport_RPC
{
    static const string MOD_NAME = "DME_Teleport_Menu";
    static const string TRAVEL_REQUEST = "RPC_TravelRequest";
    static const string SYNC_REQUEST = "RPC_SyncRequest";
    static const string SYNC_STATE = "RPC_SyncState";
    static const string TRAVEL_RESULT = "RPC_TravelResult";
};
