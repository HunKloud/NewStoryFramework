class INewStoryGroupPlayerManagerBase: Managed
{
	protected void PreInitModule(NewStoryGroupPlayerModuleBase module);
	protected void AfterInitModule(NewStoryGroupPlayerModuleBase module);

	NewStoryGroupPlayerModuleBase GetModule(typename type);

	protected void AfterStartConnection(PlayerIdentity identity);
	protected void AfterCreateCharacter(PlayerIdentity identity, Man character);
	protected void AfterConnect(PlayerIdentity identity);
	protected void AfterRespawn(PlayerIdentity identity);
	protected void AfterDisconnected(string uid);

	void OnRPC(ParamsReadContext ctx);

	void RPC(int type, array<ref Param> params);
	void RPCSingleParam(int type, Param param = null);

	PlayerIdentity GetPlayerIdentity();
}
