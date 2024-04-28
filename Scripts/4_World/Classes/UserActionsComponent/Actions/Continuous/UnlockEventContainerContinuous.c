class ActionNewStoryGroupUnlockEventContainerContinuousCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
        NewStoryGroupEventContainerBase eventContainer;
        Class.CastTo( eventContainer, m_ActionData.m_Target.GetObject() );

        string keyItemName = NewStoryGroupEventContainerKeyHandler.GetKeyTypeName( m_ActionData.m_Player.GetItemInHands() );
        int time = NewStoryGroup().GetEventContainerKeyHandler().GetUnlockedTime( eventContainer.GetKeyDataHash(), keyItemName );

		m_ActionData.m_ActionComponent = new CAContinuousTime( time );
	}
}

class ActionNewStoryGroupUnlockEventContainerContinuous: ActionContinuousBase
{
    void ActionNewStoryGroupUnlockEventContainerContinuous()
	{
		m_CallbackClass = ActionNewStoryGroupUnlockEventContainerContinuousCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_Text = "#open";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem     = new CCINone();
		m_ConditionTarget   = new CCTObject(UAMaxDistances.DEFAULT);
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

    #ifndef SERVER
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = null)
	{
        if ( !super.SetupAction( player, target, item, action_data, extra_data ) ) {
            return false;
        }

        vector targetPosition = target.GetObject().GetPosition();

        if ( vector.Distance( player.GetPosition(), targetPosition ) < 0.5 ) {
            vector headPosition;
            MiscGameplayFunctions.GetHeadBonePos( player, headPosition );

            if ( vector.Distance( headPosition, targetPosition ) < 0.9 ) {
                m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
                return true;
            }
        }

        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        return true;
	}
    #endif
    
    override protected bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        NewStoryGroupEventContainerBase eventContainer;
        
        if ( !Class.CastTo( eventContainer, target.GetObject() ) || !eventContainer.GetRequiresKey() ) {
            return false;
        }

        if ( eventContainer.GetKeyActionType() != ENewStoryGroupEventContainerKeyAction.CONTINUOUS ) {
            return false;
        }

        string keyItemName = NewStoryGroupEventContainerKeyHandler.GetKeyTypeName( player.GetItemInHands() );
        return NewStoryGroup().GetEventContainerKeyHandler().CanUnlocked( eventContainer.GetKeyDataHash(), keyItemName );
    }

    override protected bool ActionConditionContinue( ActionData action_data )
	{
        NewStoryGroupEventContainerBase eventContainer = NewStoryGroupEventContainerBase.Cast( action_data.m_Target.GetObject() );
		return eventContainer && !eventContainer.IsInventoryVisible();
	}

    override protected void OnFinishProgressServer(ActionData action_data)
    {
        NewStoryGroupEventContainerBase eventContainer;

        if ( Class.CastTo( eventContainer, action_data.m_Target.GetObject() ) ) {
            eventContainer.OnExecuteUnlockActionServer( action_data.m_Player );
        }
    }
}
