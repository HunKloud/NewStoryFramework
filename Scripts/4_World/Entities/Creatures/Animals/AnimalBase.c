modded class AnimalBase: DayZAnimal
{
    private ref NewStoryGroupCreatureHitHandler m_NewStoryGroupCreatureHitHandler;
    
    #ifdef SERVER
    void AnimalBase()
    {
        if ( NewStoryGroupCoreSettings.REQUEST_PLAYER_MODULE_KILLS ) {
            m_NewStoryGroupCreatureHitHandler = new NewStoryGroupCreatureHitHandler();
            GetOnHitByInvoker().Insert( m_NewStoryGroupCreatureHitHandler.EEHitBy );
            GetOnKilledInvoker().Insert( m_NewStoryGroupCreatureHitHandler.EEKilled );
        }
    }
    #endif

    NewStoryGroupCreatureHitHandler GetNewStoryGroupCreatureHitHandler()
    {
        return m_NewStoryGroupCreatureHitHandler;
    }
}
