class NewStoryGroupCore: INewStoryGroupCore
{
	static ref NewStoryGroupCore m_Instance;

	private ref NewStoryGroupProfilesOptions					m_ProfilesOptions;

	private ref NewStoryGroupRPC											m_RPC;
	private ref NewStoryGroupAPI											m_API;
	private ref NewStoryGroupDiscordLog								m_DiscordLog;
	private ref INewStoryGroupPlayerManagersServer		m_PlayerManagers;
	private ref INewStoryGroupPlayerManagerBase				m_PlayerManagerClient;
	private ref NewStoryGroupBasePlayerInfo						m_BasePlayerInfo;
	private ref NewStoryGroupEventContainerKeyHandler	m_EventContainerKeyHandler;
	private ref NewStoryGroupCategoryList							m_CategoryList;

	private ref NewStoryGroupClientPlayerList					m_ClientPlayerList;

	private ref NewStoryGroupGameTimeEvents						m_GameTimeEvents;

	private string m_ServerName;
	private ref array<ParamsReadContext> m_AwaitReadPlayerManagerClientCtx;

	private string m_StorageFolder;

	override void Init()
	{
		m_ProfilesOptions	= new NewStoryGroupProfilesOptions();

		m_RPC				= new NewStoryGroupRPC();
		m_API				= new NewStoryGroupAPI();
		m_BasePlayerInfo	= new NewStoryGroupBasePlayerInfo();

		if ( NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			m_EventContainerKeyHandler = new NewStoryGroupEventContainerKeyHandler();
		}

		if ( NewStoryGroupCoreSettings.REQUEST_SYNC_CATEGORY_LIST ) {
			m_CategoryList = new NewStoryGroupCategoryList();
		}
	}

	override void OnCreateMission()
	{
		if ( NewStoryGroupCoreSettings.REQUEST_GAME_TIME_EVENTS ) {
			m_GameTimeEvents = new NewStoryGroupGameTimeEvents();
		}
	}

	override void OnCreateMissionGameplay()
	{
		if ( NewStoryGroupCoreSettings.REQUEST_CLIENT_PLAYER_LIST ) {
			m_ClientPlayerList = new NewStoryGroupClientPlayerList();
		}
	}

	override void OnPreCreateMissionServer()
	{
		NewStoryGroupSystemFunction.CreateFolder( NEW_STORY_GROUP_FOLDER );
		NewStoryGroupSystemFunction.CreateFolder( NEW_STORY_GROUP_CONFIG_FOLDER );
		NewStoryGroupSystemFunction.CreateFolder( NEW_STORY_GROUP_DATA_FOLDER );
	}

	override void OnCreateMissionServer()
	{
		if ( NewStoryGroupCoreSettings.REQUEST_STORAGE_FOLDER ) {
			int instanceId = GetGame().ServerConfigGetInt( "instanceId" );
			m_StorageFolder = "$mission:storage_" + instanceId + "\\";

			// Да это полная шляпа, но другого способа увы нет
			//  иначе придется 2 раза запускать сервер ( ´･･)ﾉ(._.`)
			NewStoryGroupSystemFunction.CreateFolder( m_StorageFolder );

			m_StorageFolder += "NewStoryGroup\\";
			NewStoryGroupSystemFunction.CreateFolder( m_StorageFolder );
		}

		if ( NewStoryGroupCoreSettings.REQUEST_DISCORD_LOG ) {
			m_DiscordLog = new NewStoryGroupDiscordLog();
		}

		if ( NewStoryGroupCoreSettings.REQUEST_SYNC_CATEGORY_LIST ) {
			m_CategoryList.OnCreateMissionServer();
		}
	}

	override void OnInitMissionServer()
	{
		if ( NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			m_EventContainerKeyHandler.OnInitMissionServer();
		}

		if ( NewStoryGroupCoreSettings.REQUEST_SYNC_BASE_PLAYER_INFO ) {
			m_BasePlayerInfo.OnInitMissionServer();
		}
	}

	override void OnDestroyMission()
	{
		m_GameTimeEvents = null;

		if ( NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			m_EventContainerKeyHandler.OnDestroyMission();
		}

		if ( NewStoryGroupCoreSettings.REQUEST_SYNC_CATEGORY_LIST ) {
			m_CategoryList.OnDestroyMission();
		}
	}

	override void OnDestroyMissionGameplay()
	{
		m_PlayerManagerClient = null;
		m_ClientPlayerList	 = null;
	}

	void LoadServerName()
	{
		m_ServerName = SetServerName();
	}

	override string GetServerName()
	{
		return m_ServerName;
	}

	override string GetStorageFolder()
	{
		return m_StorageFolder;
	}

