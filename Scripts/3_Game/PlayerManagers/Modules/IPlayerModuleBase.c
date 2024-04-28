class INewStoryGroupPlayerModuleBase: Managed
{
	void OnInit();

	bool OnRPC(int type, ParamsReadContext ctx);

	// --- Server side only ---
	void OnStartConnection(PlayerIdentity identity);
	void OnCreateCharacter(PlayerIdentity identity, Man character);
	void OnConnect();
	void OnRespawn();
	void OnDisconnected(string uid);
	// -------------------------

	PlayerIdentity GetPlayerIdentity();
	Man GetPlayer();
}
