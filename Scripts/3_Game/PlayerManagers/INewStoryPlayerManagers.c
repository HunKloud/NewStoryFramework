class INewStoryGroupPlayerManagersServer
{
	void OnStartConnection(PlayerIdentity identity);
	void OnCreateCharacter(PlayerIdentity identity, Man character);
	void OnConnect(PlayerIdentity identity);
	void Disconnected(string uid);

	// Возвращает кол-во игроков на сервере
	int GetPlayerCount();

	// Возвращает менеджер игрока по UID
	INewStoryGroupPlayerManagerBase GetPlayerManager(string uid);

	// Если надо что бы происходил вызов метода при обновлении кол-ва игроков на сервере
	// void your_function_name(int count);
	ScriptInvoker Event_OnUpdatePlayerCount();

	// Вызывается каждый раз когда игрок начинает подкоючение к серверу
	// (PlayerIdentity identity)
	ScriptInvoker Event_OnStartConnection();

	// Вызывается когда игрок подключился к серверу
	// (PlayerIdentity identity)
	ScriptInvoker Event_OnConnect();

	// Вызывается когда игрок отключился от сервера
	// (string uid)
	ScriptInvoker Event_OnDisconnected();
}
