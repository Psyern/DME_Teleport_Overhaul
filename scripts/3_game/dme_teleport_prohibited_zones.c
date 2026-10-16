class DME_ProhibitedTeleportZone
{
	ref array<float> POI;
	float Radius;
	string WarningMessage;
	bool PolygonZoneEnable;
	ref array<ref array<float>> PolygonVectors;

	void DME_ProhibitedTeleportZone()
	{
		POI = new array<float>;
		PolygonVectors = new array<ref array<float>>;
		WarningMessage = "#STR_DME_TELEPORT_WARNING_ZONE";
	}

	vector GetPOIPosition()
	{
		if (POI && POI.Count() >= 3)
			return Vector(POI[0], POI[1], POI[2]);

		return vector.Zero;
	}

	bool HasValidPolygon()
	{
		return PolygonZoneEnable && PolygonVectors && PolygonVectors.Count() >= 3;
	}

	array<vector> GetPolygonPositions()
	{
		array<vector> positions = new array<vector>;

		if (!PolygonVectors)
			return positions;

		for (int index = 0; index < PolygonVectors.Count(); index++)
		{
			array<float> polygonPoint = PolygonVectors[index];
			if (!polygonPoint || polygonPoint.Count() < 3)
				continue;

			positions.Insert(Vector(polygonPoint[0], polygonPoint[1], polygonPoint[2]));
		}

		return positions;
	}
};

class DME_ProhibitedZonesConfig : PsyCore_ConfigBase
{
	int VersionID;
	ref array<ref DME_ProhibitedTeleportZone> ProhibitedTeleportZones;

	void DME_ProhibitedZonesConfig()
	{
		VersionID = 1;
		ProhibitedTeleportZones = new array<ref DME_ProhibitedTeleportZone>;
	}

	// PsyCore_ConfigBase: the version field keeps its existing JSON name "VersionID"
	override int GetVersion()
	{
		return VersionID;
	}

	override void SetVersion(int version)
	{
		VersionID = version;
	}

	override int GetCurrentVersion()
	{
		return DME_ProhibitedZonesConfigLoader.CURRENT_VERSION_ID;
	}

	override void OnMigrate(int fromVersion)
	{
		DME_ProhibitedZonesConfigLoader.MarkChanged();
	}

	// Called after every load: repair + example zones for an empty list (unchanged logic)
	override void OnValidate()
	{
		if (DME_ProhibitedZonesConfigLoader.Repair(this))
			DME_ProhibitedZonesConfigLoader.MarkChanged();
	}
};

class DME_ProhibitedZonesConfigLoader
{
	static const int CURRENT_VERSION_ID = 1;
	// set by DME_ProhibitedZonesConfig.OnMigrate/OnValidate: the file is only rewritten when something changed
	protected static bool s_DME_Changed;

	static void MarkChanged()
	{
		s_DME_Changed = true;
	}

	static DME_ProhibitedZonesConfig Load()
	{
		string filePath = DME_Teleport_Constants.PROHIBITED_ZONES_FILE;
		EnsureDirectories();

		bool existed = FileExist(filePath);
		s_DME_Changed = false;
		DME_ProhibitedZonesConfig config;
		// writeBack=false: existing files are only rewritten after a migration/repair (as before)
		bool loaded = PsyCore_ConfigLoader<DME_ProhibitedZonesConfig>.Load(filePath, config, "", "DME_Teleport_Menu", false);
		if (loaded)
		{
			if (s_DME_Changed)
				Save(config);

			PsyCore_Log.Get("DME_Teleport_Menu").Info("Prohibited zones loaded: " + config.ProhibitedTeleportZones.Count().ToString());
			return config;
		}

		if (existed)
		{
			// a broken file is never overwritten: the admin must be able to fix it
			PsyCore_Log.Get("DME_Teleport_Menu").Error("Prohibited zones " + filePath + " could not be read, using the example zones until it is fixed.");
			return config;
		}

		Save(config);
		PsyCore_Log.Get("DME_Teleport_Menu").Info("Default prohibited zone config created.");
		return config;
	}

	// Migrate + example zones; true when the config was changed
	static bool Repair(DME_ProhibitedZonesConfig config)
	{
		if (!config)
			return false;

		bool changed = Migrate(config);
		if (EnsureExampleZones(config))
			changed = true;

		return changed;
	}

