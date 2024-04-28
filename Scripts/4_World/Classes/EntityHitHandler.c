class NewStoryGroupEntityHitHandlerBase
{
    protected int     m_LastHitDamageType;
    protected string  m_LastHitDmgZone;
    protected string  m_LastHitAmmo;

    int GetLastHitDamageType()
    {
        return m_LastHitDamageType;
    }

    string GetLastHitDmgZone()
    {
        return m_LastHitDmgZone;
    }

    string GetLastHitAmmo()
    {
        return m_LastHitAmmo;
    }

    void EEHitBy(EntityAI entity, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        m_LastHitDamageType  = damageType;
        m_LastHitDmgZone     = dmgZone;
        m_LastHitAmmo        = ammo;
    }

    void EEKilled(EntityAI entity, Object killer);
}

class NewStoryGroupCreatureHitHandler: NewStoryGroupEntityHitHandlerBase
{
    override void EEKilled(EntityAI entity, Object killer)
    {
        EntityAI murderer = NewStoryGroupPlayerModuleKills.GetMurderer( killer );
        PlayerBase murdererPlayer;

        if ( Class.CastTo( murdererPlayer, murderer ) && murdererPlayer.GetIdentity() != null ) {
            auto murdererPlayerManager = murdererPlayer.GetIdentity().GetNewStoryGroupPlayerManagerServer();

            if ( murdererPlayerManager ) {
                NewStoryGroupPlayerModuleKills murdererModuleKills;
                
                if ( Class.CastTo( murdererModuleKills, murdererPlayerManager.GetModule( NewStoryGroupPlayerModuleKills ) ) ) {
                    bool isHeadshot = m_LastHitDmgZone == "Head";
                    murdererModuleKills.OnEntityKilled( entity, EntityAI.Cast( killer ), isHeadshot );
                }
            }
        }
    }
}

class NewStoryGroupPlayerHitHandler: NewStoryGroupEntityHitHandlerBase
{
    private NewStoryGroupPlayerModuleKills GetModuleKills(PlayerBase player)
    {
        auto playerManager = player.GetIdentity().GetNewStoryGroupPlayerManagerServer();
        return NewStoryGroupPlayerModuleKills.Cast( playerManager.GetModule( NewStoryGroupPlayerModuleKills ) );
    }

    override void EEKilled(EntityAI entity, Object killer)
    {
        PlayerBase currentPlayer = PlayerBase.Cast( entity );

        EntityAI entityKiller = EntityAI.Cast( killer );
        EntityAI murderer = NewStoryGroupPlayerModuleKills.GetMurderer( killer );
        PlayerBase murdererPlayer = PlayerBase.Cast( murderer );

        bool isHeadshot = m_LastHitDmgZone == "Head";

        if ( currentPlayer.GetIdentity() != null ) {
            // Умер от объекта
            if ( !murdererPlayer ) {
                if ( !entityKiller ) {
                    return;
                }

                GetModuleKills(currentPlayer).OnDeathFromEntity( entityKiller, isHeadshot );

                // InventoryLocation oldLoc = entityKiller.m_OldLocation;

                // if ( !oldLoc || !oldLoc.GetParent() || !oldLoc.GetParent().GetHierarchyRootPlayer() ) {
                //     GetModuleKills(currentPlayer).OnDeathFromEntity( entityKiller, isHeadshot );
                //     return;
                // }

                // murdererPlayer = PlayerBase.Cast( oldLoc.GetParent().GetHierarchyRootPlayer() );
            }

            // Суицид
            if ( murdererPlayer == currentPlayer ) {
                if ( currentPlayer.CommitedSuicide() ) {
                    GetModuleKills(currentPlayer).OnSuicide( currentPlayer.GetItemInHands() );
                    return;
                }

                GetModuleKills(currentPlayer).OnUnknownDeath();
                return;
            }

            // Умер от NPC
            if ( murdererPlayer.GetIdentity() == null ) {
                GetModuleKills(currentPlayer).OnDeathFromNPC( murdererPlayer, entityKiller, isHeadshot );
                return;
            }

            GetModuleKills(murdererPlayer).OnEntityKilled( currentPlayer, entityKiller, isHeadshot );
            return;
        }
        
        if ( murdererPlayer && murdererPlayer.GetIdentity() != null ) {
            GetModuleKills(murdererPlayer).OnEntityKilled( currentPlayer, entityKiller, isHeadshot );
        }
    }
}
