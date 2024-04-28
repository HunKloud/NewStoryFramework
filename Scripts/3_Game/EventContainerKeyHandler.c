enum ENewStoryGroupEventContainerKeyAction
{
	SINGLE = 0,
	CONTINUOUS,
}

class NewStoryGroupEventContainerKeyData
{
	string ItemName;
	int UseCount;
	string ActionInfoText;
	int UnlockedTime;
}

class NewStoryGroupEventContainerKeyHandler
{
	private static const string CONFIG_PATH = NEW_STORY_GROUP_CONFIG_FOLDER + "EventContainerKeyPresets.json";

	private ref map<int, ref array<ref NewStoryGroupEventContainerKeyData>> _KeyPresets;
	
	// Сразу отделяю данные которые нужны на клиенте, т.к. на сервере планируется использовать куда больше
	//  и передавать избытычные не вижу смысла, хотя и гемор при записи/чтении

	// Param - Param1<int>( preset count )
	// Param - Param1<int>( presetNameHash )
	// Param - Param1<int>( element count )
	// Param - Param3<string, string, int>( ItemName, ActionInfoText, UnlockedTime )
	private ref array<ref Param> _SendToClientParams;

	void NewStoryGroupEventContainerKeyHandler()
	{
		NewStoryGroup().RPC().Add( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_EVENT_CONTAINER_KEYS, ScriptCaller.Create( ReadRPC ), ENewStoryGroupRPCInstance.CLIENT );
	}

	void OnInitMissionServer()
	{
		map<string, ref array<ref NewStoryGroupEventContainerKeyData>> presetsConfig;
		JsonFileLoader<map<string, ref array<ref NewStoryGroupEventContainerKeyData>>>.JsonLoadFile( CONFIG_PATH, presetsConfig );
		
		if ( !presetsConfig && !FileExist( CONFIG_PATH ) ) {
			CreateDefaultConfig();
			return;
		}

		NewStoryGroupEventContainerKeyData keyData;

		foreach ( string presetName, array<ref NewStoryGroupEventContainerKeyData> keys: presetsConfig ) {
			string presetNameBuffer = presetName;
			presetNameBuffer.ToLower();

			int presetNameHash = presetNameBuffer.Hash();

			if ( _KeyPresets && _KeyPresets.Contains( presetNameHash ) ) {
				ErrorEx( string.Format( "[NSG EventContainerKeyPresets] Preset \"%1\" already exists!", presetName ), ErrorExSeverity.WARNING );
				continue;
			}

			for ( int i = keys.Count() - 1; i >= 0; i-- ) {
				keyData = keys[i];

				if ( keyData.ItemName != "" && !NewStoryGroupObjectFunctuon.ItemHasConfig( "", CFG_VEHICLESPATH + " " + keyData.ItemName ) ) {
					ErrorEx( string.Format( "[NSG EventContainerKeyPresets] Item with name \"%1\" is not found in game!", keyData.ItemName ), ErrorExSeverity.WARNING );
					keys.Remove(i);
					continue;
				}

				keyData.ItemName.ToLower();
				keyData.UnlockedTime = Math.Max( keyData.UnlockedTime, 0 );
				keyData.UseCount = Math.Max( keyData.UseCount, 1 );
			}

			if ( keys.Count() == 0 ) {
				ErrorEx( string.Format( "[NSG EventContainerKeyPresets] Preset \"%1\" requires at least 1 key item!", presetName ), ErrorExSeverity.WARNING );
				continue;
			}

			if ( !_KeyPresets ) {
				_KeyPresets = new map<int, ref array<ref NewStoryGroupEventContainerKeyData>>();
			}

			_KeyPresets.Insert( presetNameHash, keys );
		}

		if ( _KeyPresets ) {
			LoadClientSyncData();
		}

		if ( _SendToClientParams ) {
			NewStoryGroup().GetPlayerManagers().Event_OnStartConnection().Insert( OnPlayerStartConnection );
		}
	}

