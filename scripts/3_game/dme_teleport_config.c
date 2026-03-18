class DME_TeleportDestination
{
	string TeleportName;
	ref array<float> TeleportPos;
	int Cost;
	int CooldownSec;
	int Picture;

	void DME_TeleportDestination()
	{
		TeleportPos = new array<float>;
		Picture = 1;
	}

	vector GetPosition()
	{
		if (TeleportPos && TeleportPos.Count() >= 3)
			return Vector(TeleportPos[0], TeleportPos[1], TeleportPos[2]);

		return vector.Zero;
	}
};

class DME_TeleportConfig
{
	int Version;
	int RepMode;
	ref array<ref DME_TeleportDestination> Destinations;

	void DME_TeleportConfig()
	{
		Version = 2;
		RepMode = 1;
		Destinations = new array<ref DME_TeleportDestination>;
	}
};

class DME_TeleportConfigLoader
{
	static ref DME_TeleportConfig Load()
	{
		string file = DME_Teleport_Constants.CONFIG_FILE;
		EnsureDirectories();

		ref DME_TeleportConfig config;
		if (FileExist(file))
		{
			config = new DME_TeleportConfig();
			JsonFileLoader<DME_TeleportConfig>.JsonLoadFile(file, config);

			bool migrated;
			if (config && config.Version < 2 && config.RepMode == 0)
			{
				config.RepMode = 1;
				migrated = true;
			}

			if (config && config.Version < 2)
			{
				if (config.Destinations)
				{
					for (int d = 0; d < config.Destinations.Count(); d++)
					{
						DME_TeleportDestination dest = config.Destinations[d];
						if (dest && dest.Picture <= 0)
						{
							dest.Picture = 1;
							migrated = true;
						}
					}
				}

				config.Version = 2;
				migrated = true;
			}

			if (config && config.Destinations && config.Destinations.Count() > 0)
			{
				if (migrated)
					Save(config);

				Print("[DME_Teleport_Menu] Config loaded with " + config.Destinations.Count().ToString() + " destinations.");
				return config;
			}
		}

		config = CreateDefaults();
		Save(config);
		Print("[DME_Teleport_Menu] Default config created and saved.");
		return config;
	}

	static void Save(DME_TeleportConfig config)
	{
		EnsureDirectories();
		JsonFileLoader<DME_TeleportConfig>.JsonSaveFile(DME_Teleport_Constants.CONFIG_FILE, config);
	}

	static void EnsureDirectories()
	{
		DME_Teleport_Overhaul.EnsureDirectories();
	}

	static ref DME_TeleportConfig CreateDefaults()
	{
		ref DME_TeleportConfig config = new DME_TeleportConfig();
		config.Version = 2;
		config.RepMode = 1;

		ref DME_TeleportDestination greenMtn = new DME_TeleportDestination();
		greenMtn.TeleportName = "Green Mountain";
		greenMtn.TeleportPos = { 3700.51, 0.0, 5981.27 };
		greenMtn.Cost = 1200;
		greenMtn.CooldownSec = 1700;
		greenMtn.Picture = 1;

		ref DME_TeleportDestination krasno = new DME_TeleportDestination();
		krasno.TeleportName = "Krasno Airfield";
		krasno.TeleportPos = { 11880.40, 0.0, 12460.74 };
		krasno.Cost = 1800;
		krasno.CooldownSec = 1500;
		krasno.Picture = 2;

		config.Destinations.Insert(greenMtn);
		config.Destinations.Insert(krasno);
		return config;
	}
};
