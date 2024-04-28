modded class ItemBase: InventoryItem
{
	// (ItemBase item, int oldLevel, int newLevel, string zone)
	protected ref ScriptInvoker m_OnNSgHealthLevelChanged;
	// (ItemBase item, int oldLevel)
	protected ref ScriptInvoker m_OnNSgDamageDestroyed;
	// (ItemBase item, float delta)
	protected ref ScriptInvoker m_OnNSgQuantityChanged;

	protected ref set<int> _NewStoryGroupLinkedClothes;

	override void InitItemVariables()
	{
		super.InitItemVariables();

		string itemTypeName = GetType();
		itemTypeName.ToLower();

		NewStoryGroupInitItemVariables(itemTypeName);
	}

	protected void NewStoryGroupInitItemVariables(string itemType);

	protected void AddNewStoryGroupLinkedClothesSlot(int slost)
	{
		if ( !_NewStoryGroupLinkedClothes ) {
			_NewStoryGroupLinkedClothes = new set<int>();
		}

		_NewStoryGroupLinkedClothes.Insert( slost );
	}

	set<int> GetNewStoryGroupLinkedClothesSlots()
	{
		return _NewStoryGroupLinkedClothes;
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );

		if ( m_OnNSgHealthLevelChanged ) {
			m_OnNSgHealthLevelChanged.Invoke( this, oldLevel, newLevel, zone );
		}
	}

	override void OnDamageDestroyed(int oldLevel)
	{
		super.OnDamageDestroyed( oldLevel );

		if ( m_OnNSgDamageDestroyed ) {
			m_OnNSgDamageDestroyed.Invoke( this, oldLevel );
		}
	}

	override void OnQuantityChanged(float delta)
	{
		super.OnQuantityChanged( delta );

		if ( m_OnNSgQuantityChanged ) {
			m_OnNSgQuantityChanged.Invoke( this, delta );
		}
	}
}
