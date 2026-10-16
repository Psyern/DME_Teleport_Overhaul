class DME_TeleportDestRow
{
	string Name;
	int Cost;
	int CooldownMax;
	int RemainingSeconds;
	int Picture;
	float ClientTimestamp;

	Widget RootWidget;
	ImageWidget PictureWidget;
	TextWidget NameText;
	TextWidget CostText;
	TextWidget CooldownText;
	ButtonWidget TravelBtn;

	bool IsReady()
	{
		return GetCurrentRemaining() <= 0;
	}

	int GetCurrentRemaining()
	{
		if (RemainingSeconds <= 0)
			return 0;

		float elapsed = g_Game.GetTickTime() - ClientTimestamp;
		int remaining = RemainingSeconds - (int) elapsed;
		if (remaining < 0)
			remaining = 0;

		return remaining;
	}
};

class DME_TeleportMenu extends UIScriptedMenu
{
	private const string LAYOUT_PATH = "DME_Teleport_Overhaul/gui/layouts/dme_teleport.layout";
	private const string ROW_LAYOUT_PATH = "DME_Teleport_Overhaul/gui/layouts/dme_teleport_row.layout";

	private Widget m_RootFrame;
	private TextWidget m_TitleText;
	private TextWidget m_ReputationText;
	private TextWidget m_StatusText;
	private Widget m_DestinationList;
	private ButtonWidget m_CloseButton;
	private TextWidget m_HeaderCostText;

	private int m_PlayerReputation;
	private int m_RepMode;
	private string m_EconomyLabelText;
	private ref array<ref DME_TeleportDestRow> m_Rows;
	private float m_UpdateTimer;

	void DME_TeleportMenu()
	{
		m_Rows = new array<ref DME_TeleportDestRow>;
		m_UpdateTimer = 0;
		m_EconomyLabelText = "#STR_DME_TELEPORT_REPUTATION";

		// UIScriptedMenu is Managed; the menu instance is kept alive by MissionGameplay.m_DMETeleportMenu
		PsyCore_RPC.Register(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_STATE, this, DME_Teleport_RPC.SYNC_STATE, PsyCore_RpcFlags.CLIENT);
		PsyCore_RPC.Register(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_RESULT, this, DME_Teleport_RPC.TRAVEL_RESULT, PsyCore_RpcFlags.CLIENT);
	}

	void ~DME_TeleportMenu()
	{
		ClearRows();
	}

	private string TranslateText(string text)
	{
		if (text == string.Empty)
			return text;

		return Widget.TranslateString(text);
	}

	override Widget Init()
	{
		layoutRoot = g_Game.GetWorkspace().CreateWidgets(LAYOUT_PATH);
		if (layoutRoot)
		{
			m_RootFrame = layoutRoot;
			m_TitleText = TextWidget.Cast(layoutRoot.FindAnyWidget("TitleText"));
			m_ReputationText = TextWidget.Cast(layoutRoot.FindAnyWidget("ReputationText"));
			m_StatusText = TextWidget.Cast(layoutRoot.FindAnyWidget("StatusText"));
			m_DestinationList = layoutRoot.FindAnyWidget("DestinationContent");
			m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
			m_HeaderCostText = TextWidget.Cast(layoutRoot.FindAnyWidget("HeaderCost"));

			if (m_TitleText)
				m_TitleText.SetText(TranslateText("#STR_DME_TELEPORT_TITLE"));

			if (m_StatusText)
				m_StatusText.SetText(TranslateText("#STR_DME_TELEPORT_LOADING"));

			if (m_ReputationText)
				m_ReputationText.SetText(string.Format(TranslateText("#STR_DME_TELEPORT_REPUTATION_FORMAT"), GetEconomyLabelText(), "---"));
		}

		return layoutRoot;
	}

	override void OnShow()
	{
		super.OnShow();
		SetFocus(layoutRoot);
		g_Game.GetInput().ChangeGameFocus(1);
		g_Game.GetUIManager().ShowUICursor(true);

		// keep the menu key active while the menu has focus, so the same key closes it again
		UAInput toggleInput = GetUApi().GetInputByName(DME_Teleport_Constants.INPUT_MENU_TOGGLE);
		if (toggleInput)
		{
			toggleInput.ForceDisable(false);
			GetUApi().UpdateControls();
		}

		PsyCore_RPC.SendEmpty(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_REQUEST, true);

		if (m_StatusText)
			m_StatusText.SetText(TranslateText("#STR_DME_TELEPORT_REQUESTING_DATA"));
	}

