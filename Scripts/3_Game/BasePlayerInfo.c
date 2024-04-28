class NewStoryGroupBasePlayerInfo
{
	private string _Name;
	private string _SteamID;
	private string _UID;

	void NewStoryGroupBasePlayerInfo()
	{
		NewStoryGroup().RPC().Add( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_BASE_PLAYER_INFO, ScriptCaller.Create( ReadRPC ), ENewStoryGroupRPCInstance.CLIENT );
	}

	void OnInitMissionServer()
	{
		NewStoryGroup().GetPlayerManagers().Event_OnStartConnection().Insert( OnPlayerStartConnection );
	}

	void OnPlayerStartConnection(PlayerIdentity identity)
	{
		_Name	   = identity.GetName();
		_SteamID	= identity.GetPlainId();
		_UID		= identity.GetId();

		Param param = new Param3<string, string, string>( _Name, _SteamID, _UID );
		NewStoryGroup().RPC().SendSingleParam( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_BASE_PLAYER_INFO, param, identity );
	}

	void ReadRPC(PlayerIdentity sender, ParamsReadContext ctx)
	{
		Param3<string, string, string> param;

		if ( !ctx.Read( param ) ) {
			ErrorEx( "Can't read param 'base player info'", ErrorExSeverity.WARNING );
			return;
		}

		_Name	   = param.param1;
		_SteamID	= param.param2;
		_UID		= param.param3;
	}

	string GetName()
	{
		return _Name;
	}

	string GetSteamID()
	{
		return _SteamID;
	}

	string GetUID()
	{
		return _UID;
	}
}
