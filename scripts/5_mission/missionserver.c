modded class MissionServer
{
    void MissionServer()
    {
        JsonConfigManager.SpawnTeleportPoints();
    }

    override void OnInit()
    {
        super.OnInit();
        DME_TeleportManager.GetInstance();
    }

    override void OnMissionFinish()
    {
        DME_TeleportManager.DestroyInstance();
        super.OnMissionFinish();
    }
};
