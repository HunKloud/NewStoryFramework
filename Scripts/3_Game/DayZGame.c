modded class DayZGame: CGame
{
	void DayZGame()
	{
		NewStoryGroup().Init();
	}

	override void DeferredInit()
	{
		super.DeferredInit();
		NewStoryGroup().RegisterProfilesOptions();
	}

	#ifdef SERVER
	override void GlobalsInit()
	{
		super.GlobalsInit();
		NewStoryGroup().LoadServerName();
	}
	#endif

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if ( !NewStoryGroup().OnRPC( sender, target, rpc_type, ctx ) ) {
			super.OnRPC( sender, target, rpc_type, ctx );
		}
	}
}
