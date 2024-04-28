modded class MissionBase: MissionBaseWorld
{
    override protected void NewStoryGroupConstructor()
    {
        bool isMissionServer    = IsNewStoryGroupMissionServer();
        bool isMissionGameplay  = IsNewStoryGroupMissionGameplay();

        if ( isMissionServer || isMissionGameplay ) {
            NewStoryGroup().OnPreCreateMission();
            NewStoryGroup().OnCreateMission();
            NewStoryGroup().OnAfterCreateMission();
        }

        if ( isMissionGameplay ) {
            NewStoryGroup().OnPreCreateMissionGameplay();

            NewStoryGroupPlayerManagerClient playerManagerClient = new NewStoryGroupPlayerManagerClient();
            playerManagerClient.LoadModules();
            NewStoryGroup().SetPlayerManagerClient( playerManagerClient );

            NewStoryGroup().OnCreateMissionGameplay();
            NewStoryGroup().OnAfterCreateMissionGameplay();
        }

        if ( isMissionServer ) {
            NewStoryGroup().OnPreCreateMissionServer();
            NewStoryGroup().SetPlayerManagers( new NewStoryGroupPlayerManagersServer() );
            NewStoryGroup().OnCreateMissionServer();
            NewStoryGroup().OnAfterCreateMissionServer();
        }

        super.NewStoryGroupConstructor();
    }

    void ~MissionBase()
    {
        bool isMissionServer    = IsNewStoryGroupMissionServer();
        bool isMissionGameplay  = IsNewStoryGroupMissionGameplay();

        if ( isMissionServer || isMissionGameplay ) {
            NewStoryGroup().OnPreDestroyMission();
            NewStoryGroup().OnDestroyMission();
            NewStoryGroup().OnAfterDestroyMission();
        }

        if ( isMissionGameplay ) {
            NewStoryGroup().OnPreDestroyMissionGameplay();
            NewStoryGroup().OnDestroyMissionGameplay();
            NewStoryGroup().OnAfterDestroyMissionGameplay();
        }

        if ( isMissionServer ) {
            NewStoryGroup().OnPreDestroyMissionServer();
            NewStoryGroup().OnDestroyMissionServer();
            NewStoryGroup().OnAfterDestroyMissionServer();
        }
    }
}
