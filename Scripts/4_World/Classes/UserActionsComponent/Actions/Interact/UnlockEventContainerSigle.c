class ActionNewStoryGroupUnlockEventContainerSigle: ActionInteractBase
{
	void ActionNewStoryGroupUnlockEventContainerSigle()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#open";
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		NewStoryGroupEventContainerBase eventContainer;
		
		if ( Class.CastTo( eventContainer, target.GetObject() ) ) {
			string keyItemName = NewStoryGroupEventContainerKeyHandler.GetKeyTypeName( player.GetItemInHands() );
			m_Text = NewStoryGroup().GetEventContainerKeyHandler().GetActionInfoText( eventContainer.GetKeyDataHash(), keyItemName );

			if ( m_Text == "" ) {
				m_Text = "#open";
			}
		}
	}

	override protected bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		NewStoryGroupEventContainerBase eventContainer;
		
		if ( !Class.CastTo( eventContainer, target.GetObject() ) || !eventContainer.GetRequiresKey() ) {
			return false;
		}

		if ( eventContainer.GetKeyActionType() != ENewStoryGroupEventContainerKeyAction.SINGLE ) {
			return false;
		}

		string keyItemName = NewStoryGroupEventContainerKeyHandler.GetKeyTypeName( player.GetItemInHands() );
		return NewStoryGroup().GetEventContainerKeyHandler().CanUnlocked( eventContainer.GetKeyDataHash(), keyItemName );
	}

	override protected void OnExecuteServer(ActionData action_data)
	{
		NewStoryGroupEventContainerBase eventContainer;

		if ( Class.CastTo( eventContainer, action_data.m_Target.GetObject() ) ) {
			eventContainer.OnExecuteUnlockActionServer( action_data.m_Player );
		}
	}
}
