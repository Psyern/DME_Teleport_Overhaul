// Хранение времени последней активации каждого телепорта
static ref map<string, float> g_teleportCooldowns = new map<string, float>();

static bool IsTeleportOnCooldown(string teleportName, int cooldownTime) {
    // Если кулдаун 0, всегда разрешаем телепортацию
    if (cooldownTime == 0) {
        return false;
    }

    float currentTime = GetGame().GetTime() / 1000; // Текущее время в секундах
    if (g_teleportCooldowns.Contains(teleportName)) {
        float lastUsed = g_teleportCooldowns.Get(teleportName);
        if ((currentTime - lastUsed) < cooldownTime) {
            return true;
        }
    }
    return false;
}

static void SetTeleportCooldown(string teleportName, int cooldownTime) {
    if (cooldownTime > 0) { // Устанавливаем кулдаун только если он больше 0
        float currentTime = GetGame().GetTime() / 1000; // Текущее время в секундах
        g_teleportCooldowns.Set(teleportName, currentTime);
    }
}

autoptr ref TStringArray g_activatedActionTeleports = {};
static ref TStringArray GetActivatedActionTeleports()
{
	if (!g_activatedActionTeleports) g_activatedActionTeleports = {};
	return g_activatedActionTeleports;
}

static int SearchActivatedActionTeleport(string teleportName) {
    foreach (auto key, auto name: g_activatedActionTeleports)
    {
        if (name == teleportName) {
            return key;
        }
    }
    return -1;
}

static void InsertActivatedActionTeleport(string teleportName) {
    if (SearchActivatedActionTeleport(teleportName) < 0) {
        g_activatedActionTeleports.Insert(teleportName);
    }
}

static void RemoveActivatedActionTeleport(string teleportName) {
    int index = SearchActivatedActionTeleport(teleportName);
    if (index >= 0) {
        g_activatedActionTeleports.Remove(index);
    }
}

class DME_Teleport_OverhaulActionCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(3);
	}
};
class DME_Teleport_OverhaulAction: ActionContinuousBase
{
    protected ref LO_TeleportConfig m_TeleportConfig;  // Ссылка на объект конфигурации телепорта

    void DME_Teleport_OverhaulAction()
    {
        m_CallbackClass = DME_Teleport_OverhaulActionCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_RESTRAINTARGET; //CMD_ACTIONFB_INTERACT;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_FullBody			= true;
        m_Text = "#STR_Controls_Interact";
        LoadTeleportConfig();  // Загружаем конфигурацию телепортов
    }

    protected void LoadTeleportConfig()
    {
        m_TeleportConfig = JsonConfigManager.LoadTeleportConfig();  // Загружаем конфигурацию с помощью JsonConfigManager
    }

    override void CreateConditionComponents()
    {
		m_ConditionTarget = new CCTBase; //CCTCursorParent(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
    }

