modded class PluginTransmissionAgents: PluginBase
{
    float GetNewStoryGroupProtectionLevel(int type, int slot, Man player, bool consider_filter = true, int system = 0)
    {
        ItemBase attachment = ItemBase.Cast( player.GetInventory().FindAttachment(slot) );
        
        if ( !attachment ) {
            PlayerBase playerBase = PlayerBase.Cast( player );
            attachment = playerBase.GetNewStoryGroupLinkedClothes(slot);
        }

        if ( attachment ) {
            return attachment.GetProtectionLevel( type, consider_filter, system );
        }
		
		return 0;
    }

    override protected float GetProtectionLevel(int type, int slot, Man player)
	{
        ItemBase attachment = ItemBase.Cast( player.GetInventory().FindAttachment(slot) );
        
        if ( !attachment ) {
            PlayerBase playerBase = PlayerBase.Cast( player );
            attachment = playerBase.GetNewStoryGroupLinkedClothes(slot);
        }

        if ( attachment ) {
            return attachment.GetProtectionLevel( type, true, 0 );
        }
		
		return super.GetProtectionLevel( type, slot, player );
	}
}
