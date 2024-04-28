class NewStoryGroupPlayerManagerModulesWorld: NewStoryGroupPlayerManagerModulesGame
{
	override void RegisterModules(set<typename> modules)
	{
		super.RegisterModules(modules);
		
		if ( NewStoryGroupCoreSettings.REQUEST_PLAYER_MODULE_KILLS ) {
			modules.Insert( NewStoryGroupPlayerModuleKills );
		}
	}
}
