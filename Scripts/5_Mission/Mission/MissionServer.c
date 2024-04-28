modded class MissionServer: MissionBase
{
	override void OnInit()
	{
		super.OnInit();

		NewStoryGroup().OnPreInitMission();
		NewStoryGroup().OnInitMission();
		NewStoryGroup().OnAfterInitMission();

		NewStoryGroup().OnPreInitMissionServer();
		NewStoryGroup().OnInitMissionServer();
		NewStoryGroup().OnAfterInitMissionServer();
	}

	override protected bool IsNewStoryGroupMissionServer()
	{
		return true;
	}

	// override void OnEvent(EventType eventTypeId, Param params)
	// {
	//	 super.OnEvent( eventTypeId, params );

	//	 ClientPrepareEventTypeID - Перед тем как, новый персонаж создан/выгружен из б.д.
	//	 ClientNewEventTypeID	 - Когда происходит создание нового персонажа (или пользователь первый раз подключается)
	//	 ClientReadyEventTypeID   - Когда новый персонаж создан/выгружен из б.д.
	//	 ClientRespawnEventTypeID - Во время респавна, но еще новый персонаж не создан
	// }

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		super.OnClientPrepareEvent( identity, useDB, pos, yaw, preloadTimeout );
		NewStoryGroup().GetPlayerManagers().OnStartConnection( identity );
	}

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		PlayerBase character = super.OnClientNewEvent(identity, pos, ctx);
		NewStoryGroup().GetPlayerManagers().OnCreateCharacter( identity, character );

		return character;
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		NewStoryGroup().GetPlayerManagers().OnConnect( identity );
	}

	// Если игрок не дождался завершения отсчета то 'identity' = NULL
	override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
	{
		super.PlayerDisconnected(player, identity, uid);
		NewStoryGroup().GetPlayerManagers().Disconnected( uid );
	}
}