	private string SetServerName()
	{
		string path = "$CurrentDir:serverDZ.cfg";
		array<string> buffer = new array<string>();
		int i = 0;
		
		// ----------------------------
		//   Ищем где находится конфиг
		// дибильное решение, но пофигу
		// ----------------------------
		{
			string cfgPath;
			string bufferStr = "";
			GetGame().CommandlineGetParam( "config", cfgPath );

			if ( cfgPath != "" )
			{
				if ( cfgPath.Substring(1, 2) == ":\\" )
				{
					cfgPath.Split( "\\", buffer );

					if ( buffer && buffer.Count() > 0 )
					{
						for ( i = buffer.Count() - 1; i >= 0; i-- )
						{
							if ( bufferStr == "" )
								bufferStr = buffer[i];
							else
								bufferStr = buffer[i] + "\\" + bufferStr;
							
							if ( FileExist( "$CurrentDir:" + bufferStr ) )
							{
								path = "$CurrentDir:" + bufferStr;
								break;
							}
						}
					}
				}
				else
				{
					path = "$CurrentDir:" + cfgPath;
				}
			}

			if ( !FileExist(path) )
				path = "$CurrentDir:serverDZ.cfg";
		}

		// --------------
		// Считываем файл
		// --------------
		FileHandle fileHandle = OpenFile( path, FileMode.READ );

		if ( fileHandle == 0 ) return "";

		string line;
		
		while ( FGets( fileHandle, line ) > 0 )
		{
			int indx = line.IndexOf( "hostname" );

			if ( indx == -1 ) continue;

			int charCount = line.Length();
			bool start = false;
			string serverName = "";

			for ( i = indx + 8; i < charCount; i++ )
			{
				string sign = line[i];

				if ( !start && sign == "\"" )
				{
					start = true;
				}
				else if ( start )
				{
					if ( sign == "\"" )
					{
						CloseFile(fileHandle);
						return serverName;
					}

					serverName += sign;
				}
			}
		}

		CloseFile(fileHandle);

		return "";
	}

	override void SendChatMsg(PlayerIdentity recipient, string name, string message, string colorName = "", int channel = 0)
	{
		Param param = new Param4<string, string, string, int>( name, message, colorName, channel );
		m_RPC.SendSingleParam( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SEND_CHAT_MSG, param, recipient );
	}

	override NewStoryGroupProfilesOptions GetProfilesOptions()
	{
		return m_ProfilesOptions;
	}

	override INewStoryGroupRPC RPC()
	{
		return m_RPC;
	}

	override NewStoryGroupAPI GetAPI()
	{
		return m_API;
	}

	override NewStoryGroupDiscordLog GetDiscordLog()
	{
		return m_DiscordLog;
	}

	override INewStoryGroupPlayerManagersServer GetPlayerManagers()
	{
		return m_PlayerManagers;
	}

	void SetPlayerManagers(INewStoryGroupPlayerManagersServer handler)
	{
		m_PlayerManagers = handler;
	}

	override INewStoryGroupPlayerManagerBase GetPlayerManagerClient()
	{
		return m_PlayerManagerClient;
	}

	void SetPlayerManagerClient(INewStoryGroupPlayerManagerBase manager)
	{
		m_PlayerManagerClient = manager;

		if ( !m_AwaitReadPlayerManagerClientCtx ) {
			return;
		}

		for ( int i = 0; i < m_AwaitReadPlayerManagerClientCtx.Count(); i++ ) {
			m_PlayerManagerClient.OnRPC( m_AwaitReadPlayerManagerClientCtx[i] );
		}

		m_AwaitReadPlayerManagerClientCtx = null;
	}

	override NewStoryGroupBasePlayerInfo GetBasePlayerInfo()
	{
		return m_BasePlayerInfo;
	}

	override NewStoryGroupClientPlayerList GetClientPlayerList()
	{
		return m_ClientPlayerList;
	}

	override NewStoryGroupGameTimeEvents GetGameTimeEvents()
	{
		return m_GameTimeEvents;
	}

	override NewStoryGroupEventContainerKeyHandler GetEventContainerKeyHandler()
	{
		return m_EventContainerKeyHandler;
	}

	override NewStoryGroupCategoryList GetCategoryList()
	{
		return m_CategoryList;
	}

	bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		#ifdef SERVER
		if ( sender == null && ( rpc_type == ENewStoryGroupRPC.RPC_MANAGER || rpc_type == ENewStoryGroupRPC.PLAYER_MANAGER ) ) {
						ErrorEx( "Invalid sender" );
						return false;
				}
		#endif

		if ( rpc_type == ENewStoryGroupRPC.RPC_MANAGER ) {
			m_RPC.OnRPC( sender, target, ctx );
			return true;
		}

		if ( rpc_type == ENewStoryGroupRPC.PLAYER_MANAGER ) {
			#ifndef SERVER
			if ( m_PlayerManagerClient ) {
				m_PlayerManagerClient.OnRPC(ctx);
			}
			else {
				if ( !m_AwaitReadPlayerManagerClientCtx ) {
					m_AwaitReadPlayerManagerClientCtx = new array<ParamsReadContext>();
				}

				m_AwaitReadPlayerManagerClientCtx.Insert( ctx );
			}
			
			#else
			sender.GetNewStoryGroupPlayerManagerServer().OnRPC(ctx);
			#endif

			return true;
		}

		return false;
	}
}
