#ifdef SERVER
modded class PluginManager
{
	override void Init()
	{
		super.Init();

		if ( NewStoryGroupCoreSettings.REQUEST_SPAWN_ITEM_PRESETS ) {
			RegisterPlugin( "NewStoryGroupPluginSpawnItemPresets", false, true );
		}
		
		if ( NewStoryGroupCoreSettings.REQUEST_MAPPING_HANDLER ) {
			RegisterPlugin( "NewStoryGroupPluginMappingHandler", false, true );
		}
	}
}
#endif
