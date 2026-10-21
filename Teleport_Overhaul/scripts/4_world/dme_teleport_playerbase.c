modded class PlayerBase
{
	private bool m_DME_OverhaulTeleportTransitionActive;
	private bool m_DME_OverhaulTeleportTransitionRemoteSynch;
	private int m_DME_OverhaulTeleportTransitionCounter;
	private bool m_DME_OverhaulTeleportTransitionPreviousAllowDamage;
	private bool m_DME_OverhaulTeleportTransitionPreviousSolid;

	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableBool("m_DME_OverhaulTeleportTransitionRemoteSynch");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_DME_OverhaulTeleportTransitionActive == m_DME_OverhaulTeleportTransitionRemoteSynch)
			return;

		m_DME_OverhaulTeleportTransitionActive = m_DME_OverhaulTeleportTransitionRemoteSynch;
		SetInvisible(m_DME_OverhaulTeleportTransitionActive);
	}

	override bool CanBeTargetedByAI(EntityAI ai)
	{
		if (!super.CanBeTargetedByAI(ai))
			return false;

		if (m_DME_OverhaulTeleportTransitionActive)
			return false;

		return true;
	}

	void DME_OverhaulBeginTeleportTransition()
	{
		if (!g_Game || !g_Game.IsServer())
			return;

		if (m_DME_OverhaulTeleportTransitionCounter <= 0)
		{
			m_DME_OverhaulTeleportTransitionPreviousAllowDamage = GetAllowDamage();
			m_DME_OverhaulTeleportTransitionPreviousSolid = PhysicsIsSolid();
			SetAllowDamage(false);
			PhysicsSetSolid(false);
		}

		m_DME_OverhaulTeleportTransitionCounter++;
		DME_OverhaulUpdateTeleportTransitionState();
	}

	void DME_OverhaulEndTeleportTransition()
	{
		if (!g_Game || !g_Game.IsServer())
			return;

		if (m_DME_OverhaulTeleportTransitionCounter > 0)
			m_DME_OverhaulTeleportTransitionCounter--;

		if (m_DME_OverhaulTeleportTransitionCounter <= 0)
		{
			SetAllowDamage(m_DME_OverhaulTeleportTransitionPreviousAllowDamage);
			PhysicsSetSolid(m_DME_OverhaulTeleportTransitionPreviousSolid);
		}

		DME_OverhaulUpdateTeleportTransitionState();
	}

	protected void DME_OverhaulUpdateTeleportTransitionState()
	{
		bool isActive = m_DME_OverhaulTeleportTransitionCounter > 0;
		if (m_DME_OverhaulTeleportTransitionActive == isActive && m_DME_OverhaulTeleportTransitionRemoteSynch == isActive)
			return;

		m_DME_OverhaulTeleportTransitionActive = isActive;
		m_DME_OverhaulTeleportTransitionRemoteSynch = isActive;
		SetInvisible(isActive);
		SetSynchDirty();
	}

	#ifndef SERVER
	override void EOnPostFrame(IEntity other, int extra)
	{
		super.EOnPostFrame(other, extra);

		if (m_DME_OverhaulTeleportTransitionActive)
			SetInvisible(true);
	}
	#endif
}