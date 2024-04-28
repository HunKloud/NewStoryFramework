class NewStoryGroupPlayerModuleBase: INewStoryGroupPlayerModuleBase
{
	#ifdef SERVER
	private bool _SendErrorPlayerIdentity;
	#endif

	protected PlayerIdentity _PlayerIdentity;

	void SetPlayerIdentity(PlayerIdentity identity)
	{
		_PlayerIdentity = identity;
	}

	override PlayerIdentity GetPlayerIdentity()
	{
		#ifdef SERVER
		return _PlayerIdentity;
		#else
		return GetGame().GetPlayer().GetIdentity();
		#endif
	}

	override Man GetPlayer()
	{
		#ifdef SERVER
		if ( _PlayerIdentity ) {
			return _PlayerIdentity.GetPlayer();
		}

		// Для того что бы не спамило в логи
		if ( !_SendErrorPlayerIdentity ) {
			_SendErrorPlayerIdentity = true;
			ErrorEx( "[NSG PlayerModule] RPCSingleParam :: Unknown PlayerIdentity!", ErrorExSeverity.WARNING );
		}

		return null;
		#else
		return GetGame().GetPlayer();
		#endif
	}
}
