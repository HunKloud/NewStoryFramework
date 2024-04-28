modded class MissionGameplay: MissionBase
{
	protected ref NewStoryGroupFlowingWidgetHandler _NewStoryGroupFlowingWidgetHandler;

	void MissionGameplay()
	{
		NewStoryGroup().RPC().Add( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SEND_CHAT_MSG, ScriptCaller.Create( NewStoryGroupReadRPCSendChatMsg ) );
		
		if ( NewStoryGroupCoreSettings.REQUEST_FLOWING_WIDGET_HANDLER ) {
			_NewStoryGroupFlowingWidgetHandler = new NewStoryGroupFlowingWidgetHandler();
		}
	}

	void ~MissionGameplay()
	{
		NewStoryGroup().RPC().Remove( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SEND_CHAT_MSG );
		_NewStoryGroupFlowingWidgetHandler = null;
	}

	override void OnInit()
	{
		super.OnInit();

		NewStoryGroup().OnPreInitMission();
		NewStoryGroup().OnInitMission();
		NewStoryGroup().OnAfterInitMission();

		NewStoryGroup().OnPreInitMissionGameplay();
		NewStoryGroup().OnInitMissionGameplay();
		NewStoryGroup().OnAfterInitMissionGameplay();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		NewStoryGroup().OnStartMission();
		NewStoryGroup().OnStartGameplay();
	}

	override protected bool IsNewStoryGroupMissionGameplay()
	{
		return true;
	}

	NewStoryGroupFlowingWidgetHandler GetNewStoryGroupFlowingWidgetHandler()
	{
		return _NewStoryGroupFlowingWidgetHandler;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if ( _NewStoryGroupFlowingWidgetHandler ) {
			_NewStoryGroupFlowingWidgetHandler.UpdateGame( timeslice );
		}
	}

	void NewStoryGroupReadRPCSendChatMsg(PlayerIdentity sender, ParamsReadContext ctx)
	{
		Param4<string, string, string, int> param;

		if ( !ctx.Read( param ) ) {
			ErrorEx( "Can't read param 'send chat msg'", ErrorExSeverity.WARNING );
			return;
		}

		AddNewStoryGroupChatMessage( param.param1, param.param2, param.param3, param.param4 );
	}

	override void AddNewStoryGroupChatMessage(string name, string message, string colorName = "", int channel = 0)
	{
		if ( m_Chat ) {
			m_Chat.Add( new ChatMessageEventParams( channel, name, message, colorName ) );
		}
	}
}