	override void OnHide()
	{
		super.OnHide();
		g_Game.GetInput().ChangeGameFocus(-1);
		g_Game.GetUIManager().ShowUICursor(false);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_CloseButton)
		{
			Close();
			return true;
		}

		for (int i = 0; i < m_Rows.Count(); i++)
		{
			DME_TeleportDestRow row = m_Rows[i];
			if (w == row.TravelBtn)
			{
				RequestTravel(row.Name);
				return true;
			}
		}

		return false;
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		if (key == KeyCode.KC_ESCAPE)
		{
			Close();
			return true;
		}

		return super.OnKeyDown(w, x, y, key);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		m_UpdateTimer += timeslice;
		if (m_UpdateTimer >= 1.0)
		{
			m_UpdateTimer = 0;
			UpdateCooldownDisplays();
		}
	}

	void RPC_SyncState(ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param4<int, int, string, string> data;
		if (!ctx.Read(data) || !data)
			return;

		m_PlayerReputation = data.param1;
		m_RepMode = data.param2;
		string serialized = data.param3;
		m_EconomyLabelText = data.param4;

		if (m_ReputationText)
			m_ReputationText.SetText(string.Format(TranslateText("#STR_DME_TELEPORT_REPUTATION_FORMAT"), GetEconomyLabelText(), GetEconomyValueText()));

		if (m_HeaderCostText)
		{
			if (m_RepMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP)
				m_HeaderCostText.SetText(TranslateText("#STR_DME_TELEPORT_MIN_REP"));
			else
				m_HeaderCostText.SetText(TranslateText("#STR_DME_TELEPORT_COST"));
		}

		ClearRows();
		if (serialized == "")
		{
			if (m_StatusText)
				m_StatusText.SetText(TranslateText("#STR_DME_TELEPORT_NO_DEST_CONFIGURED"));
			return;
		}

		TStringArray entries = new TStringArray;
		serialized.Split(";", entries);
		float nowTick = g_Game.GetTickTime();

		for (int i = 0; i < entries.Count(); i++)
		{
			TStringArray parts = new TStringArray;
			entries[i].Split("|", parts);
			if (parts.Count() < 4)
				continue;

			DME_TeleportDestRow row = new DME_TeleportDestRow();
			row.Name = parts[0];
			row.Cost = parts[1].ToInt();
			row.CooldownMax = parts[2].ToInt();
			row.RemainingSeconds = parts[3].ToInt();
			if (parts.Count() >= 5)
				row.Picture = parts[4].ToInt();
			row.ClientTimestamp = nowTick;

			CreateRowWidget(row);
			m_Rows.Insert(row);
		}

		if (m_StatusText)
		{
			if (m_Rows.Count() > 0)
				m_StatusText.SetText("");
			else
				m_StatusText.SetText(TranslateText("#STR_DME_TELEPORT_NO_DEST_AVAILABLE"));
		}

		UpdateCooldownDisplays();
	}

	void RPC_TravelResult(ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param5<int, string, int, string, int> data;
		if (!ctx.Read(data) || !data)
			return;

		bool success = data.param1 == 1;
		string message = data.param2;
		m_PlayerReputation = data.param3;
		string destName = data.param4;

		if (m_ReputationText)
			m_ReputationText.SetText(string.Format(TranslateText("#STR_DME_TELEPORT_REPUTATION_FORMAT"), GetEconomyLabelText(), GetEconomyValueText()));

		if (m_StatusText)
			m_StatusText.SetText(TranslateText(message));

		if (success)
		{
			float nowTick = g_Game.GetTickTime();
			for (int i = 0; i < m_Rows.Count(); i++)
			{
				DME_TeleportDestRow row = m_Rows[i];
				if (row.Name == destName)
				{
					row.RemainingSeconds = row.CooldownMax;
					row.ClientTimestamp = nowTick;
					break;
				}
			}

			UpdateCooldownDisplays();
			Close();
		}
		else
		{
			UpdateCooldownDisplays();
		}
	}

	private void CreateRowWidget(DME_TeleportDestRow row)
	{
		if (!m_DestinationList)
			return;

		Widget rowWidget = g_Game.GetWorkspace().CreateWidgets(ROW_LAYOUT_PATH, m_DestinationList);
		if (!rowWidget)
			return;

		row.RootWidget = rowWidget;
		row.PictureWidget = ImageWidget.Cast(rowWidget.FindAnyWidget("ImageWidget0"));
		row.NameText = TextWidget.Cast(rowWidget.FindAnyWidget("DestNameText"));
		row.CostText = TextWidget.Cast(rowWidget.FindAnyWidget("DestCostText"));
		row.CooldownText = TextWidget.Cast(rowWidget.FindAnyWidget("DestCooldownText"));
		row.TravelBtn = ButtonWidget.Cast(rowWidget.FindAnyWidget("TravelButton"));

		if (row.PictureWidget)
		{
			string tex = DME_Teleport_Constants.GetPictureTexture(row.Picture);
			if (tex != string.Empty)
				row.PictureWidget.LoadImageFile(0, tex);
		}

		if (row.NameText)
			row.NameText.SetText(row.Name);

		if (row.CostText)
			row.CostText.SetText(row.Cost.ToString());
	}

	private void ClearRows()
	{
		for (int i = 0; i < m_Rows.Count(); i++)
		{
			if (m_Rows[i].RootWidget)
				m_Rows[i].RootWidget.Unlink();
		}
		m_Rows.Clear();
	}

	private void UpdateCooldownDisplays()
	{
		for (int i = 0; i < m_Rows.Count(); i++)
		{
			DME_TeleportDestRow row = m_Rows[i];
			int remaining = row.GetCurrentRemaining();
			if (remaining <= 0)
			{
				if (row.CooldownText)
				{
					row.CooldownText.SetText(TranslateText("#STR_DME_TELEPORT_READY"));
					row.CooldownText.SetColor(ARGB(255, 128, 255, 128));
				}

				if (row.TravelBtn)
					row.TravelBtn.Enable(CanAffordRow(row));
			}
			else
			{
				int minutes = remaining / 60;
				int seconds = remaining % 60;
				string timeStr;
				if (minutes > 0)
					timeStr = minutes.ToString() + Widget.TranslateString("#STR_time_unit_abbrev_minute_0") + " " + seconds.ToString() + Widget.TranslateString("#STR_time_unit_abbrev_second_0");
				else
					timeStr = seconds.ToString() + Widget.TranslateString("#STR_time_unit_abbrev_second_0");

				if (row.CooldownText)
				{
					row.CooldownText.SetText(timeStr);
					row.CooldownText.SetColor(ARGB(255, 220, 80, 80));
				}

				if (row.TravelBtn)
					row.TravelBtn.Enable(false);
			}
		}
	}

	private void RequestTravel(string destinationName)
	{
		if (m_StatusText)
			m_StatusText.SetText(string.Format(TranslateText("#STR_DME_TELEPORT_TRAVELING_TO"), destinationName));

		PsyCore_RPC.SendParam(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_REQUEST, new Param1<string>(destinationName), true);
	}

	override int GetID()
	{
		return DME_Teleport_Constants.DME_TELEPORT_MENU;
	}

	override bool UseKeyboard()
	{
		return false;
	}

	override bool UseMouse()
	{
		return true;
	}

	private bool CanAffordRow(DME_TeleportDestRow row)
	{
		if (!row)
			return false;

		if (m_RepMode == DME_Teleport_Constants.ECONOMY_MODE_NONE)
			return true;

		return m_PlayerReputation >= row.Cost;
	}

	private string GetEconomyLabelText()
	{
		if (m_EconomyLabelText != string.Empty)
			return TranslateText(m_EconomyLabelText);

		if (m_RepMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_MIN_REP || m_RepMode == DME_Teleport_Constants.ECONOMY_MODE_HARDLINE_COST)
			return TranslateText("#STR_DME_TELEPORT_REPUTATION");

		return TranslateText("#STR_DME_TELEPORT_BALANCE");
	}

	private string GetEconomyValueText()
	{
		if (m_RepMode == DME_Teleport_Constants.ECONOMY_MODE_NONE)
			return TranslateText("#STR_DME_TELEPORT_FREE");

		return m_PlayerReputation.ToString();
	}
};
