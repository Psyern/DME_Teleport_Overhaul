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
			SendTravelResult(sender, false, errorMsg, GetPlayerEconomyValue(player), destName, 0);
			return;
		}

		DME_TeleportDestination dest = FindDestination(destName);
		if (!dest)
		{
			SendTravelResult(sender, false, "Invalid destination.", GetPlayerEconomyValue(player), destName, 0);
			return;
		}

		if (IsTeleportBlocked(player, dest, errorMsg))
		{
			SendTravelResult(sender, false, errorMsg, GetPlayerEconomyValue(player), destName, 0);
			return;
		}

		if (!CheckQuestRequirement(player, dest, errorMsg))
		{
			SendTravelResult(sender, false, errorMsg, GetPlayerEconomyValue(player), destName, 0);
			return;
		}

		int currentTime = CF_Date.Now(true).GetTimestamp();
		int nextAvailable = GetCooldown(uid, dest.TeleportName);
		if (currentTime < nextAvailable)
		{
			int remaining = nextAvailable - currentTime;
			SendTravelResult(sender, false, "Cooldown active: " + remaining.ToString() + "s remaining.", GetPlayerEconomyValue(player), destName, nextAvailable);
			return;
		}

		int playerValue = GetPlayerEconomyValue(player);
		if (!CanAffordDestination(player, dest, playerValue))
		{
			SendTravelResult(sender, false, GetInsufficientFundsMessage(dest.Cost, playerValue), playerValue, destName, 0);
			return;
		}

		if (!TrySpendTeleportCost(player, dest.Cost))
		{
			SendTravelResult(sender, false, GetSpendFailureMessage(), GetPlayerEconomyValue(player), destName, 0);
			return;
		}

		int newNextAvailable = currentTime + dest.CooldownSec;
		SetCooldown(uid, dest.TeleportName, newNextAvailable);

		vector markerPosition = player.GetPosition();

		vector pos = dest.GetPosition();
		if (pos[1] == 0)
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		vector safePos = DME_OverhaulTeleportHelper.GetSafeTeleportPosition(pos, true);
		BroadcastTeleportPortalEffect(markerPosition, DME_Teleport_Overhaul.GUI_TELEPORT_PARTICLE_DURATION_MS, DME_Teleport_Overhaul.GUI_TELEPORT_SOUND_SET);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExecuteDelayedGUITeleport, DME_Teleport_Overhaul.GUI_TELEPORT_DELAY_MS, false, sender, player, safePos, dest.TeleportName, dest.Marker, markerPosition);

		int updatedRep = GetPlayerEconomyValue(player);
		Print("[DME_Teleport_Menu] Scheduled GUI teleport for " + sender.GetName() + " (" + uid + ") to " + destName + " at " + safePos.ToString() + " after " + DME_Teleport_Overhaul.GUI_TELEPORT_DELAY_MS.ToString() + "ms.");
		SendTravelResult(sender, true, "Teleporting to " + destName + "...", updatedRep, destName, newNextAvailable);
	}

	private void ExecuteDelayedGUITeleport(PlayerIdentity identity, PlayerBase player, vector safePos, string destName, int shouldBroadcastMarker, vector markerPosition)
	{
		if (!player || !identity)
			return;

		DME_OverhaulTeleportHelper.BeginTeleportTransition(player, DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS);
		GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_LOADING_SCREEN, new Param2<int, string>(DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS, m_GlobalMenuPreloadObjectTypes), true, identity);
		DME_OverhaulTeleportHelper.TeleportPlayerWithSafety(player, safePos, destName, 0, true);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BroadcastTeleportPortalEffect, DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS - DME_Teleport_Overhaul.TELEPORT_ARRIVAL_EFFECT_LEAD_MS, false, safePos, DME_Teleport_Overhaul.TELEPORT_ARRIVAL_EFFECT_DURATION_MS, DME_Teleport_Overhaul.GUI_TELEPORT_OUT_SOUND_SET);

		if (shouldBroadcastMarker == 1 && identity.GetName() != string.Empty)
			BroadcastGUITeleportMarker(identity.GetName(), markerPosition);
	}

	static void BroadcastTeleportPortalEffect(vector worldPosition, int durationMs, string soundSet)
	{
		GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_TELEPORT_PARTICLE, new Param3<vector, int, string>(worldPosition, durationMs, soundSet), true);
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
			reputation = GetPlayerEconomyValue(player);

		int currentTime = CF_Date.Now(true).GetTimestamp();
		string serialized = "";
		for (int i = 0; i < m_Config.Destinations.Count(); i++)
		{
			DME_TeleportDestination dest = m_Config.Destinations[i];
			int nextAvailable = GetCooldown(uid, dest.TeleportName);
			int remainingSecs = 0;
			if (currentTime < nextAvailable)
				remainingSecs = nextAvailable - currentTime;

			if (i > 0)
				serialized += ";";

			serialized += dest.TeleportName + "|" + dest.Cost.ToString() + "|" + dest.CooldownSec.ToString() + "|" + remainingSecs.ToString() + "|" + dest.Picture.ToString();
		}

		GetRPCManager().SendRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_STATE, new Param4<int, int, string, string>(reputation, m_Config.EconomyMode, serialized, GetEconomyDisplayName()), true, identity);
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

	int GetPlayerEconomyValue(PlayerBase player)
	{
		if (!player || !m_Config)
			return 0;

		switch (m_Config.EconomyMode)
		{
			case DME_Teleport_Constants.ECONOMY_MODE_ITEM:
				return GetPlayerItemBalance(player);

			case DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP:
			case DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST:
				#ifdef EXPANSIONMODHARDLINE
				return player.Expansion_GetReputation();
				#else
				return 0;
				#endif
		}

		return 0;
	}

	bool TrySpendTeleportCost(PlayerBase player, int amount)
	{
		if (!player || !m_Config || amount <= 0)
			return true;

		switch (m_Config.EconomyMode)
		{
			case DME_Teleport_Constants.ECONOMY_MODE_NONE:
			case DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP:
				return true;

			case DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST:
				#ifdef EXPANSIONMODHARDLINE
				int current = GetPlayerEconomyValue(player);
				if (current < amount)
					return false;

				player.Expansion_DecreaseReputation(amount);
				return true;
				#else
				return false;
				#endif

			case DME_Teleport_Constants.ECONOMY_MODE_ITEM:
				return TrySpendItemCurrency(player, amount);
		}

		return true;
	}

	private bool CanAffordDestination(PlayerBase player, DME_TeleportDestination dest, int playerValue)
	{
		if (!dest || !m_Config)
			return false;

		if (m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_NONE)
			return true;

		return playerValue >= dest.Cost;
	}

	private string GetEconomyDisplayName()
	{
		if (m_Config && m_Config.CurrencyDisplayName != string.Empty)
			return m_Config.CurrencyDisplayName;

		if (m_Config && (m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP || m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST))
			return "#STR_DME_TELEPORT_REPUTATION";

		return "#STR_DME_TELEPORT_BALANCE";
	}

	private string GetInsufficientFundsMessage(int required, int current)
	{
		if (m_Config && (m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP || m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST))
			return "Not enough reputation. Need " + required.ToString() + ", have " + current.ToString() + ".";

		return "Not enough funds. Need " + required.ToString() + ", have " + current.ToString() + ".";
	}

	private string GetSpendFailureMessage()
	{
		if (m_Config && m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_ITEM)
			return "Failed to deduct payment item.";

		if (m_Config && m_Config.EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST)
			return "Failed to deduct reputation.";

		return "Failed to deduct teleport cost.";
	}

	private int GetPlayerItemBalance(PlayerBase player)
	{
		if (!player || !m_Config || m_Config.CurrencyClassName == string.Empty)
			return 0;

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		int total;
		foreach (EntityAI entity : items)
		{
			ItemBase item;
			if (!Class.CastTo(item, entity))
				continue;

			if (!item.IsKindOf(m_Config.CurrencyClassName))
				continue;

			total += GetCurrencyItemAmount(item);
		}

		return total;
	}

	private bool TrySpendItemCurrency(PlayerBase player, int amount)
	{
		if (!player || !m_Config || m_Config.CurrencyClassName == string.Empty)
			return false;

		int remaining = amount;
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI entity : items)
		{
			if (remaining <= 0)
				break;

			ItemBase item;
			if (!Class.CastTo(item, entity))
				continue;

			if (!item.IsKindOf(m_Config.CurrencyClassName))
				continue;

			int itemAmount = GetCurrencyItemAmount(item);
			if (itemAmount <= 0)
				continue;

			if (item.HasQuantity())
			{
				if (itemAmount <= remaining)
				{
					remaining = remaining - itemAmount;
					GetGame().ObjectDelete(item);
				}
				else
				{
					item.SetQuantity(item.GetQuantity() - remaining);
					remaining = 0;
				}
			}
			else
			{
				remaining = remaining - 1;
				GetGame().ObjectDelete(item);
			}
		}

		return remaining <= 0;
	}

	private int GetCurrencyItemAmount(ItemBase item)
	{
		if (!item)
			return 0;

		if (item.HasQuantity())
			return (int)Math.Floor(item.GetQuantity());

		return 1;
	}

	private bool CheckQuestRequirement(PlayerBase player, DME_TeleportDestination dest, out string errorMsg)
	{
		errorMsg = "";

		if (!dest || dest.Expansion_Quest_Enable != 1)
			return true;

		if (dest.Expansion_Quest_ID < 0)
			return true;

		#ifdef EXPANSIONMODQUESTS
		if (!player || !player.GetIdentity())
			return false;

		ExpansionQuestModule questModule = ExpansionQuestModule.GetModuleInstance();
		if (!questModule)
		{
			Print("[DME_Teleport_Menu] ExpansionQuestModule not available.");
			return true;
		}

		if (!questModule.HasCompletedQuest(dest.Expansion_Quest_ID, player.GetIdentity().GetId()))
		{
			errorMsg = "You must complete the required quest (ID: " + dest.Expansion_Quest_ID.ToString() + ") before teleporting here.";
			return false;
		}
		#endif

		return true;
	}
};
