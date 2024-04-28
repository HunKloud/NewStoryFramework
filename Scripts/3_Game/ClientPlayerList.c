class NewStoryGroupClientPlayerList
{
	// Вызывается когда создается 'PlayerBase' с 'PlayerIdentity'
	// (Man player)
	private ref ScriptInvoker _OnAdd;
	// Вызывается когда уничтожается'PlayerBase' с 'PlayerIdentity'
	// (Man player)
	private ref ScriptInvoker _OnRemove;

	private ref array<Man> _List = new array<Man>();
	private ref array<Man> _ListWithIdentity = new array<Man>();

	private int _Indx = -1;

	void NewStoryGroupClientPlayerList()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( Update );
	}

	void AddPlayer(Man player)
	{
		PlayerIdentity identity = player.GetIdentity();

		if ( identity == null ) {
			_List.Insert( player );
			return;
		}

		_ListWithIdentity.Insert( player );

		if ( _OnAdd ) {
			_OnAdd.Invoke( player );
		}
	}

	void RemovePlayer(Man player)
	{
		int indx = _ListWithIdentity.Find( player );

		if ( indx != -1 ) {
			_ListWithIdentity.Remove( indx );

			if ( _OnRemove ) {
				_OnRemove.Invoke( player );
			}
		}
		else {
			_List.RemoveItemUnOrdered( player );
		}
	}

	void Update(float dTime)
	{
		if ( _List.Count() == 0 ) {
			return;
		}

		_Indx = Math.Clamp( _Indx, 0, _List.Count() - 1 );
		int count = Math.Min( _List.Count(), 5 );

		for ( int i = 0; i < count; i++ ) {
			Man player = _List[_Indx];

			if ( player.GetIdentity() != null ) {
				_List.Remove( _Indx );
				_ListWithIdentity.Insert( player );

				if ( _OnAdd ) {
					_OnAdd.Invoke( player );
				}
			}

			_Indx--;

			if ( _Indx < 0 ) {
				_Indx = _List.Count() - 1;
			}
		}
	}

	array<Man> GetList()
	{
		return _ListWithIdentity;
	}

	Man GetPlayerByUID(string uid)
	{
		for ( int i = 0; i < _ListWithIdentity.Count(); i++ ) {
			if ( _ListWithIdentity[i].GetIdentity().GetId() == uid ) {
				return _ListWithIdentity[i];
			}
		}

		return null;
	}

	ScriptInvoker Event_OnAdd()
	{
		if ( !_OnAdd ) {
			_OnAdd = new ScriptInvoker();
		}

		return _OnAdd;
	}

	ScriptInvoker Event_OnRemove()
	{
		if ( !_OnRemove ) {
			_OnRemove = new ScriptInvoker();
		}

		return _OnRemove;
	}
}
