class DME_Teleport_Overhaul
{
    static const string ROOT_DIRECTORY = "$profile:DeadmansEcho/TeleportOverhaul";
    static const string CONFIG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/teleport_config.json";
    static const string PRELOAD_CONFIG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/teleport_preload_config.json";
    static const string LOG_DIRECTORY = "$profile:DeadmansEcho/TeleportOverhaul/Logs";
    static const string LOG_FILE = "$profile:DeadmansEcho/TeleportOverhaul/Logs/teleport.log";
    static const string RPC_NAMESPACE = "DME_Teleport_Overhaul";
    static const string RPC_SHOW_LOADING_SCREEN = "ShowTeleportLoadingScreen";
    static const string RPC_SHOW_TELEPORT_MARKER = "ShowTeleportMarker";
    static const string RPC_SHOW_TELEPORT_PARTICLE = "ShowTeleportParticle";
    static const string GUI_TELEPORT_SOUND_SET = "DME_TeleportPortal_SoundSet";
    static const string GUI_TELEPORT_OUT_SOUND_SET = "DME_TeleportPortalOut_SoundSet";
    static const int LOADING_SCREEN_DURATION_MS = 12000;
    static const int GUI_TELEPORT_DELAY_MS = 7000;
    static const int ACTION_TELEPORT_DELAY_MS = 1000;
    static const int GUI_TELEPORT_PARTICLE_DURATION_MS = 7000;
    static const int TELEPORT_ARRIVAL_EFFECT_LEAD_MS = 2000;
    static const int TELEPORT_ARRIVAL_EFFECT_DURATION_MS = 2000;
    static const int GUI_TELEPORT_MARKER_DURATION_MS = 300000;
    static const float TELEPORT_SURFACE_OFFSET = 0.35;
    static const int TELEPORT_POSITION_RECHECK_DELAY_MS = 1200;
    static const float GUI_TELEPORT_MARKER_VISIBILITY_RADIUS = 10.0;
    static const float GUI_TELEPORT_MARKER_HEIGHT_OFFSET = 1.8;

    static void EnsureDirectories()
    {
        // $profile:DeadmansEcho -> ROOT_DIRECTORY -> LOG_DIRECTORY (every missing level is created)
        if (FileExist(LOG_DIRECTORY))
            return;

        if (PsyCore_Paths.EnsureDir(LOG_DIRECTORY))
            PsyCore_Log.Get("DME_Teleport_Overhaul").Info("Created folder: " + LOG_DIRECTORY);
        else
            PsyCore_Log.Get("DME_Teleport_Overhaul").Error("Could not create folder: " + LOG_DIRECTORY);
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
    // was 35002 (collided with DME_Custom_Events); first id of the reserved PsyCore block
    static const int DME_TELEPORT_MENU = PsyCore_Ids.MENU_DME_TELEPORT;
    static const string INPUT_MENU_TOGGLE = "UADMETeleportMenuToggle";
    static const int ECONOMY_MODE_NONE = 0;
    static const int ECONOMY_MODE_HARDLINE_MIN_REP = 1;
    static const int ECONOMY_MODE_HARDLINE_COST = 2;
    static const int ECONOMY_MODE_ITEM = 3;
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
