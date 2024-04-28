class NewStoryGroupFlowingEntityWidgetBase: NewStoryGroupFlowingWidgetBase
{
	protected EntityAI _Entity;

	void SetEntity(EntityAI entity)
	{
		_Entity = entity;
	}

	override protected bool CanShow()
	{
		return super.CanShow() && _Entity != null;
	}

	protected vector GetPosition()
	{
		return _Entity.GetPosition();
	}

	override protected float GetDistance()
	{
		vector pos1 = GetGame().GetPlayer().GetPosition();
		vector pos2 = GetPosition();
		return vector.Distance( pos1, pos2 );
	}

	// Возвращает расстояние на которое надо сместить виджет над позицией предмета
	//  если "предметом" является игрок, то смещение будет работать только, если он мертв!
	protected vector GetWidgetOffset()
	{
		return "0 0.5 0";
	}

	override vector GetWorldPosition()
	{
		// return _Player.ModelToWorld( "0 0.5 0" );
		if ( !_Entity.IsAlive() || !_Entity.IsInherited( Human ) ) {
			return _Entity.GetPosition() + GetWidgetOffset();
		}

		vector position;
		Human human = Human.Cast( _Entity );

		int headBoneIndx = human.GetBoneIndexByName("Head");

		if ( headBoneIndx == -1 ) {
			position = human.GetPosition();
			position[1] = position[1] + 1.85;
		}
		else {
			position = human.GetBonePositionWS(headBoneIndx);
			position[1] = position[1] + 0.25;
		}
		
		return position;
	}
}
