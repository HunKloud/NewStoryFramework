class NewStoryGroupEventContainerBase: ItemBase
{
	protected ref OpenableBehaviour _Openable;
	
	protected bool _RequiresKey;
	protected int _KeyDataHash;
	protected int _KeyActionType;
	
	protected ref array<int> _SpawnItemPresets;

	protected bool _SpawnOnePreset;
	protected bool _IgnoreSpawnPresetChances;
	protected bool _IgnoreSpawnPresetItemChances;

	protected int _RadiusDestroyObject;

	void NewStoryGroupEventContainerBase()
	{
		_Openable	   = new OpenableBehaviour(false);
		_KeyDataHash	= 0;
		_KeyActionType  = 0;
		_RequiresKey	= false;

		_SpawnOnePreset				 = false;
		_IgnoreSpawnPresetChances	   = false;
		_IgnoreSpawnPresetItemChances   = false;
		
		_RadiusDestroyObject = 0;

		RegisterNetSyncVariableBool( "_Openable.m_IsOpened" );
		RegisterNetSyncVariableBool( "m_IsSoundSynchRemote" );

		RegisterNetSyncVariableBool( "_RequiresKey" );
		RegisterNetSyncVariableInt( "_KeyDataHash" );
		RegisterNetSyncVariableInt( "_KeyActionType" );
	}

	bool GetRequiresKey()
	{
		return _RequiresKey && _KeyDataHash != 0;
	}

	int GetKeyDataHash()
	{
		return _KeyDataHash;
	}

	void SetKeyDataHash(int keyDataHash, bool requiresCheck = true)
	{
		if ( !NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			return;
		}

		if ( requiresCheck && !NewStoryGroup().GetEventContainerKeyHandler().HasPreset( keyDataHash ) ) {
			return;
		}

		_RequiresKey = true;
		_KeyDataHash = keyDataHash;
		SetSynchDirty();
	}

	int GetKeyActionType()
	{
		return _KeyActionType;
	}

	void SetKeyActionType(int keyActionType)
	{
		_KeyActionType = Math.Clamp(
			keyActionType,
			ENewStoryGroupEventContainerKeyAction.SINGLE,
			ENewStoryGroupEventContainerKeyAction.CONTINUOUS );

		SetSynchDirty();
	}

	void SetSpawnItemPresets(notnull array<int> presets, bool onlyOnePreset = false, bool ignorePresetChances = false, bool ignoreItemChances = false, bool requiresCheck = true)
	{
		if ( !NewStoryGroupCoreSettings.REQUEST_SPAWN_ITEM_PRESETS ) {
			return;
		}

		array<int> setPresets = new array<int>();
		setPresets.Copy( presets );

		if ( requiresCheck ) {
			NewStoryGroupPluginSpawnItemPresets pluginSpawnItemPresets;
			Class.CastTo( pluginSpawnItemPresets, GetPlugin( NewStoryGroupPluginSpawnItemPresets ) );
			pluginSpawnItemPresets.CorrectedPresetList( setPresets );
		}

		if ( setPresets.Count() != 0 ) {
			_SpawnItemPresets			   = setPresets;
			_SpawnOnePreset				 = onlyOnePreset;
			_IgnoreSpawnPresetChances	   = ignorePresetChances;
			_IgnoreSpawnPresetItemChances   = ignoreItemChances;
		}
	}

	void SetRadiusDestroyObject(int radius)
	{
		_RadiusDestroyObject = Math.Max( radius, 0 );
	}

	void OnExecuteUnlockActionServer(PlayerBase player)
	{
		ItemBase keyItem = player.GetItemInHands();

		if ( !keyItem || keyItem.IsDamageDestroyed() ) {
			return;
		}

		string keyItemName = keyItem.GetType();
		int useCount = NewStoryGroup().GetEventContainerKeyHandler().GetUseCount( _KeyDataHash, keyItemName );

		if ( useCount == -1 ) {
			return;
		}

		float decreaseHealth = Math.Ceil( keyItem.GetMaxHealth() / useCount );
		keyItem.DecreaseHealth( decreaseHealth, false );
		
		_RequiresKey = false;
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( IsSoundSynchRemote() && m_Initialized ) {
			if ( IsOpen() ) {
				SoundOpenPlay();
			}
			else if ( !IsOpen() ) {
				SoundClosePlay();
			}
		}

		UpdateVisualState();
	}

	protected void UpdateVisualState();
	protected void SoundOpenPlay();
	protected void SoundClosePlay();

	override bool IsInventoryVisible()
	{
		return _Openable.IsOpened();
	}

	override bool IsHealthVisible()
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

	#ifdef SERVER
	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		if ( !HasAnyCargo() ) {
			SetLifetime(0);
		}
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		if ( _RadiusDestroyObject > 0 && _Openable.IsOpened() && GetCEApi().AvoidPlayer( GetPosition(), _RadiusDestroyObject ) ) {
			Delete();
		}
	}

	override void Open()
	{
		if ( _Openable.IsOpened() ) {
			return;
		}

		if ( _SpawnItemPresets ) {
			NewStoryGroupPluginSpawnItemPresets pluginSpawnItemPresets;
			Class.CastTo( pluginSpawnItemPresets, GetPlugin( NewStoryGroupPluginSpawnItemPresets ) );

			if ( _SpawnOnePreset ) {
				pluginSpawnItemPresets.SpawnOneFromPresets( this, _SpawnItemPresets, _IgnoreSpawnPresetChances, _IgnoreSpawnPresetItemChances );
			}
			else {
				pluginSpawnItemPresets.SpawnPresets( this, _SpawnItemPresets, _IgnoreSpawnPresetChances, _IgnoreSpawnPresetItemChances );
			}

			_SpawnItemPresets = null;

			_SpawnOnePreset				 = false;
			_IgnoreSpawnPresetChances	   = false;
			_IgnoreSpawnPresetItemChances   = false;
		}

		_Openable.Open();
		SoundSynchRemote();
		UpdateVisualState();
	}

	override void Close()
	{
		if ( _Openable.IsOpened() ) {
			_Openable.Close();
			SoundSynchRemote();
			UpdateVisualState();
		}
	}
	#endif

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave( ctx );

		ctx.Write( _RequiresKey );

		if ( _RequiresKey ) {
			ctx.Write( _KeyDataHash );
			ctx.Write( _KeyActionType );
		}

		int spawnItemPresetCount = 0;

		if ( _SpawnItemPresets ) {
			spawnItemPresetCount = _SpawnItemPresets.Count();
			ctx.Write( spawnItemPresetCount );

			int spawnItemPresetHash;

			for ( int i = 0; i < spawnItemPresetCount; i++ ) {
				spawnItemPresetHash = _SpawnItemPresets[i];
				ctx.Write( spawnItemPresetHash );
			}

			ctx.Write( _SpawnOnePreset );
			ctx.Write( _IgnoreSpawnPresetChances );
			ctx.Write( _IgnoreSpawnPresetItemChances );
		}
		else {
			ctx.Write( spawnItemPresetCount );
		}

		ctx.Write( _Openable.IsOpened() );
		ctx.Write( _RadiusDestroyObject );
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( !super.OnStoreLoad( ctx, version ) ) {
			return false;
		}

		if ( !ctx.Read( _RequiresKey ) ) {
			return false;
		}

		if ( _RequiresKey && ( !ctx.Read( _KeyDataHash ) || !ctx.Read( _KeyActionType ) ) ) {
			return false;
		}

		int spawnItemPresetCount;
		
		if ( !ctx.Read( spawnItemPresetCount ) ) {
			return false;
		}

		if ( spawnItemPresetCount != 0 ) {
			_SpawnItemPresets = new array<int>();
			int spawnItemPresetHash;

			NewStoryGroupPluginSpawnItemPresets pluginSpawnItemPresets;
			Class.CastTo( pluginSpawnItemPresets, GetPlugin( NewStoryGroupPluginSpawnItemPresets ) );

			for ( int i = 0; i < spawnItemPresetCount; i++ ) {
				if ( !ctx.Read( spawnItemPresetHash ) ) {
					return false;
				}

				if ( pluginSpawnItemPresets && pluginSpawnItemPresets.HasPreset( spawnItemPresetHash ) ) {
					_SpawnItemPresets.Insert( spawnItemPresetHash );
				}
			}

			if ( !ctx.Read( _SpawnOnePreset ) ) {
				return false;
			}

			if ( !ctx.Read( _IgnoreSpawnPresetChances ) ) {
				return false;
			}

			if ( !ctx.Read( _IgnoreSpawnPresetItemChances ) ) {
				return false;
			}
		}

		bool opened;

		if ( !ctx.Read( opened ) ) {
			return false;
		}

		if ( !ctx.Read( _RadiusDestroyObject ) ) {
			return false;
		}

		if ( opened ) {
			_Openable.Open();
		}
		else {
			_Openable.Close();
		}

		UpdateVisualState();
		SetSynchDirty();

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		bool reqDelete = false;

		if ( !NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			reqDelete = true;
		}
		else if ( _KeyDataHash != 0 && !NewStoryGroup().GetEventContainerKeyHandler().HasPreset( _KeyDataHash ) ){
			reqDelete = true;
		}
		else if ( _SpawnItemPresets && _SpawnItemPresets.Count() == 0 ) {
			reqDelete = true;
		}

		if ( reqDelete ) {
			Delete();
		}
	}

	override void SetActions()
	{
		super.SetActions();

		RemoveAction(ActionTakeItem);
		RemoveAction(ActionTakeItemToHands);
		RemoveAction(ActionWorldCraft);
		RemoveAction(ActionDropItem);
		
		if ( NewStoryGroupCoreSettings.REQUEST_SPAWN_EVENT_CONTAINER ) {
			AddAction(ActionNewStoryGroupOpenEventContainer);
			AddAction(ActionNewStoryGroupUnlockEventContainerSigle);
			AddAction(ActionNewStoryGroupUnlockEventContainerContinuous);
		}
	}

	// ########################
	// #### STATIC METHODS ####
	// ########################

	// keyActionType = 0 -> ENewStoryGroupEventContainerKeyAction.SINGLE
	static NewStoryGroupEventContainerBase Create(string typeName, vector position, vector orientation, int keyDataHash = 0, int keyActionType = 0, bool requiresCheckKeyData = false)
	{
		NewStoryGroupEventContainerBase eventContainer;

		if ( !Class.CastTo( eventContainer, GetGame().CreateObjectEx( typeName, position, ECE_PLACE_ON_SURFACE ) ) ) {
			return null;
		}

		eventContainer.SetOrientation( orientation );
		eventContainer.SetKeyDataHash( keyDataHash, requiresCheckKeyData );
		eventContainer.SetKeyActionType( keyActionType );

		return eventContainer;
	}
}

class NewStoryGroupEventContainer_WoodenCrate: NewStoryGroupEventContainerBase {}
