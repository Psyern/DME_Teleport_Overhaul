class DME_TeleportDestination
{
	string TeleportName;
	ref array<float> TeleportPos;
	int Cost;
	int CooldownSec;
	int Picture;
	int Marker;
	int Expansion_Quest_Enable;
	int Expansion_Quest_ID;

	void DME_TeleportDestination()
	{
		TeleportPos = new array<float>;
		Picture = 1;
		Marker = 1;
		Expansion_Quest_Enable = 0;
		Expansion_Quest_ID = -1;
	}

	vector GetPosition()
	{
		if (TeleportPos && TeleportPos.Count() >= 3)
			return Vector(TeleportPos[0], TeleportPos[1], TeleportPos[2]);

		return vector.Zero;
	}
};

class DME_TeleportConfig : PsyCore_ConfigBase
{
	int Version;
	int RepMode;
	int EconomyMode;
	string CurrencyClassName;
	string CurrencyDisplayName;
	ref array<ref DME_TeleportDestination> Destinations;

	void DME_TeleportConfig()
	{
		Version = 5;
		RepMode = 1;
		#ifdef EXPANSIONMODHARDLINE
		EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST;
		#else
		EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_NONE;
		#endif
		CurrencyClassName = string.Empty;
		CurrencyDisplayName = "#STR_DME_TELEPORT_BALANCE";
		Destinations = new array<ref DME_TeleportDestination>;
	}

	// PsyCore_ConfigBase: the version field keeps its existing JSON name "Version"
	override int GetVersion()
	{
		return Version;
	}

	override void SetVersion(int version)
	{
		Version = version;
	}

	override int GetCurrentVersion()
	{
		return 5;
	}

	// Called by PsyCore_ConfigLoader when Version < 5 (steps unchanged, run in order)
	override void OnMigrate(int fromVersion)
	{
		if (Version < 2 && RepMode == 0)
			RepMode = 1;

		if (Version < 2)
		{
			if (Destinations)
			{
				for (int d = 0; d < Destinations.Count(); d++)
				{
					DME_TeleportDestination dest = Destinations[d];
					if (dest && dest.Picture <= 0)
						dest.Picture = 1;
				}
			}

			Version = 2;
		}

		if (Version < 3)
		{
			if (Destinations)
			{
				for (int markerIndex = 0; markerIndex < Destinations.Count(); markerIndex++)
				{
					DME_TeleportDestination markerDestination = Destinations[markerIndex];
					if (!markerDestination)
						continue;

					markerDestination.Marker = 1;
				}
			}

			Version = 3;
		}

		if (Version < 4)
		{
			#ifdef EXPANSIONMODHARDLINE
			if (RepMode == 0)
				EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP;
			else
				EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST;
			#else
			EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_NONE;
			#endif

			if (CurrencyDisplayName == string.Empty)
				CurrencyDisplayName = "#STR_DME_TELEPORT_BALANCE";

			Version = 4;
		}

		if (Version < 5)
		{
			if (Destinations)
			{
				for (int questIndex = 0; questIndex < Destinations.Count(); questIndex++)
				{
					DME_TeleportDestination questDest = Destinations[questIndex];
					if (!questDest)
						continue;

					questDest.Expansion_Quest_Enable = 0;
					questDest.Expansion_Quest_ID = -1;
				}
			}

			Version = 5;
		}

		DME_TeleportConfigLoader.MarkChanged();
	}

	// Called after every load
	override void OnValidate()
	{
		if (CurrencyDisplayName == string.Empty)
		{
			CurrencyDisplayName = "#STR_DME_TELEPORT_BALANCE";
			DME_TeleportConfigLoader.MarkChanged();
		}

		#ifndef EXPANSIONMODHARDLINE
		if (EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP || EconomyMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST)
		{
			EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_NONE;
			DME_TeleportConfigLoader.MarkChanged();
			PsyCore_Log.Get("DME_Teleport_Menu").Warn("Hardline not detected. EconomyMode reset to NONE. Configure CurrencyClassName and EconomyMode=3 for item-based payment.");
		}
		#endif
	}
};

class DME_TeleportConfigLoader
{
	// set by DME_TeleportConfig.OnMigrate/OnValidate: the file is only rewritten when something changed
	protected static bool s_DME_Changed;

	static void MarkChanged()
	{
		s_DME_Changed = true;
	}

	static DME_TeleportConfig Load()
	{
		string file = DME_Teleport_Constants.CONFIG_FILE;
		EnsureDirectories();

		bool existed = FileExist(file);
		s_DME_Changed = false;
		DME_TeleportConfig config;
		// writeBack=false: existing files are only rewritten after a migration/repair (as before)
		bool loaded = PsyCore_ConfigLoader<DME_TeleportConfig>.Load(file, config, "", "DME_Teleport_Menu", false);
		if (loaded && config.Destinations && config.Destinations.Count() > 0)
		{
			if (s_DME_Changed)
				Save(config);

			PsyCore_Log.Get("DME_Teleport_Menu").Info("Config loaded with " + config.Destinations.Count().ToString() + " destinations.");
			return config;
		}

		config = CreateDefaults();
		if (existed && !loaded)
		{
			// a broken file is never overwritten: the admin must be able to fix it
			PsyCore_Log.Get("DME_Teleport_Menu").Error("Config " + file + " could not be read, using default destinations until it is fixed.");
			return config;
		}

		Save(config);
		PsyCore_Log.Get("DME_Teleport_Menu").Info("Default config created and saved.");
		return config;
	}

	static void Save(DME_TeleportConfig config)
	{
		EnsureDirectories();
		PsyCore_ConfigLoader<DME_TeleportConfig>.Save(DME_Teleport_Constants.CONFIG_FILE, config, "DME_Teleport_Menu");
	}

	static void EnsureDirectories()
	{
		DME_Teleport_Overhaul.EnsureDirectories();
	}

	static DME_TeleportConfig CreateDefaults()
	{
		DME_TeleportConfig config = new DME_TeleportConfig();
		config.Version = 5;
		config.RepMode = 1;
		#ifdef EXPANSIONMODHARDLINE
		config.EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST;
		#else
		config.EconomyMode = DME_Teleport_Constants.ECONOMY_MODE_NONE;
		#endif
		config.CurrencyClassName = string.Empty;
		config.CurrencyDisplayName = "#STR_DME_TELEPORT_BALANCE";

		DME_TeleportDestination greenMtn = new DME_TeleportDestination();
		greenMtn.TeleportName = "Green Mountain";
		greenMtn.TeleportPos = { 3700.51, 0.0, 5981.27 };
		greenMtn.Cost = 1200;
		greenMtn.CooldownSec = 1700;
		greenMtn.Picture = 1;
		greenMtn.Marker = 1;

		DME_TeleportDestination krasno = new DME_TeleportDestination();
		krasno.TeleportName = "Krasno Airfield";
		krasno.TeleportPos = { 11880.40, 0.0, 12460.74 };
		krasno.Cost = 1800;
		krasno.CooldownSec = 1500;
		krasno.Picture = 2;
		krasno.Marker = 1;

		config.Destinations.Insert(greenMtn);
		config.Destinations.Insert(krasno);
		return config;
	}
};