    // override string GetText()
    // {
    //     return "Взаимодействовать";
    // }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return true;
    }
	
	override void OnStartServer(ActionData action_data ) {}

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        ActionTarget target = action_data.m_Target;
        Object targetObject = target.GetObject();
        LO_TeleportEntry entry = GetTeleportEntry(targetObject);

        // Логика телепортации
        if (entry) {
            bool canTeleport = false;
            if (IsTeleportOnCooldown(entry.TeleportName, entry.TeleportCooldownSeconds)) {
                player.MessageStatus("Этот телепорт на кулдауне, попробуйте позже!");
                return;
            }            
            if (entry.EnableTeleport) {
                if ("DME_Teleport_OverhaulPoint" == targetObject.GetType() && vector.Distance(targetObject.GetPosition(), player.GetPosition()) <= entry.CheckRadius)
                {
                    if (SearchActivatedActionTeleport(entry.TeleportName) < 0) {
                        if (entry.RequiredItem != "") {
                            ItemBase heldItem = player.GetItemInHands();
                            if (heldItem && heldItem.GetType() == entry.RequiredItem)
                            {
                                int healthLevel = heldItem.GetHealthLevel();
                                
                                // Проверяем, не разрушен ли предмет
                                if (healthLevel < GameConstants.STATE_RUINED) {
                                    int currentHealth = heldItem.GetHealth();
                                    int maxHealth = heldItem.GetMaxHealth();
                                    
                                    // Рассчитываем повреждения и наносим их
                                    float damage = Math.Floor(maxHealth / 100 * entry.RequiredItemDamagePercent);
                                    heldItem.SetHealth(currentHealth - damage); // Наносим повреждения

                                    // Логирование повреждения
                                    Print(string.Format("Item damaged by %1%%, remaining health: %2/%3", entry.RequiredItemDamagePercent, heldItem.GetHealth(), maxHealth));
                                    
                                    // Если здоровье предмета уменьшилось до 0 или ниже, то он разрушен
                                    if (heldItem.GetHealth() <= 0) {
                                        Print("The item has been destroyed.");
                                        canTeleport = true;  // Разрешаем телепортацию
                                    } else {
                                        // Если предмет не разрушен, но поврежден, то тоже можно телепортировать
                                        canTeleport = true;
                                    }

                                    // Если время активации телепорта больше 0, мы продолжаем
                                    if (entry.TeleportActiveTimeSeconds > 0) {
                                        InsertActivatedActionTeleport(entry.TeleportName);
                                        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveActivatedActionTeleport, entry.TeleportActiveTimeSeconds * 1000, false, entry.TeleportName);    
                                    }
                                } else {
                                    Print("Item is already ruined, no further damage required.");
                                }
                            }
                        } else {
                            canTeleport = true;
                        }
                    } else {
                        canTeleport = true;
                    }
                }

                if (canTeleport) {
                    // Логирование использования телепорта
                    string steamID;
                    if (player.GetIdentity()) {
                        steamID = player.GetIdentity().GetId();
                    } else {
                        steamID = "Unknown";
                    }

                    string playerName;
                    if (player.GetIdentity()) {
                        playerName = player.GetIdentity().GetName();
                    } else {
                        playerName = "Unknown";
                    }

                    string teleportName = entry.TeleportName;
                    vector teleportPosition = entry.TeleportPosition;
                    string preloadObjectTypes = DME_Teleport_Overhaul.SerializePreloadObjectTypes(entry.PreloadObjectTypes);

                    // Добавляем случайное смещение в радиусе ±1 метр
                    float randomX = Math.RandomFloatInclusive(-1, 1);
                    float randomZ = Math.RandomFloatInclusive(-1, 1);
                    vector randomTeleportPosition = Vector(teleportPosition[0] + randomX, teleportPosition[1], teleportPosition[2] + randomZ);

                    vector startPosition = targetObject.GetPosition();

                    string logMessage = string.Format("[Teleport] Player: %1 (GUID: %2) teleported from %3 (%4) to %5",
                        playerName, steamID, startPosition.ToString(), teleportName, randomTeleportPosition.ToString());

                    Print(logMessage);  // Логирование в консоль

                    // Убедимся, что папка существует
                    DME_Teleport_Overhaul.EnsureDirectories();

                    // Логирование в файл
                    string filePath = DME_Teleport_Overhaul.LOG_FILE;
                    FileHandle logFile = OpenFile(filePath, FileMode.APPEND);
                    if (logFile) {
                        FPrint(logFile, logMessage + "\n");
                        CloseFile(logFile);
                    }

                    // Телепортация игрока в случайную точку
                    if (player.GetIdentity()) {
                        GetRPCManager().SendRPC(DME_Teleport_Overhaul.RPC_NAMESPACE, DME_Teleport_Overhaul.RPC_SHOW_LOADING_SCREEN, new Param2<int, string>(DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS, preloadObjectTypes), true, player.GetIdentity());
                    }

                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TeleportHelper.TeleportPlayerWithSafety, DME_Teleport_Overhaul.LOADING_SCREEN_DURATION_MS, false, player, randomTeleportPosition, entry.TeleportName, entry.TeleportCooldownSeconds, entry.UseSurfaceSafety);
                } else {
                    // Уведомление о нехватке предмета
                    player.MessageStatus(entry.MissingItemMessage);
                } 
            }
        }
    }

    LO_TeleportEntry GetTeleportEntry(Object targetObject) {
        foreach (ref LO_TeleportEntry entry : m_TeleportConfig.TeleportEntries)
        {
            if (vector.Distance(targetObject.GetPosition(), entry.ObjectCoordinates) < 3) {
                return entry;
            }
        }
        return null;
    }
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasProgress()
	{
		return true;
	}
	
	override bool HasTarget()
	{
		return true;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return false;
	}
};

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        
        // Регистрируем новое действие
        actions.Insert(DME_Teleport_OverhaulAction);
    }
};

class DME_Teleport_OverhaulPoint extends ItemBase
{
    void DME_Teleport_OverhaulPoint()
    {
        SetSynchDirty();
        SetLifetime(600); // Время жизни объекта, если необходимо
    }
    override void SetActions()
    {
        super.SetActions();		
        AddAction(DME_Teleport_OverhaulAction);
    }

    string Get_StaticReturnItem()
    {
        return "DME_Teleport_OverhaulPoint";
    }
    
    override bool IsHealthVisible()
    {
        return false;
    }
    override bool IsBuilding()
    {
        return true;
    }
    override bool IsInventoryItem()
    {
        return false;
    }
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo(parent) ) {return false;}
        return false;
    }

    override bool CanPutIntoHands( EntityAI player )
    {
        if( !super.CanPutIntoHands( parent ) )
        {
            return false;
        }
        return false;
    }

};