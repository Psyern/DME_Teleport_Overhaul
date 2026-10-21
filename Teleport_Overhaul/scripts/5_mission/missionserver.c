modded class MissionServer
{
    void MissionServer()
    {
        DME_OverhaulJsonConfigManager.SpawnTeleportPoints();
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
