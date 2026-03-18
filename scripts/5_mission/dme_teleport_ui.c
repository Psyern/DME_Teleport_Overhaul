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

		float elapsed = GetGame().GetTickTime() - ClientTimestamp;
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
	private ref array<ref DME_TeleportDestRow> m_Rows;
	private float m_UpdateTimer;

	void DME_TeleportMenu()
	{
		m_Rows = new array<ref DME_TeleportDestRow>;
		m_UpdateTimer = 0;

		GetRPCManager().AddRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_STATE, this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_RESULT, this, SingleplayerExecutionType.Client);
	}

	void ~DME_TeleportMenu()
	{
		ClearRows();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets(LAYOUT_PATH);
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
				m_TitleText.SetText("DME Teleport");

			if (m_StatusText)
				m_StatusText.SetText("Loading...");

			if (m_ReputationText)
				m_ReputationText.SetText("Reputation: ---");
		}

		return layoutRoot;
	}

	override void OnShow()
	{
		super.OnShow();
		SetFocus(layoutRoot);
		GetGame().GetInput().ChangeGameFocus(1);
		GetGame().GetUIManager().ShowUICursor(true);
		GetRPCManager().SendRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.SYNC_REQUEST, null, true);

		if (m_StatusText)
			m_StatusText.SetText("Requesting data...");
	}

	override void OnHide()
	{
		super.OnHide();
		GetGame().GetInput().ChangeGameFocus(-1);
		GetGame().GetUIManager().ShowUICursor(false);
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

	void RPC_SyncState(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
			return;

		Param3<int, int, string> data = new Param3<int, int, string>(0, 1, "");
		if (!ctx.Read(data))
			return;

		m_PlayerReputation = data.param1;
		m_RepMode = data.param2;
		string serialized = data.param3;

		if (m_ReputationText)
			m_ReputationText.SetText("Reputation: " + m_PlayerReputation.ToString());

		if (m_HeaderCostText)
		{
			if (m_RepMode == 0)
				m_HeaderCostText.SetText("Min Rep");
			else
				m_HeaderCostText.SetText("Cost");
		}

		ClearRows();
		if (serialized == "")
		{
			if (m_StatusText)
				m_StatusText.SetText("No destinations configured.");
			return;
		}

		TStringArray entries = new TStringArray;
		serialized.Split(";", entries);
		float nowTick = GetGame().GetTickTime();

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
				m_StatusText.SetText("No destinations available.");
		}

		UpdateCooldownDisplays();
	}

	void RPC_TravelResult(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
			return;

		Param5<int, string, int, string, int> data = new Param5<int, string, int, string, int>(0, "", 0, "", 0);
		if (!ctx.Read(data))
			return;

		bool success = data.param1 == 1;
		string message = data.param2;
		m_PlayerReputation = data.param3;
		string destName = data.param4;

		if (m_ReputationText)
			m_ReputationText.SetText("Reputation: " + m_PlayerReputation.ToString());

		if (m_StatusText)
			m_StatusText.SetText(message);

		if (success)
		{
			float nowTick = GetGame().GetTickTime();
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
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Close, 1500, false);
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

		Widget rowWidget = GetGame().GetWorkspace().CreateWidgets(ROW_LAYOUT_PATH, m_DestinationList);
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
					row.CooldownText.SetText("Ready");
					row.CooldownText.SetColor(ARGB(255, 128, 255, 128));
				}

				if (row.TravelBtn)
					row.TravelBtn.Enable(m_PlayerReputation >= row.Cost);
			}
			else
			{
				int minutes = remaining / 60;
				int seconds = remaining % 60;
				string timeStr;
				if (minutes > 0)
					timeStr = minutes.ToString() + "m " + seconds.ToString() + "s";
				else
					timeStr = seconds.ToString() + "s";

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
			m_StatusText.SetText("Traveling to " + destinationName + "...");

		GetRPCManager().SendRPC(DME_Teleport_RPC.MOD_NAME, DME_Teleport_RPC.TRAVEL_REQUEST, new Param1<string>(destinationName), true);
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
};
