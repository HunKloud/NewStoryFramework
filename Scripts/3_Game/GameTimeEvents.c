class NewStoryGroupGameTimeEvents
{
    // (bool isDay)
    private ref ScriptInvoker _OnGameTimeChange;
    
    private ref NewStoryGroupTimeData _GameTime;

    private bool    _GameTimeIsDay;
    private float   _GameTimeBuffer;

    void NewStoryGroupGameTimeEvents()
    {
        _GameTime = new NewStoryGroupTimeData();
        _GameTime.SetGameTime();

        _GameTimeIsDay      = GameTimeIsDay();
        _GameTimeBuffer     = 0;

		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert( OnUpdate );
    }

    void ~NewStoryGroupGameTimeEvents()
    {
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove( OnUpdate );
    }

    void OnUpdate(float dTime)
    {
        OnUpdateGameTime(dTime);
    }

    private void OnUpdateGameTime(float dTime)
    {
        _GameTimeBuffer += dTime;

        if ( _GameTimeBuffer >= 60 ) {
            _GameTime.SetGameTime();

            bool isDay = GameTimeIsDay();

            if ( _GameTimeIsDay != isDay ) {
                _GameTimeIsDay = isDay;

                if ( _OnGameTimeChange ) {
                    _OnGameTimeChange.Invoke( _GameTimeIsDay );
                }
            }

            _GameTimeBuffer = 0;
        }
    }

    bool GameTimeIsDay()
    {
        return _GameTime.Hour >= 7 && _GameTime.Hour <= 19;
    }

    ScriptInvoker Event_OnGameTimeChange()
    {
        if ( !_OnGameTimeChange ) {
            _OnGameTimeChange = new ScriptInvoker();
        }

        return _OnGameTimeChange;
    }
}