	static void Save(DME_ProhibitedZonesConfig config)
	{
		EnsureDirectories();
		string saveError;
		if (!JsonFileLoader<DME_ProhibitedZonesConfig>.SaveFile(DME_Teleport_Constants.PROHIBITED_ZONES_FILE, config, saveError))
			Print("[DME_Teleport_Menu] " + saveError);
	}

	static void EnsureDirectories()
	{
		DME_Teleport_Overhaul.EnsureDirectories();
	}

	static DME_ProhibitedZonesConfig CreateDefaults()
	{
		DME_ProhibitedZonesConfig config = new DME_ProhibitedZonesConfig();
		config.VersionID = CURRENT_VERSION_ID;
		EnsureExampleZones(config);
		return config;
	}

	protected static bool EnsureExampleZones(DME_ProhibitedZonesConfig config)
	{
		if (!config)
			return false;

		if (!config.ProhibitedTeleportZones)
			config.ProhibitedTeleportZones = new array<ref DME_ProhibitedTeleportZone>;

		if (config.ProhibitedTeleportZones.Count() > 0)
			return false;

		config.ProhibitedTeleportZones.Insert(CreateExampleRadiusZone());
		config.ProhibitedTeleportZones.Insert(CreateExamplePolygonZone());
		return true;
	}

	protected static DME_ProhibitedTeleportZone CreateExampleRadiusZone()
	{
		DME_ProhibitedTeleportZone zone = new DME_ProhibitedTeleportZone();
		zone.POI = { 2731.57, 0.0, 1255.94 };
		zone.Radius = 250;
		zone.WarningMessage = "#STR_DME_TELEPORT_WARNING_RADIUS_EXAMPLE";
		zone.PolygonZoneEnable = false;
		return zone;
	}

	protected static DME_ProhibitedTeleportZone CreateExamplePolygonZone()
	{
		DME_ProhibitedTeleportZone zone = new DME_ProhibitedTeleportZone();
		zone.WarningMessage = "#STR_DME_TELEPORT_WARNING_POLYGON_EXAMPLE";
		zone.PolygonZoneEnable = true;
		zone.PolygonVectors.Insert({ 3616.18, 0.0, 2041.39 });
		zone.PolygonVectors.Insert({ 3662.84, 0.0, 2486.61 });
		zone.PolygonVectors.Insert({ 4444.40, 0.0, 2331.07 });
		zone.PolygonVectors.Insert({ 4331.63, 0.0, 2134.71 });
		return zone;
	}

	protected static bool Migrate(DME_ProhibitedZonesConfig config)
	{
		bool migrated;

		if (!config.ProhibitedTeleportZones)
		{
			config.ProhibitedTeleportZones = new array<ref DME_ProhibitedTeleportZone>;
			migrated = true;
		}

		for (int zoneIndex = 0; zoneIndex < config.ProhibitedTeleportZones.Count(); zoneIndex++)
		{
			DME_ProhibitedTeleportZone zone = config.ProhibitedTeleportZones[zoneIndex];
			if (!zone)
			{
				zone = new DME_ProhibitedTeleportZone();
				config.ProhibitedTeleportZones.Set(zoneIndex, zone);
				migrated = true;
			}

			if (!zone.POI)
			{
				zone.POI = new array<float>;
				migrated = true;
			}

			if (!zone.PolygonVectors)
			{
				zone.PolygonVectors = new array<ref array<float>>;
				migrated = true;
			}

			if (zone.Radius < 0)
			{
				zone.Radius = 0;
				migrated = true;
			}

			if (zone.WarningMessage == string.Empty)
			{
				zone.WarningMessage = "#STR_DME_TELEPORT_WARNING_ZONE";
				migrated = true;
			}

			if (zone.PolygonZoneEnable && zone.PolygonVectors.Count() < 3)
			{
				zone.PolygonZoneEnable = false;
				migrated = true;
			}
		}

		if (config.VersionID != CURRENT_VERSION_ID)
		{
			config.VersionID = CURRENT_VERSION_ID;
			migrated = true;
		}

		return migrated;
	}
};
