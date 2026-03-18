class DME_TeleportLoadingScreen
{
    protected static const float FADE_DURATION_MS = 1500.0;

    protected Widget m_Root;
    protected ImageWidget m_Background;
    protected ProgressBarWidget m_ProgressBar;
    protected int m_StartTime;
    protected int m_EndTime;
    protected int m_FadeStartTime;
    protected bool m_IsVisible;
    protected bool m_RestoredSound;
    protected ref array<string> m_Backgrounds;
    protected ref TStringArray m_PreloadedObjectTypes;

    void DME_TeleportLoadingScreen()
    {
        m_Backgrounds = {
            "DME_Teleport_Overhaul/data/dme1.edds",
            "DME_Teleport_Overhaul/data/dme2.edds",
            "DME_Teleport_Overhaul/data/dme3.edds"
        };

        m_PreloadedObjectTypes = new TStringArray();
    }

    protected void EnsureLayout()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("DME_Teleport_Overhaul/gui/layouts/teleport_loading_screen.layout");
        if (!m_Root)
            return;

        m_Background = ImageWidget.Cast(m_Root.FindAnyWidget("Background"));
        m_ProgressBar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("ProgressBar"));
        m_Root.Show(false);
    }

    void Show(int durationMs, string preloadObjectTypes = string.Empty)
    {
        EnsureLayout();
        if (!m_Root)
            return;

        int safeDuration = durationMs;
        if (safeDuration <= 0)
            safeDuration = DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS;

        PreloadObjects(preloadObjectTypes, safeDuration);

        string backgroundPath = m_Backgrounds.Get(Math.RandomInt(0, m_Backgrounds.Count()));
        if (m_Background)
        {
            m_Background.LoadImageFile(0, backgroundPath);
            m_Background.SetImage(0);
        }

        m_StartTime = GetGame().GetTime();
        m_EndTime = m_StartTime + safeDuration;
        m_FadeStartTime = m_EndTime - FADE_DURATION_MS;
        m_IsVisible = true;
        m_RestoredSound = false;

        if (m_ProgressBar)
            m_ProgressBar.SetCurrent(0);

        m_Root.SetAlpha(1.0);
        m_Root.Show(true);

        if (GetGame().GetMission() && GetGame().GetMission().GetHud())
            GetGame().GetMission().GetHud().Show(false);

        if (GetGame().GetSoundScene())
            GetGame().GetSoundScene().SetSoundVolume(0, 0);
    }

    void Update()
    {
        if (!m_IsVisible || !m_Root)
            return;

        int currentTime = GetGame().GetTime();
        float progress = Math.InverseLerp(m_StartTime, m_EndTime, currentTime) * 100.0;
        progress = Math.Clamp(progress, 0.0, 100.0);

        if (m_ProgressBar)
            m_ProgressBar.SetCurrent(progress);

        if (currentTime >= m_FadeStartTime)
        {
            if (!m_RestoredSound)
            {
                RestoreSound();
                m_RestoredSound = true;
            }

            float alpha = (m_EndTime - currentTime) / FADE_DURATION_MS;
            alpha = Math.Clamp(alpha, 0.0, 1.0);
            m_Root.SetAlpha(alpha);
        }

        if (currentTime >= m_EndTime)
            Hide();
    }

    void Hide()
    {
        if (!m_Root)
            return;

        m_IsVisible = false;
        RestoreSound();
        m_Root.SetAlpha(1.0);
        m_Root.Show(false);

        if (GetGame().GetMission() && GetGame().GetMission().GetHud())
            GetGame().GetMission().GetHud().Show(true);
    }

    protected void RestoreSound()
    {
        if (GetGame().GetSoundScene())
            GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound, 1);
    }

    protected void PreloadObjects(string preloadObjectTypes, int durationMs)
    {
        m_PreloadedObjectTypes.Clear();

        if (preloadObjectTypes == string.Empty)
            return;

        preloadObjectTypes.Split("|", m_PreloadedObjectTypes);
        foreach (string objectType : m_PreloadedObjectTypes)
        {
            if (objectType == string.Empty)
                continue;

            g_Game.PreloadObject(objectType, durationMs);
        }
    }
}

modded class MissionGameplay
{
    protected ref DME_TeleportLoadingScreen m_DME_TeleportLoadingScreen;

    override void OnInit()
    {
        super.OnInit();

        if (!m_DME_TeleportLoadingScreen)
            m_DME_TeleportLoadingScreen = new DME_TeleportLoadingScreen();

        GetRPCManager().AddRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_LOADING_SCREEN, this, SingeplayerExecutionType.Client);
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (m_DME_TeleportLoadingScreen)
            m_DME_TeleportLoadingScreen.Update();
    }

    void ShowTeleportLoadingScreen(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param2<int, string> data;
        if (!ctx.Read(data))
            return;

        if (!m_DME_TeleportLoadingScreen)
            m_DME_TeleportLoadingScreen = new DME_TeleportLoadingScreen();

        m_DME_TeleportLoadingScreen.Show(data.param1, data.param2);
    }
}
