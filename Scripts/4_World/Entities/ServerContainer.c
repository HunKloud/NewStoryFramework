class NewStoryGroupServerContainer: InventoryItem
{
	override bool IsInventoryItem()
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override bool CanObstruct()
	{
		return false;
	}
	
	override bool IsHealthVisible()
	{
		return false;
	}

	protected bool RequestDestroyAfterStoreLoad()
	{
		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if ( RequestDestroyAfterStoreLoad() ) {
			Delete();
		}	
	}
}
