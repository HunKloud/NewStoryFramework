class NewStoryGroupPlayerManagersServer: INewStoryGroupPlayerManagersServer
{
	// (PlayerIdentity identity)
	private ref ScriptInvoker m_OnStartConnection;
	// (PlayerIdentity identity)
	private ref ScriptInvoker m_OnConnect;
	// (string uid)
	private ref ScriptInvoker m_OnDisconnected;
	// (int count)
	private ref ScriptInvoker m_OnUpdatePlayerCount;

	private const int CONNECTION_LIST_CLEANING_TIME = 60;
	private const int MAX_TIME_FOR_CONNECTED		= 300;

	private ref Timer m_ConnectionListCleaningTimer;

	private ref map<string, ref NewStoryGroupPlayerManagerServer> m_List;
	private ref map<string, ref NewStoryGroupPlayerManagerServer> m_ConnectionList;

	void NewStoryGroupPlayerManagersServer()
	{
		m_List		   = new map<string, ref NewStoryGroupPlayerManagerServer>();
		m_ConnectionList = new map<string, ref NewStoryGroupPlayerManagerServer>();
	}

	override void OnStartConnection(PlayerIdentity identity)
	{
		string uid = identity.GetId();

		if ( m_List.Contains( uid ) ) {
			return;
		}

		NewStoryGroupPlayerManagerServer manager;

		if ( m_ConnectionList.Find( uid, manager ) ) {
			manager.OnStartConnection( identity );
		} else {
			manager = new NewStoryGroupPlayerManagerServer();
			manager.LoadModules();
			manager.OnStartConnection( identity );

			m_ConnectionList.Insert( uid, manager );

			StartConnectionListCleaning();
		}

		if ( m_OnStartConnection ) {
			m_OnStartConnection.Invoke( identity );
		}
	}

	override void OnCreateCharacter(PlayerIdentity identity, Man character)
	{
		string uid = identity.GetId();

		NewStoryGroupPlayerManagerServer manager;

		if ( m_ConnectionList.Find( uid, manager ) ) {
			manager.OnCreateCharacter( identity, character );
		}
	}

	override void OnConnect(PlayerIdentity identity)
	{
		string uid = identity.GetId();
		NewStoryGroupPlayerManagerServer manager;

		if ( m_ConnectionList.Find( uid, manager ) ) {
			m_List.Insert( uid, manager );
			m_ConnectionList.Remove( uid );

			manager.OnConnect( identity );

			if ( m_OnConnect ) {
				m_OnConnect.Invoke( identity );
			}

			if ( m_OnUpdatePlayerCount ) {
				m_OnUpdatePlayerCount.Invoke( GetPlayerCount() );
			}

			if ( m_ConnectionList.Count() == 0 ) {
				StopConnectionListCleaning();
			}
		}
		else if ( m_List.Find( uid, manager ) ) {
			manager.OnRespawn();
		}

		// Если каким либо образом менеджера нет, то проходим весь путь в турбо режиме )
		if ( !manager ) {
			manager = new NewStoryGroupPlayerManagerServer();
			manager.LoadModules();
			manager.OnStartConnection( identity );
			manager.OnConnect( identity );

			m_List.Insert( uid, manager );

			if ( m_OnStartConnection ) {
				m_OnStartConnection.Invoke( identity );
			}

			if ( m_OnConnect ) {
				m_OnConnect.Invoke( identity );
			}

			if ( m_OnUpdatePlayerCount ) {
				m_OnUpdatePlayerCount.Invoke( GetPlayerCount() );
			}
		}
	}

	override void Disconnected(string uid)
	{
		NewStoryGroupPlayerManagerServer manager;

		if ( m_List.Find( uid, manager ) ) {
			manager.OnDisconnected( uid );
			m_List.Remove( uid );

			if ( m_OnUpdatePlayerCount ) {
				m_OnUpdatePlayerCount.Invoke( GetPlayerCount() );
			}

			if ( m_OnDisconnected ) {
				m_OnDisconnected.Invoke( uid );
			}
		}
	}

	private void StartConnectionListCleaning()
	{
		if ( !m_ConnectionListCleaningTimer ) {
			m_ConnectionListCleaningTimer = new Timer( CALL_CATEGORY_SYSTEM );
		}

		if ( !m_ConnectionListCleaningTimer.IsRunning() ) {
			m_ConnectionListCleaningTimer.Run( CONNECTION_LIST_CLEANING_TIME, this, "ConnectionListCleaning", null, true );
		}
	}

	private void StopConnectionListCleaning()
	{
		if ( !m_ConnectionListCleaningTimer ) {
			return;
		}

		if ( m_ConnectionListCleaningTimer.IsRunning() ) {
			m_ConnectionListCleaningTimer.Stop();
		}

		m_ConnectionListCleaningTimer = null;
	}

	void ConnectionListCleaning()
	{
		NewStoryGroupPlayerManagerServer manager;
		float currentTickTime = GetGame().GetTickTime();

		for ( int i = m_ConnectionList.Count() - 1; i >= 0; i-- ) {
			manager = m_ConnectionList.GetElement(i);

			if ( currentTickTime - manager.GetTickTimeFromStartConnection() >= MAX_TIME_FOR_CONNECTED ) {
				m_ConnectionList.RemoveElement(i);
			}
		}

		if ( m_ConnectionList.Count() == 0 ) {
			StopConnectionListCleaning();
		}
	}

	override int GetPlayerCount()
	{
		return m_List.Count();
	}

	override INewStoryGroupPlayerManagerBase GetPlayerManager(string uid)
	{
		return m_List.Get( uid );
	}

	override ScriptInvoker Event_OnUpdatePlayerCount()
	{
		if ( !m_OnUpdatePlayerCount ) {
			m_OnUpdatePlayerCount = new ScriptInvoker();
		}
		
		return m_OnUpdatePlayerCount;
	}

	override ScriptInvoker Event_OnStartConnection()
	{
		if ( !m_OnStartConnection ) {
			m_OnStartConnection = new ScriptInvoker();
		}
		
		return m_OnStartConnection;
	}

	override ScriptInvoker Event_OnConnect()
	{
		if ( !m_OnConnect ) {
			m_OnConnect = new ScriptInvoker();
		}
		
		return m_OnConnect;
	}

	override ScriptInvoker Event_OnDisconnected()
	{
		if ( !m_OnDisconnected ) {
			m_OnDisconnected = new ScriptInvoker();
		}
		
		return m_OnDisconnected;
	}
}
