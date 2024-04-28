modded class BuildingBase : Building
{
	private bool	m_NewStoryGroupRequestRespawn;
	private int		m_NewStoryGroupSpawnFlags;

	void SetNewStoryGroupRequestRespawn(bool value, int flags = 0)
	{
		m_NewStoryGroupRequestRespawn = value;
		m_NewStoryGroupSpawnFlags = flags;
	}

	bool IsNewStoryGroupRequestRespawn()
	{
		return m_NewStoryGroupRequestRespawn;
	}

	int GetNewStoryGroupSpawnFlags()
	{
		return m_NewStoryGroupSpawnFlags;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if ( !m_NewStoryGroupRequestRespawn ) {
			return;
		}

		NewStoryGroupPluginMappingHandler plugin;
		
		if ( Class.CastTo( plugin, GetPlugin( NewStoryGroupPluginMappingHandler ) ) ) {
			plugin.OnDeleteBuilding( this );
		}
	}
}