	private void CreateDefaultConfig()
	{
		map<string, ref array<ref NewStoryGroupEventContainerKeyData>> demoConfig = new map<string, ref array<ref NewStoryGroupEventContainerKeyData>>();

		demoConfig.Insert( "DemoPreset1", new array<ref NewStoryGroupEventContainerKeyData>() );
		demoConfig.Insert( "DemoPreset2", new array<ref NewStoryGroupEventContainerKeyData>() );

		demoConfig.GetElement(0).Insert( new NewStoryGroupEventContainerKeyData() );
		demoConfig.GetElement(0).Insert( new NewStoryGroupEventContainerKeyData() );

		demoConfig.GetElement(0).Get(0).ItemName = "";
		demoConfig.GetElement(0).Get(1).ActionInfoText = "";
		demoConfig.GetElement(0).Get(0).UnlockedTime = 60;

		demoConfig.GetElement(0).Get(1).ItemName = "Lockpick";
		demoConfig.GetElement(0).Get(1).UseCount = 2;
		demoConfig.GetElement(0).Get(1).ActionInfoText = "Вскрыть";
		demoConfig.GetElement(0).Get(1).UnlockedTime = 30;

		demoConfig.GetElement(1).Insert( new NewStoryGroupEventContainerKeyData() );
		demoConfig.GetElement(1).Insert( new NewStoryGroupEventContainerKeyData() );

		demoConfig.GetElement(1).Get(0).ItemName = "HandcuffKeys";
		demoConfig.GetElement(1).Get(0).UseCount = 3;
		demoConfig.GetElement(0).Get(1).ActionInfoText = "Открыть";
		demoConfig.GetElement(1).Get(0).UnlockedTime = 5;

		demoConfig.GetElement(1).Get(1).ItemName = "Pliers";
		demoConfig.GetElement(1).Get(1).UseCount = 1;
		demoConfig.GetElement(0).Get(1).ActionInfoText = "Взломать";
		demoConfig.GetElement(1).Get(1).UnlockedTime = 20;

		JsonFileLoader<map<string, ref array<ref NewStoryGroupEventContainerKeyData>>>.JsonSaveFile( CONFIG_PATH, demoConfig );
	}

	private void LoadClientSyncData()
	{
		_SendToClientParams = new array<ref Param>();
		_SendToClientParams.Insert( new Param1<int>( _KeyPresets.Count() ) );

		NewStoryGroupEventContainerKeyData keyData;

		foreach( int presetNameHash, array<ref NewStoryGroupEventContainerKeyData> keys: _KeyPresets ) {
			_SendToClientParams.Insert( new Param1<int>( presetNameHash ) );
			_SendToClientParams.Insert( new Param1<int>( keys.Count() ) );

			for( int i = 0; i < keys.Count(); i++ ) {
				keyData = keys[i];
				_SendToClientParams.Insert( new Param3<string, string, int>( keyData.ItemName, keyData.ActionInfoText, keyData.UnlockedTime ) );
			}
		}
	}

	void OnDestroyMission()
	{
		_KeyPresets = null;
	}

	void OnPlayerStartConnection(PlayerIdentity identity)
	{
		NewStoryGroup().RPC().Send( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_EVENT_CONTAINER_KEYS, _SendToClientParams, identity );
	}

