class DME_TeleportLoadingScreen
{
    protected static const float FADE_DURATION_MS = 1500.0;

    protected Widget m_Root;
    protected ImageWidget m_Background;
    protected TextWidget m_TitleText;
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
        m_TitleText = TextWidget.Cast(m_Root.FindAnyWidget("Title"));
        m_ProgressBar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("ProgressBar"));

        if (m_TitleText)
            m_TitleText.SetText(Widget.TranslateString("#STR_DME_TELEPORT_LOADING_PREPARING"));

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

class DME_TeleportPlayerMarker
{
    protected Widget m_Root;
    protected TextWidget m_NameText;
    protected string m_PlayerName;
    protected vector m_WorldPosition;
    protected int m_ExpireTime;

    void DME_TeleportPlayerMarker(string playerName, vector worldPosition, int durationMs)
    {
        m_PlayerName = playerName;
        m_WorldPosition = worldPosition;
        m_ExpireTime = GetGame().GetTime() + durationMs;
    }

    void EnsureLayout()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("DME_Teleport_Overhaul/gui/layouts/dme_teleport_player_marker.layout");
        if (!m_Root)
            return;

        m_NameText = TextWidget.Cast(m_Root.FindAnyWidget("MarkerNameText"));
        if (m_NameText)
            m_NameText.SetText(m_PlayerName);

        m_Root.Show(false);
    }

    bool Matches(string playerName, vector worldPosition)
    {
        if (m_PlayerName != playerName)
            return false;

        return vector.Distance(m_WorldPosition, worldPosition) <= 0.5;
    }

    void Refresh(string playerName, vector worldPosition, int durationMs)
    {
        m_PlayerName = playerName;
        m_WorldPosition = worldPosition;
        m_ExpireTime = GetGame().GetTime() + durationMs;

        if (m_NameText)
            m_NameText.SetText(m_PlayerName);
    }

    bool Update()
    {
        EnsureLayout();
        if (!m_Root)
            return false;

        if (GetGame().GetTime() >= m_ExpireTime)
        {
            Destroy();
            return false;
        }

        PlayerBase localPlayer = PlayerBase.Cast(GetGame().GetPlayer());
        if (!localPlayer || !localPlayer.IsAlive())
        {
            m_Root.Show(false);
            return true;
        }

        if (vector.Distance(localPlayer.GetPosition(), m_WorldPosition) > DME_Teleport_Overhaul.GUI_TELEPORT_MARKER_VISIBILITY_RADIUS)
        {
            m_Root.Show(false);
            return true;
        }

        vector markerPosition = m_WorldPosition;
        markerPosition[1] = markerPosition[1] + DME_Teleport_Overhaul.GUI_TELEPORT_MARKER_HEIGHT_OFFSET;
        vector screenPosition = g_Game.GetScreenPosRelative(markerPosition);
        if (screenPosition[0] >= 1 || screenPosition[0] <= 0 || screenPosition[1] >= 1 || screenPosition[1] <= 0 || screenPosition[2] <= 0)
        {
            m_Root.Show(false);
            return true;
        }

        m_Root.SetPos(screenPosition[0] - 0.08, screenPosition[1] - 0.03);
        m_Root.Show(true);
        return true;
    }

    void Destroy()
    {
        if (m_Root)
        {
            m_Root.Unlink();
            m_Root = null;
        }
    }
}

class DME_TeleportParticleEffect
{
    protected Particle m_Particle;
    protected int m_ExpireTime;

    void DME_TeleportParticleEffect(vector worldPosition, int durationMs)
    {
        m_ExpireTime = GetGame().GetTime() + durationMs;
        m_Particle = Particle.PlayInWorld(ParticleList.DME_OVERHAUL_TELEPORT_PTC, worldPosition);
    }

    bool Update()
    {
        if (GetGame().GetTime() < m_ExpireTime)
            return true;

        Destroy();
        return false;
    }

    void Destroy()
    {
        if (!m_Particle)
            return;

        m_Particle.Stop();
        GetGame().ObjectDelete(m_Particle);
        m_Particle = null;
    }
}

