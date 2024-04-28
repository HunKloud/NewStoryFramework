class ActionNewStoryGroupOpenEventContainer: ActionInteractBase
{
	void ActionNewStoryGroupOpenEventContainer()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#open";
	}

	override protected bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		NewStoryGroupEventContainerBase eventContainer;
		return Class.CastTo( eventContainer, target.GetObject() ) && !eventContainer.GetRequiresKey() && !eventContainer.IsInventoryVisible();
	}

	override protected void OnExecuteServer(ActionData action_data)
	{
		NewStoryGroupEventContainerBase eventContainer;

		if ( Class.CastTo( eventContainer, action_data.m_Target.GetObject() ) ) {
			eventContainer.Open();
		}
	}
}
