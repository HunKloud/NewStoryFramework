modded class PlayerIdentity: PlayerIdentityBase
{
    private INewStoryGroupPlayerManagerBase m_NewStoryGroupPlayerManagerServer;

    INewStoryGroupPlayerManagerBase GetNewStoryGroupPlayerManagerServer()
    {
        return m_NewStoryGroupPlayerManagerServer;
    }

    void SetNewStoryGroupPlayerManagerServer(INewStoryGroupPlayerManagerBase manager)
    {
        m_NewStoryGroupPlayerManagerServer = manager;
    }
}
