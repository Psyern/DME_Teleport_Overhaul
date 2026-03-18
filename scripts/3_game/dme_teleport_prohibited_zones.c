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
		WarningMessage = "Teleporting is not allowed in this zone.";
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

	ref array<vector> GetPolygonPositions()
	{
		ref array<vector> positions = new array<vector>;

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

class DME_ProhibitedZonesConfig
{
	int VersionID;
	ref array<ref DME_ProhibitedTeleportZone> ProhibitedTeleportZones;

	void DME_ProhibitedZonesConfig()
	{
		VersionID = 1;
		ProhibitedTeleportZones = new array<ref DME_ProhibitedTeleportZone>;
	}
};

class DME_ProhibitedZonesConfigLoader
{
	static const int CURRENT_VERSION_ID = 1;

	static ref DME_ProhibitedZonesConfig Load()
	{
		string filePath = DME_Teleport_Constants.PROHIBITED_ZONES_FILE;
		EnsureDirectories();

		ref DME_ProhibitedZonesConfig config;
		if (FileExist(filePath))
		{
			config = new DME_ProhibitedZonesConfig();
			JsonFileLoader<DME_ProhibitedZonesConfig>.JsonLoadFile(filePath, config);

			if (!config)
				config = new DME_ProhibitedZonesConfig();

			bool migrated = Migrate(config);
			if (migrated)
				Save(config);

			Print("[DME_Teleport_Menu] Prohibited zones loaded: " + config.ProhibitedTeleportZones.Count().ToString());
			return config;
		}

		config = CreateDefaults();
		Save(config);
		Print("[DME_Teleport_Menu] Default prohibited zone config created.");
		return config;
	}

	static void Save(DME_ProhibitedZonesConfig config)
	{
		EnsureDirectories();
		JsonFileLoader<DME_ProhibitedZonesConfig>.JsonSaveFile(DME_Teleport_Constants.PROHIBITED_ZONES_FILE, config);
	}

	static void EnsureDirectories()
	{
		DME_Teleport_Overhaul.EnsureDirectories();
	}

	static ref DME_ProhibitedZonesConfig CreateDefaults()
	{
		ref DME_ProhibitedZonesConfig config = new DME_ProhibitedZonesConfig();
		config.VersionID = CURRENT_VERSION_ID;
		return config;
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
				zone.WarningMessage = "Teleporting is not allowed in this zone.";
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
