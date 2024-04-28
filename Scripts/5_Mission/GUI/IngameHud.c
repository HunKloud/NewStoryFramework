modded class IngameHud: Hud
{
	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if ( NewStoryGroupCoreSettings.REQUEST_FLOWING_WIDGET_HANDLER ) {
			NewStoryGroupFlowingWidgetHandler.GetInstance().ShowIngameHud( m_IsHudVisible && !m_HudInventory );
		}
	}
}