modded class MissionGameplay
{
    protected ref DME_TeleportLoadingScreen m_DME_TeleportLoadingScreen;
	private ref DME_TeleportMenu m_DMETeleportMenu;
	private bool m_DMETeleportMenuOpen;
    protected ref array<ref DME_TeleportPlayerMarker> m_DME_TeleportMarkers;
    protected ref array<ref DME_TeleportParticleEffect> m_DME_TeleportParticles;

    override void OnInit()
    {
        super.OnInit();

        if (!m_DME_TeleportLoadingScreen)
            m_DME_TeleportLoadingScreen = new DME_TeleportLoadingScreen();

		if (!m_DME_TeleportMarkers)
			m_DME_TeleportMarkers = new array<ref DME_TeleportPlayerMarker>;

        if (!m_DME_TeleportParticles)
            m_DME_TeleportParticles = new array<ref DME_TeleportParticleEffect>;

		m_DMETeleportMenuOpen = false;

        GetRPCManager().AddRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_LOADING_SCREEN, this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_TELEPORT_MARKER, this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_TELEPORT_PARTICLE, this, SingeplayerExecutionType.Client);
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (m_DME_TeleportLoadingScreen)
            m_DME_TeleportLoadingScreen.Update();

		UpdateTeleportMarkers();
		UpdateTeleportParticles();
    }

    override void OnMissionFinish()
    {
		ClearTeleportParticles();
		ClearTeleportMarkers();
        CloseDMETeleportMenu();
        DME_TeleportManager.DestroyInstance();
        super.OnMissionFinish();
    }

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);

        if (key == KeyCode.KC_F2)
        {
            if (m_DMETeleportMenuOpen)
            {
                CloseDMETeleportMenu();
                return;
            }

            UIScriptedMenu topMenu = GetGame().GetUIManager().GetMenu();
            if (topMenu)
                return;

            OpenDMETeleportMenu();
        }
    }

    private void OpenDMETeleportMenu()
    {
        if (m_DMETeleportMenuOpen)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive())
            return;

        if (!m_DMETeleportMenu)
            m_DMETeleportMenu = new DME_TeleportMenu();

        GetGame().GetUIManager().ShowScriptedMenu(m_DMETeleportMenu, null);
        m_DMETeleportMenuOpen = true;
    }

    private void CloseDMETeleportMenu()
    {
        if (!m_DMETeleportMenuOpen)
            return;

        if (m_DMETeleportMenu)
            GetGame().GetUIManager().HideScriptedMenu(m_DMETeleportMenu);

        m_DMETeleportMenuOpen = false;
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

    void ShowTeleportMarker(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param3<string, vector, int> data = new Param3<string, vector, int>(string.Empty, vector.Zero, 0);
        if (!ctx.Read(data))
            return;

        if (!m_DME_TeleportMarkers)
            m_DME_TeleportMarkers = new array<ref DME_TeleportPlayerMarker>;

        for (int markerIndex = 0; markerIndex < m_DME_TeleportMarkers.Count(); markerIndex++)
        {
            DME_TeleportPlayerMarker existingMarker = m_DME_TeleportMarkers[markerIndex];
            if (!existingMarker || !existingMarker.Matches(data.param1, data.param2))
                continue;

            existingMarker.Refresh(data.param1, data.param2, data.param3);
            return;
        }

        m_DME_TeleportMarkers.Insert(new DME_TeleportPlayerMarker(data.param1, data.param2, data.param3));
    }

    void ShowTeleportParticle(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param2<vector, int> data = new Param2<vector, int>(vector.Zero, 0);
        if (!ctx.Read(data))
            return;

        if (!m_DME_TeleportParticles)
            m_DME_TeleportParticles = new array<ref DME_TeleportParticleEffect>;

        m_DME_TeleportParticles.Insert(new DME_TeleportParticleEffect(data.param1, data.param2));
    }

    protected void UpdateTeleportMarkers()
    {
        if (!m_DME_TeleportMarkers)
            return;

        for (int markerIndex = m_DME_TeleportMarkers.Count() - 1; markerIndex >= 0; markerIndex--)
        {
            DME_TeleportPlayerMarker marker = m_DME_TeleportMarkers[markerIndex];
            if (marker && marker.Update())
                continue;

            m_DME_TeleportMarkers.Remove(markerIndex);
        }
    }

    protected void UpdateTeleportParticles()
    {
        if (!m_DME_TeleportParticles)
            return;

        for (int particleIndex = m_DME_TeleportParticles.Count() - 1; particleIndex >= 0; particleIndex--)
        {
            DME_TeleportParticleEffect particleEffect = m_DME_TeleportParticles[particleIndex];
            if (particleEffect && particleEffect.Update())
                continue;

            m_DME_TeleportParticles.Remove(particleIndex);
        }
    }

    protected void ClearTeleportMarkers()
    {
        if (!m_DME_TeleportMarkers)
            return;

        foreach (DME_TeleportPlayerMarker marker : m_DME_TeleportMarkers)
        {
            if (marker)
                marker.Destroy();
        }

        m_DME_TeleportMarkers.Clear();
    }

    protected void ClearTeleportParticles()
    {
        if (!m_DME_TeleportParticles)
            return;

        foreach (DME_TeleportParticleEffect particleEffect : m_DME_TeleportParticles)
        {
            if (particleEffect)
                particleEffect.Destroy();
        }

        m_DME_TeleportParticles.Clear();
    }
}
