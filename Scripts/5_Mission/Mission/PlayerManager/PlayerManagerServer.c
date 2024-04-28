class NewStoryGroupPlayerManagerServer: NewStoryGroupPlayerManagerBase
{
	private PlayerIdentity m_PlayerIdentity;

	private float m_TickTimeFromStartConnection = -2;

	void OnStartConnection(PlayerIdentity identity)
	{
		m_TickTimeFromStartConnection = GetGame().GetTickTime();

		m_PlayerIdentity = identity;
		m_PlayerIdentity.SetNewStoryGroupPlayerManagerServer( this );

		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			m_Modules.GetElement(i).OnStartConnection( identity );
		}

		AfterStartConnection(identity);
	}

	void OnCreateCharacter(PlayerIdentity identity, Man character)
	{
		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			m_Modules.GetElement(i).OnCreateCharacter( identity, character );
		}

		AfterCreateCharacter(identity, character);
	}

	void OnConnect(PlayerIdentity identity)
	{
		identity.SetNewStoryGroupPlayerManagerServer( this );

		Man player = identity.GetPlayer();

		// Костыль т.к. 'identity' и 'player.GetIdentity()' нихуя не одно и то же
		if ( player ) {
			player.GetIdentity().SetNewStoryGroupPlayerManagerServer( this );
		}

		m_TickTimeFromStartConnection = -1;

		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			NewStoryGroupPlayerModuleBase module = m_Modules.GetElement(i);
			module.SetPlayerIdentity( m_PlayerIdentity );
			module.OnConnect();
		}

		AfterConnect( m_PlayerIdentity );
	}

	void OnRespawn()
	{
		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			m_Modules.GetElement(i).OnRespawn();
		}

		AfterRespawn( m_PlayerIdentity );
	}

	void OnDisconnected(string uid)
	{
		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			m_Modules.GetElement(i).OnDisconnected(uid);
		}

		AfterDisconnected( uid );
	}

	float GetTickTimeFromStartConnection()
	{
		return m_TickTimeFromStartConnection;
	}

	override void RPC(int type, array<ref Param> params)
	{
		if ( !m_PlayerIdentity ) {
			ErrorEx( "[NSG PlayerManager] RPC :: Unknown PlayerIdentity!", ErrorExSeverity.WARNING );
			return;
		}

		array<ref Param> sendParams = params;
		Param1<int> paramType = new Param1<int>(type);

		if ( !sendParams ) {
			sendParams = new array<ref Param>();
			sendParams.Insert( paramType );
		}
		else {
			sendParams.InsertAt( paramType, 0 );
		}

		GetGame().RPC( null, ENewStoryGroupRPC.PLAYER_MANAGER, sendParams, true, m_PlayerIdentity );
	}

	override void RPCSingleParam(int type, Param param = null)
	{
		if ( !m_PlayerIdentity ) {
			ErrorEx( "[NSG PlayerManager] RPCSingleParam :: Unknown PlayerIdentity!", ErrorExSeverity.WARNING );
			return;
		}

		array<ref Param> sendParams = new array<ref Param>();
		sendParams.Insert( new Param1<int>(type) );

		if ( param ) {
			sendParams.Insert( param );
		}

		GetGame().RPC( null, ENewStoryGroupRPC.PLAYER_MANAGER, sendParams, true, m_PlayerIdentity );
	}

	override PlayerIdentity GetPlayerIdentity()
	{
		return m_PlayerIdentity;
	}
}