	void ReadRPC(PlayerIdentity sender, ParamsReadContext ctx)
	{
		Param1<int> paramPresetCount;

		if ( !ctx.Read( paramPresetCount ) ) {
			ErrorEx( "[NSG EventContainerKeyPresets] Can't read param 'preset count'", ErrorExSeverity.WARNING );
			return;
		}

		for ( int i = 0; i < paramPresetCount.param1; i++ ) {
			Param1<int> paramPresetNameHash;

			if ( !ctx.Read( paramPresetNameHash ) ) {
				ErrorEx( "[NSG EventContainerKeyPresets] Can't read param 'preset name'", ErrorExSeverity.WARNING );
				return;
			}

			Param1<int> paramKeyCount;

			if ( !ctx.Read( paramKeyCount ) ) {
				ErrorEx( "[NSG EventContainerKeyPresets] Can't read param 'key count'", ErrorExSeverity.WARNING );
				return;
			}

			array<ref NewStoryGroupEventContainerKeyData> presets = new array<ref NewStoryGroupEventContainerKeyData>();

			for ( int j = 0; j < paramKeyCount.param1; j++ ) {
				Param3<string, string, int> paramKeyData;

				if ( !ctx.Read( paramKeyData ) ) {
					ErrorEx( "[NSG EventContainerKeyPresets] Can't read param 'key data'", ErrorExSeverity.WARNING );
					return;
				}

				NewStoryGroupEventContainerKeyData data = new NewStoryGroupEventContainerKeyData();

				data.ItemName	   = paramKeyData.param1;
				data.ActionInfoText = paramKeyData.param2;
				data.UnlockedTime   = paramKeyData.param3;

				presets.Insert( data );
			}

			if ( !_KeyPresets ) {
				_KeyPresets = new map<int, ref array<ref NewStoryGroupEventContainerKeyData>>();
			}

			_KeyPresets.Insert( paramPresetNameHash.param1, presets );
		}
	}

	bool HasPreset(int presetHash)
	{
		return _KeyPresets && _KeyPresets.Contains(presetHash);
	}

	bool CanUnlocked(int presetHash, string itemName)
	{
		if ( !_KeyPresets ) {
			return false;
		}

		itemName.ToLower();

		array<ref NewStoryGroupEventContainerKeyData> keys = _KeyPresets.Get( presetHash );

		if ( !keys ) {
			return false;
		}

		NewStoryGroupEventContainerKeyData keyData;
		bool hasHands = false;

		for ( int i = 0; i < keys.Count(); i++ ) {
			keyData = keys[i];

			if ( keyData.ItemName == "" ) {
				hasHands = true;
				continue;
			}

			if ( keyData.ItemName == itemName ) {
				return true;
			}
		}

		return hasHands;
	}

	int GetUnlockedTime(int presetHash, string itemName)
	{
		if ( !_KeyPresets || presetHash == 0 ) {
			return -1;
		}

		itemName.ToLower();

		array<ref NewStoryGroupEventContainerKeyData> keys = _KeyPresets.Get( presetHash );

		int handsTime = -1;
		NewStoryGroupEventContainerKeyData keyData;

		for ( int i = 0; i < keys.Count(); i++ ) {
			keyData = keys[i];

			if ( keyData.ItemName == "" ) {
				handsTime = keyData.UnlockedTime;
				continue;
			}

			if ( keyData.ItemName == itemName ) {
				return keyData.UnlockedTime;
			}
		}

		return handsTime;
	}

	int GetUseCount(int presetHash, string itemName)
	{
		if ( !_KeyPresets || presetHash == 0 ) {
			return -1;
		}

		itemName.ToLower();

		array<ref NewStoryGroupEventContainerKeyData> keys = _KeyPresets.Get( presetHash );
		NewStoryGroupEventContainerKeyData keyData;

		for ( int i = 0; i < keys.Count(); i++ ) {
			keyData = keys[i];
			if ( keyData.ItemName != "" && keyData.ItemName == itemName ) {
				return keyData.UseCount;
			}
		}

		return -1;
	}

	string GetActionInfoText(int presetHash, string itemName)
	{
		if ( !_KeyPresets || presetHash == 0 ) {
			return "";
		}

		itemName.ToLower();

		array<ref NewStoryGroupEventContainerKeyData> keys = _KeyPresets.Get( presetHash );
		NewStoryGroupEventContainerKeyData keyData;

		string actionText = "";

		for ( int i = 0; i < keys.Count(); i++ ) {
			keyData = keys[i];

			if ( keyData.ItemName == "" ) {
				actionText = keyData.ActionInfoText;
			}

			if ( keyData.ItemName == itemName ) {
				return keyData.ActionInfoText;
			}
		}

		return actionText;
	}

	static string GetKeyTypeName(EntityAI keyItem)
	{
		if ( keyItem && keyItem.IsAlive() ) {
			return keyItem.GetType();
		}

		return "";
	}
}
