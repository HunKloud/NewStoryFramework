class NewStoryGroupPlayerManagerClient: NewStoryGroupPlayerManagerBase
{
    override void RPC(int type, array<ref Param> params)
    {
        array<ref Param> sendParams = params;
        Param1<int> paramType = new Param1<int>(type);

        if ( !sendParams ) {
            sendParams = new array<ref Param>();
            sendParams.Insert( paramType );
        }
        else {
            sendParams.InsertAt( paramType, 0 );
        }
        
        GetGame().RPC( null, ENewStoryGroupRPC.PLAYER_MANAGER, sendParams, true );
    }

    override void RPCSingleParam(int type, Param param = null)
    {
        array<ref Param> sendParams = new array<ref Param>();
        sendParams.Insert( new Param1<int>(type) );

        if ( param ) {
            sendParams.Insert( param );
        }

        GetGame().RPC( null, ENewStoryGroupRPC.PLAYER_MANAGER, sendParams, true );
    }

    override PlayerIdentity GetPlayerIdentity()
    {
        return GetGame().GetPlayer().GetIdentity();
    }
}
