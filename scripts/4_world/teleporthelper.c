class DME_OverhaulTeleportHelper
{
    // Функция для создания объекта для взаимодействия
    static void CreateTeleportInteractionObject(string objectType, vector objectCoordinates, vector objectOrientation)
    {
        // Создаем видимый объект
        Object obj = GetGame().CreateObjectEx(objectType, objectCoordinates, ECE_NOSURFACEALIGN);
        // Object obj = GetGame().CreateObject(objectType, vector.Zero);
        if (obj)
        {
            // Применяем ориентацию к объекту
            obj.SetOrientation(objectOrientation);
            Print("Object " + objectType + " created at position: " + objectCoordinates.ToString());

            // Спавним невидимый объект для взаимодействия
            DME_Teleport_OverhaulPoint interactionObj = GetGame().CreateObjectEx("DME_Teleport_OverhaulPoint", objectCoordinates, ECE_NOSURFACEALIGN, RF_IGNORE);
            if (interactionObj)
            {
                // ActionInteractBase action = new DME_Teleport_OverhaulAction();
                // interactionObj.AddAction(DME_Teleport_OverhaulAction);
                Print("Object DME_Teleport_OverhaulPoint created at position: " + objectCoordinates.ToString() + " " + interactionObj);
            } else {
                Print("Failed to create object DME_Teleport_OverhaulPoint at position: " + objectCoordinates.ToString());
            }
        }
        else
        {
            Print("Failed to create object " + objectType + " at position: " + objectCoordinates.ToString());
        }
    }

    static void TeleportPlayerDelayed(PlayerBase player, vector teleportPosition, string teleportName, int cooldownTime)
    {
        if (!player)
            return;

        player.SetPosition(teleportPosition);
        DME_OverhaulSetTeleportCooldown(teleportName, cooldownTime);
    }

    static void BeginTeleportTransition(PlayerBase player, int durationMs)
    {
        if (!player)
            return;

        player.DME_OverhaulBeginTeleportTransition();

        if (durationMs > 0)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EndTeleportTransition, durationMs, false, player);
    }

    static void EndTeleportTransition(PlayerBase player)
    {
        if (!player)
            return;

        player.DME_OverhaulEndTeleportTransition();
    }

    static vector GetSafeTeleportPosition(vector teleportPosition, bool useSurfaceSafety)
    {
        vector safePosition = teleportPosition;

        if (!useSurfaceSafety)
            return safePosition;

        float surfaceY = g_Game.SurfaceY(safePosition[0], safePosition[2]);
        if (safePosition[1] < surfaceY + DME_Teleport_Overhaul.TELEPORT_SURFACE_OFFSET)
            safePosition[1] = surfaceY + DME_Teleport_Overhaul.TELEPORT_SURFACE_OFFSET;

        return safePosition;
    }

    static void TeleportPlayerWithSafety(PlayerBase player, vector teleportPosition, string teleportName, int cooldownTime, bool useSurfaceSafety)
    {
        if (!player)
            return;

        vector safePosition = GetSafeTeleportPosition(teleportPosition, useSurfaceSafety);
        TeleportPlayerDelayed(player, safePosition, teleportName, cooldownTime);

        if (useSurfaceSafety)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnsurePlayerAboveGround, DME_Teleport_Overhaul.TELEPORT_POSITION_RECHECK_DELAY_MS, false, player, safePosition);
    }

    static void EnsurePlayerAboveGround(PlayerBase player, vector expectedPosition)
    {
        if (!player)
            return;

        vector currentPosition = player.GetPosition();
        if (vector.Distance(currentPosition, expectedPosition) > 5.0)
            return;

        float surfaceY = g_Game.SurfaceY(currentPosition[0], currentPosition[2]);
        if (currentPosition[1] < surfaceY + DME_Teleport_Overhaul.TELEPORT_SURFACE_OFFSET)
        {
            currentPosition[1] = surfaceY + DME_Teleport_Overhaul.TELEPORT_SURFACE_OFFSET;
            player.SetPosition(currentPosition);
        }
    }
}
