class NewStoryGroupRPC: INewStoryGroupRPC
{
	private ref map<string, ref map<int, ref ScriptCaller>> m_RPCS;

	void NewStoryGroupRPC()
	{
		m_RPCS = new map<string, ref map<int, ref ScriptCaller>>();
	}

	override void Add(string key, int type, notnull ScriptCaller callback, int instance_type = ENewStoryGroupRPCInstance.ANY)
	{
		// PrintFormat( "NewStoryGroupRPC::Add key %1 type %2", key, type );
		#ifdef SERVER
		if ( (instance_type & ENewStoryGroupRPCInstance.SERVER) != ENewStoryGroupRPCInstance.SERVER ) {
			// PrintFormat( "Server check instance_type %1", instance_type );
			return;
		}
		#else
		if ( (instance_type & ENewStoryGroupRPCInstance.CLIENT) != ENewStoryGroupRPCInstance.CLIENT ) {
			// PrintFormat( "Client check instance_type %1", instance_type );
			return;
		}
		#endif

		map<int, ref ScriptCaller> list;

		if ( !m_RPCS.Find( key, list ) ) {
			list = new map<int, ref ScriptCaller>();
			m_RPCS.Insert( key, list );
			// PrintFormat( "Add key %1", key );
		}
		else if ( list.Contains( type ) ) {
			ErrorEx( string.Format( "type = %1 is registered in key = %2", type, key ), ErrorExSeverity.WARNING );
			return;
		}

		list.Insert( type, callback );
		// PrintFormat( "Add type %1", type );
	}

	override void Remove(string key, int type, int instance_type = ENewStoryGroupRPCInstance.ANY)
	{
		// PrintFormat( "NewStoryGroupRPC::Remove key %1 type %2", key, type );
		#ifdef SERVER
		if ( !(instance_type & ENewStoryGroupRPCInstance.SERVER) ) {
			// PrintFormat( "Check instance_type %1", instance_type );
			return;
		}
		#else
		if ( !(instance_type & ENewStoryGroupRPCInstance.CLIENT) ) {
			// PrintFormat( "Check instance_type %1", instance_type );
			return;
		}
		#endif

		map<int, ref ScriptCaller> list;

		if ( m_RPCS.Find( key, list ) ) {
			list.Remove(type);
			// PrintFormat( "Remove type %1", type );
			if ( list.Count() == 0 ) {
				m_RPCS.Remove(key);
				// PrintFormat( "Remove key %1", key );
			}
		}
		else {
			// PrintFormat( "Can't find key %1", key );
		}
	}

	override void Send(string key, int type, array<ref Param> params, PlayerIdentity recipient = null)
	{
		Param2<string, int> paramKeys = new Param2<string, int>(key, type);

		array<ref Param> sendParams = params;

		if ( !sendParams ) {
			sendParams = new array<ref Param>();
			sendParams.Insert( paramKeys );
		}
		else {
			sendParams.InsertAt( paramKeys, 0 );
		}

		GetGame().RPC( null, ENewStoryGroupRPC.RPC_MANAGER, sendParams, true, recipient );
	}

	override void SendToPlayers(string key, int type, array<ref Param> params, notnull array<PlayerIdentity> recipients)
	{
		params.InsertAt( new Param2<string, int>(key, type), 0 );

		for ( int i = 0; i < recipients.Count(); i++ ) {
			PlayerIdentity recipient = recipients[i];
			if ( recipient ) {
				GetGame().RPC( null, ENewStoryGroupRPC.RPC_MANAGER, params, true, recipient );
			}
		}
	}

	override void SendSingleParam(string key, int type, Param param, PlayerIdentity recipient = null)
	{
		array<ref Param> sendParams = new array<ref Param>();
		sendParams.Insert( new Param2<string, int>(key, type) );

		if ( param ) {
			sendParams.Insert( param );
		}

		GetGame().RPC( null, ENewStoryGroupRPC.RPC_MANAGER, sendParams, true, recipient );
	}

	override void SendSingleParamToPlayers(string key, int type, Param param, notnull array<PlayerIdentity> recipients)
	{
		array<ref Param> sendParams = new array<ref Param>();

		sendParams.Insert( new Param2<string, int>(key, type) );
		
		if ( param ) {
			sendParams.Insert(param);
		}

		for ( int i = 0; i < recipients.Count(); i++ ) {
			PlayerIdentity recipient = recipients[i];
			if ( recipient ) {
				GetGame().RPC( null, ENewStoryGroupRPC.RPC_MANAGER, sendParams, true, recipient );
			}
		}
	}

	void OnRPC(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Param2<string, int> paramKeys;

		if ( !ctx.Read( paramKeys ) ) {
			ErrorEx( "Can't read paramKeys" );
			return;
		}

		map<int, ref ScriptCaller> list;
		ScriptCaller callback;

		if ( !m_RPCS.Find( paramKeys.param1, list ) ) {
			ErrorEx( string.Format( "Can't find key = %1", paramKeys.param1 ) );
			return;
		}

		if ( !list.Find( paramKeys.param2, callback ) ) {
			ErrorEx( string.Format( "Can't find callback with id = %1 in key = %2", paramKeys.param2, paramKeys.param1 ) );
			return;
		}

		callback.Invoke( sender, ctx );
	}

	void DebugLog()
	{
		Print( "NewStoryGroupRPC::DebugLog" );

		foreach ( string key, auto list: m_RPCS ) {
			for ( int i = 0; i < list.Count(); i++ ) {
				PrintFormat( "key = %1 | type = %2", key, list.GetKey(i) );
			}
		}
	}
}
