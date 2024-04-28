class NewStoryGroupPlayerModuleKills: NewStoryGroupPlayerModuleBase
{
	protected ref ScriptInvoker m_OnEntityKilled;
	
	override bool OnRPC(int type, ParamsReadContext ctx)
	{
		if ( type != ENewStoryGroupRPCPlayerManager.MODULE_KILLS ) {
			return false;
		}

		#ifndef SERVER
		Param3<EntityAI, EntityAI, bool> param;
		if ( ctx.Read( param ) ) {
			OnEntityKilled( param.param1, param.param2, param.param3 );
		}
		else {
			ErrorEx( "[NSG PlayerModuleKills] OnRPC :: Can't read 'param'!", ErrorExSeverity.WARNING );
		}
		#endif

		return true;
	}

	protected string GetEntityName(EntityAI entity)
	{
		return entity.ConfigGetString( "displayName" );
	}

	protected string GetNameNPC(EntityAI npc)
	{
		return Widget.TranslateString( npc.ConfigGetString( "displayName" ) ) + " (AI)";
	}
	
	// Когда совершили суицид
	void OnSuicide(EntityAI source);
	// Умер хрен знает от чего
	//  вроде как работает только если с высоты
	void OnUnknownDeath();
	// Когда умерли от объекта
	void OnDeathFromEntity(EntityAI source, bool isHeadshot);
	// Когда умерли от NPC
	void OnDeathFromNPC(EntityAI killer, EntityAI source, bool isHeadshot);

	// Когда убили кого-то
	void OnEntityKilled(EntityAI victim, EntityAI source, bool isHeadshot)
	{
		if ( m_OnEntityKilled ) {
			m_OnEntityKilled.Invoke( victim, source, isHeadshot );
		}
		
		#ifdef SERVER
		if ( NewStoryGroupCoreSettings.REQUEST_SYNC_PLAYER_MODULE_KILLS ) {
			Param sendParam = new Param3<EntityAI, EntityAI, bool>( victim, source, isHeadshot);
			GetPlayerIdentity().GetNewStoryGroupPlayerManagerServer().RPCSingleParam( ENewStoryGroupRPCPlayerManager.MODULE_KILLS, sendParam );
		}
		#endif
	}

	ScriptInvoker Event_OnEntityKilled()
	{
		if ( !m_OnEntityKilled ) {
			m_OnEntityKilled = new ScriptInvoker();
		}

		return m_OnEntityKilled;
	}

	static EntityAI GetMurderer(Object killer)
	{
		EntityAI killerEnt = EntityAI.Cast( killer );

		if ( !killerEnt ) {
			return null;
		}

		if ( killerEnt.IsWeapon() || killerEnt.IsMeleeWeapon() ) {
			return killerEnt.GetHierarchyParent();
		}

		if ( killerEnt.IsTransport() ) {
			return Transport.Cast( killerEnt ).CrewMember(0);
		}

		return killerEnt;
	}
}
