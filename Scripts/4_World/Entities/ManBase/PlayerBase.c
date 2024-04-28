modded class PlayerBase: ManBase
{
	protected ref map<int, ItemBase>			m_NewStoryGroupLinkedClothes;
	protected ref NewStoryGroupPlayerHitHandler m_NewStoryGroupPlayerHitHandler;

	void PlayerBase()
	{
		#ifndef SERVER
		if ( NewStoryGroup().GetClientPlayerList() ) {
			NewStoryGroup().GetClientPlayerList().AddPlayer( this );
		}
		#else
		if ( NewStoryGroupCoreSettings.REQUEST_PLAYER_MODULE_KILLS ) {
			m_NewStoryGroupPlayerHitHandler = new NewStoryGroupPlayerHitHandler();
			GetOnHitByInvoker().Insert( m_NewStoryGroupPlayerHitHandler.EEHitBy );
			GetOnKilledInvoker().Insert( m_NewStoryGroupPlayerHitHandler.EEKilled );
		}
		#endif
	}

	#ifndef SERVER
	void ~PlayerBase()
	{
		if ( NewStoryGroup().GetClientPlayerList() ) {
			NewStoryGroup().GetClientPlayerList().RemovePlayer( this );
		}
	}
	#endif

	NewStoryGroupPlayerHitHandler GetNewStoryGroupPlayerHitHandler()
	{
		return m_NewStoryGroupPlayerHitHandler;
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached( item, slot_name );

		ItemBase itemIB = ItemBase.Cast(item);
		set<int> linkedClothes = itemIB.GetNewStoryGroupLinkedClothesSlots();

		if ( !linkedClothes || linkedClothes.Count() == 0 ) {
			return;
		}

		if ( !m_NewStoryGroupLinkedClothes ) {
			m_NewStoryGroupLinkedClothes = new map<int, ItemBase>();
		}

		for ( int i = 0; i < linkedClothes.Count(); i++ ) {
			m_NewStoryGroupLinkedClothes.Insert( linkedClothes[i], itemIB );
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		ItemBase itemIB = ItemBase.Cast(item);
		set<int> linkedClothes = itemIB.GetNewStoryGroupLinkedClothesSlots();

		if ( !linkedClothes || linkedClothes.Count() == 0 ) {
			return;
		}

		for ( int i = 0; i < linkedClothes.Count(); i++ ) {
			m_NewStoryGroupLinkedClothes.Remove( linkedClothes[i] );
		}
	}

	ItemBase GetNewStoryGroupLinkedClothes(int slot)
	{
		if ( m_NewStoryGroupLinkedClothes ) {
			return m_NewStoryGroupLinkedClothes.Get(slot);
		}

		return null;
	}
}
