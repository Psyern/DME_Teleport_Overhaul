class DME_TeleportManager
{
	private static ref DME_TeleportManager s_Instance;
	private ref DME_TeleportConfig m_Config;
	private ref DME_ProhibitedZonesConfig m_ProhibitedZonesConfig;
	private string m_GlobalMenuPreloadObjectTypes;
	private ref map<string, ref map<string, int>> m_Cooldowns;
	private ref map<string, int> m_LastRequestTime;

	void DME_TeleportManager()
	{
		m_Cooldowns = new map<string, ref map<string, int>>;
		m_LastRequestTime = new map<string, int>;

		GetRPCManager().AddRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_REQUEST, this, SingleplayerExecutionType.Server);
		GetRPCManager().AddRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_REQUEST, this, SingleplayerExecutionType.Server);

		if (GetGame().IsServer())
		{
			m_Config = DME_TeleportConfigLoader.Load();
			m_ProhibitedZonesConfig = DME_ProhibitedZonesConfigLoader.Load();
			m_GlobalMenuPreloadObjectTypes = DME_OverhaulJsonConfigManager.GetSerializedGlobalPreloadObjectTypes();
		}

		Print("[DME_Teleport_Menu] Manager initialized (server=" + GetGame().IsServer().ToString() + ").");
	}

	static DME_TeleportManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DME_TeleportManager();
		return s_Instance;
	}

	static void DestroyInstance()
	{
		s_Instance = null;
	}

	void RPC_TravelRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Server || !sender)
			return;

		Param1<string> data = new Param1<string>("");
		if (!ctx.Read(data))
			return;

		string destName = data.param1;
		string uid = sender.GetId();
		int nowMS = GetGame().GetTime();

		if (m_LastRequestTime.Contains(uid))
		{
			int lastTime = m_LastRequestTime.Get(uid);
			if ((nowMS - lastTime) < DME_Teleport_Constants.TRAVEL_REQUEST_COOLDOWN_MS)
			{
				SendTravelResult(sender, false, "Request too fast. Please wait.", 0, destName, 0);
				return;
			}
		}
		m_LastRequestTime.Set(uid, nowMS);

		PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
		if (!player)
		{
			SendTravelResult(sender, false, "Player not found.", 0, destName, 0);
			return;
		}

		string errorMsg;
		if (!ValidatePlayerState(player, errorMsg))
		{
			SendTravelResult(sender, false, errorMsg, GetPlayerReputation(player), destName, 0);
			return;
		}

		DME_TeleportDestination dest = FindDestination(destName);
		if (!dest)
		{
			SendTravelResult(sender, false, "Invalid destination.", GetPlayerReputation(player), destName, 0);
			return;
		}

		if (IsTeleportBlocked(player, dest, errorMsg))
		{
			SendTravelResult(sender, false, errorMsg, GetPlayerReputation(player), destName, 0);
			return;
		}

		int currentTime = CF_Date.Now(true).GetTimestamp();
		int nextAvailable = GetCooldown(uid, dest.TeleportName);
		if (currentTime < nextAvailable)
		{
			int remaining = nextAvailable - currentTime;
			SendTravelResult(sender, false, "Cooldown active: " + remaining.ToString() + "s remaining.", GetPlayerReputation(player), destName, nextAvailable);
			return;
		}

		int playerRep = GetPlayerReputation(player);
		if (playerRep < dest.Cost)
		{
			SendTravelResult(sender, false, "Not enough reputation. Need " + dest.Cost.ToString() + ", have " + playerRep.ToString() + ".", playerRep, destName, 0);
			return;
		}

		if (m_Config && m_Config.RepMode == 1)
		{
			if (!TrySpendReputation(player, dest.Cost))
			{
				SendTravelResult(sender, false, "Failed to deduct reputation.", playerRep, destName, 0);
				return;
			}
		}

		int newNextAvailable = currentTime + dest.CooldownSec;
		SetCooldown(uid, dest.TeleportName, newNextAvailable);

		vector markerPosition = player.GetPosition();

		vector pos = dest.GetPosition();
		if (pos[1] == 0)
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		vector safePos = DME_OverhaulTeleportHelper.GetSafeTeleportPosition(pos, true);
		GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_TELEPORT_PARTICLE, new Param2<vector, int>(markerPosition, DME_Teleport_Overhaul.GUI_TELEPORT_PARTICLE_DURATION_MS), true);
		DME_OverhaulTeleportHelper.BeginTeleportTransition(player, DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS);

		if (sender)
			GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_LOADING_SCREEN, new Param2<int, string>(DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS, m_GlobalMenuPreloadObjectTypes), true, sender);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DME_OverhaulTeleportHelper.TeleportPlayerWithSafety, DME_Teleport_Overhaul.GUI_TELEPORT_DELAY_MS, false, player, safePos, dest.TeleportName, 0, true);

		if (dest.Marker == 1 && sender && sender.GetName() != string.Empty)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DME_TeleportManager.BroadcastGUITeleportMarker, DME_Teleport_Overhaul.GUI_TELEPORT_DELAY_MS, false, sender.GetName(), markerPosition);

		int updatedRep = GetPlayerReputation(player);
		Print("[DME_Teleport_Menu] Scheduled teleport for " + sender.GetName() + " (" + uid + ") to " + destName + " at " + safePos.ToString() + " after " + DME_Teleport_Overhaul.GUI_TELEPORT_DELAY_MS.ToString() + "ms while loading screen stays visible.");
		SendTravelResult(sender, true, "Teleporting to " + destName + "...", updatedRep, destName, newNextAvailable);
	}

	static void BroadcastGUITeleportMarker(string playerName, vector worldPosition)
	{
		if (playerName == string.Empty)
			return;

		GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_TELEPORT_MARKER, new Param3<string, vector, int>(playerName, worldPosition, DME_Teleport_Overhaul.GUI_TELEPORT_MARKER_DURATION_MS), true);
	}

	void RPC_SyncRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Server || !sender)
			return;
		RequestSyncState(sender);
	}

	void RequestSyncState(PlayerIdentity identity)
	{
		if (!identity || !m_Config)
			return;

		string uid = identity.GetId();
		PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
		int reputation = 0;
		if (player)
			reputation = GetPlayerReputation(player);

		int currentTime = CF_Date.Now(true).GetTimestamp();
		string serialized = "";
		for (int i = 0; i < m_Config.Destinations.Count(); i++)
		{
			DME_TeleportDestination dest = m_Config.Destinations[i];
			int nextAvailable = GetCooldown(uid, dest.TeleportName);
			int remainingSecs;
			if (currentTime < nextAvailable)
				remainingSecs = nextAvailable - currentTime;

			if (i > 0)
				serialized += ";";

			serialized += dest.TeleportName + "|" + dest.Cost.ToString() + "|" + dest.CooldownSec.ToString() + "|" + remainingSecs.ToString() + "|" + dest.Picture.ToString();
		}

		GetRPCManager().SendRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_STATE, new Param3<int, int, string>(reputation, m_Config.RepMode, serialized), true, identity);
	}

	private void SendTravelResult(PlayerIdentity identity, bool success, string message, int updatedRep, string destName, int nextAvailableTime)
	{
		int successInt;
		if (success)
			successInt = 1;

		GetRPCManager().SendRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_RESULT, new Param5<int, string, int, string, int>(successInt, message, updatedRep, destName, nextAvailableTime), true, identity);
	}

	private bool ValidatePlayerState(PlayerBase player, out string errorMsg)
	{
		if (!player.IsAlive())
		{
			errorMsg = "You are dead.";
			return false;
		}

		if (player.IsUnconscious())
		{
			errorMsg = "You are unconscious.";
			return false;
		}

		if (player.IsRestrained())
		{
			errorMsg = "You are restrained.";
			return false;
		}

		HumanCommandVehicle vehicleCmd = player.GetCommand_Vehicle();
		if (vehicleCmd)
		{
			errorMsg = "You cannot teleport from a vehicle.";
			return false;
		}

		errorMsg = "";
		return true;
	}

	private bool IsTeleportBlocked(PlayerBase player, DME_TeleportDestination dest, out string errorMsg)
	{
		errorMsg = "";

		if (!player || !dest)
			return false;

		if (IsPositionInsideProhibitedZone(player.GetPosition(), false, errorMsg))
			return true;

		vector destinationPosition = dest.GetPosition();
		if (destinationPosition[1] == 0)
			destinationPosition[1] = GetGame().SurfaceY(destinationPosition[0], destinationPosition[2]);

		if (IsPositionInsideProhibitedZone(destinationPosition, true, errorMsg))
			return true;

		return false;
	}

	private bool IsPositionInsideProhibitedZone(vector position, bool isDestination, out string errorMsg)
	{
		errorMsg = "";

		if (!m_ProhibitedZonesConfig || !m_ProhibitedZonesConfig.ProhibitedTeleportZones)
			return false;

		for (int zoneIndex = 0; zoneIndex < m_ProhibitedZonesConfig.ProhibitedTeleportZones.Count(); zoneIndex++)
		{
			DME_ProhibitedTeleportZone zone = m_ProhibitedZonesConfig.ProhibitedTeleportZones[zoneIndex];
			if (!zone)
				continue;

			if (!IsInsideProhibitedZone(position, zone))
				continue;

			errorMsg = GetZoneWarningMessage(zone, isDestination);
			return true;
		}

		return false;
	}

	private bool IsInsideProhibitedZone(vector position, DME_ProhibitedTeleportZone zone)
	{
		if (!zone)
			return false;

		if (zone.HasValidPolygon())
			return IsInsidePolygonZone(position, zone.GetPolygonPositions());

		if (zone.Radius <= 0)
			return false;

		vector poi = zone.GetPOIPosition();
		vector flatPosition = Vector(position[0], 0, position[2]);
		vector flatPoi = Vector(poi[0], 0, poi[2]);
		return vector.Distance(flatPosition, flatPoi) <= zone.Radius;
	}

	private bool IsInsidePolygonZone(vector position, array<vector> polygonPoints)
	{
		if (!polygonPoints || polygonPoints.Count() < 3)
			return false;

		bool inside;
		float px = position[0];
		float pz = position[2];
		int pointCount = polygonPoints.Count();

		for (int pointIndex = 0, previousIndex = pointCount - 1; pointIndex < pointCount; previousIndex = pointIndex++)
		{
			vector currentPoint = polygonPoints[pointIndex];
			vector previousPoint = polygonPoints[previousIndex];
			float currentX = currentPoint[0];
			float currentZ = currentPoint[2];
			float previousX = previousPoint[0];
			float previousZ = previousPoint[2];

			bool intersects = ((currentZ > pz) != (previousZ > pz));
			if (!intersects)
				continue;

			float slope = (previousX - currentX) / (previousZ - currentZ);
			float intersectX = currentX + slope * (pz - currentZ);
			if (px < intersectX)
				inside = !inside;
		}

		return inside;
	}

	private string GetZoneWarningMessage(DME_ProhibitedTeleportZone zone, bool isDestination)
	{
		if (zone && zone.WarningMessage != string.Empty)
			return zone.WarningMessage;

		if (isDestination)
			return "#STR_DME_TELEPORT_WARNING_DESTINATION";

		return "#STR_DME_TELEPORT_WARNING_ZONE";
	}

	private DME_TeleportDestination FindDestination(string name)
	{
		if (!m_Config || !m_Config.Destinations)
			return null;

		for (int i = 0; i < m_Config.Destinations.Count(); i++)
		{
			if (m_Config.Destinations[i].TeleportName == name)
				return m_Config.Destinations[i];
		}

		return null;
	}

	private int GetCooldown(string uid, string destName)
	{
		if (!m_Cooldowns.Contains(uid))
			return 0;

		map<string, int> playerCooldowns = m_Cooldowns.Get(uid);
		if (!playerCooldowns || !playerCooldowns.Contains(destName))
			return 0;

		return playerCooldowns.Get(destName);
	}

	private void SetCooldown(string uid, string destName, int nextAvailableTime)
	{
		if (!m_Cooldowns.Contains(uid))
			m_Cooldowns.Set(uid, new map<string, int>);

		map<string, int> playerCooldowns = m_Cooldowns.Get(uid);
		playerCooldowns.Set(destName, nextAvailableTime);
	}

	int GetPlayerReputation(PlayerBase player)
	{
		if (!player)
			return 0;

		return player.Expansion_GetReputation();
	}

	bool TrySpendReputation(PlayerBase player, int amount)
	{
		if (!player)
			return false;

		int current = GetPlayerReputation(player);
		if (current < amount)
			return false;

		player.Expansion_DecreaseReputation(amount);
		return true;
	}
};
