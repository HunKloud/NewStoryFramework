class INewStoryGroupCore
{
	// Вызывается после создания 'DayZGame'
	// Called after creation 'DayZGame'
	void Init();

	// Вызываются перед вызовом 'PluginManagerInit'
	// Called before calling 'PluginManagerInit'

	// ### Стек вызовов при создании миссии: ###
	//  OnPreCreateMission();
	//  OnCreateMission();
	//  OnAfterCreateMission();

	//  --- Клиент ---
	//	OnPreCreateMissionGameplay();
	//	create PlayerManagerClient "с этого момента доступен" GetPlayerManagerClient();
	//	OnCreateMissionGameplay();
	//	OnAfterCreateMissionGameplay();

	//  --- Сервер ---
	//	OnPreCreateMissionServer();
	//	create PlayerManagers "с этого момента доступен" GetPlayerManagers();
	//	OnCreateMissionServer();
	//	OnAfterCreateMissionServer();

	void OnPreCreateMission();
	void OnPreCreateMissionGameplay();
	void OnPreCreateMissionServer();

	void OnCreateMission();
	void OnCreateMissionGameplay();
	void OnCreateMissionServer();

	void OnAfterCreateMission();
	void OnAfterCreateMissionGameplay();
	void OnAfterCreateMissionServer();


	void OnPreInitMission();
	void OnPreInitMissionGameplay();
	void OnPreInitMissionServer();

	void OnInitMission();
	void OnInitMissionGameplay();
	void OnInitMissionServer();

	void OnAfterInitMission();
	void OnAfterInitMissionGameplay();
	void OnAfterInitMissionServer();

	// Вызываются после вызова 'PluginManagerDelete'
	// Called after call 'PluginManagerDelete'
	void OnPreDestroyMission();
	void OnPreDestroyMissionGameplay();
	void OnPreDestroyMissionServer();

	void OnDestroyMission();
	void OnDestroyMissionGameplay();
	void OnDestroyMissionServer();

	void OnAfterDestroyMission();
	void OnAfterDestroyMissionGameplay();
	void OnAfterDestroyMissionServer();

	// Название сервера, которое указано в 'serverDZ.cfg'
	// The name of the server, which is specified in 'serverDZ.cfg'
	// Server side only
	string GetServerName();
	string GetStorageFolder();

	void RegisterProfilesOptions();

	void SendChatMsg(PlayerIdentity recipient, string name, string message, string colorName = "", int channel = 0);

	NewStoryGroupProfilesOptions GetProfilesOptions();

	NewStoryGroupRPC RPC();
	NewStoryGroupAPI GetAPI();
	NewStoryGroupDiscordLog GetDiscordLog();

	INewStoryGroupPlayerManagersServer GetPlayerManagers();
	INewStoryGroupPlayerManagerBase GetPlayerManagerClient();
	NewStoryGroupBasePlayerInfo GetBasePlayerInfo();
	NewStoryGroupClientPlayerList GetClientPlayerList();
	NewStoryGroupGameTimeEvents GetGameTimeEvents();
	NewStoryGroupEventContainerKeyHandler GetEventContainerKeyHandler();
	NewStoryGroupCategoryList GetCategoryList();
}

NewStoryGroupCore NewStoryGroup()
{
	if ( !NewStoryGroupCore.m_Instance ) {
		NewStoryGroupCore.m_Instance = new NewStoryGroupCore();
	}

	return NewStoryGroupCore.m_Instance;
}
