modded class InventoryMenu: UIScriptedMenu
{
	private Widget obfv_m_SubButtonsRoot;
	private Widget obfv_m_SubInventoryBackground;

	private ref NSgSubInventoryMenuButton obfv_m_SubButtonInventory;

	private ref map<ref NSgSubInventoryMenuButton, ref NSgSubInventoryMenuBase> obfv_m_SubInventoryMenus;

	private NSgSubInventoryMenuButton obfv_m_SubButtonCurrent;

	private ref array<ref Param2<int, string>> obfv_m_RegisteredSubMenus;

	void InventoryMenu()
	{
		if ( !NewStoryGroupCoreSettings.REQUEST_INVENTORY_SUB_MENU ) {
			return;
		}

		obfv_m_SubButtonsRoot = GetGame().GetWorkspace().CreateWidgets( GetNSgSubInventoryButtonsRootName() );
		obfv_m_SubInventoryBackground = GetGame().GetWorkspace().CreateWidgets( GetNSgSubInventoryBackgroundName() );
		obfv_m_SubInventoryBackground.Show( false );

		obfv_m_SubInventoryMenus = new map<ref NSgSubInventoryMenuButton, ref NSgSubInventoryMenuBase>();

		obfv_m_SubButtonInventory = new NSgSubInventoryMenuButton( this, obfv_m_SubButtonsRoot );
		obfv_m_SubButtonInventory.SetInventoryData();
		obfv_m_SubButtonInventory.OnSelect();

		obfv_m_SubButtonCurrent = obfv_m_SubButtonInventory;

		obfv_m_RegisteredSubMenus = new array<ref Param2<int, string>>();

		RegisterNSgSubMenu();

		// Похуй
		for ( int i = 0; i < obfv_m_RegisteredSubMenus.Count() - 1; i++ ) {
			int obfv_maxIndex = i;

			for ( int j = i + 1; obfv_m_RegisteredSubMenus.Count(); j++ ) {
				if ( obfv_m_RegisteredSubMenus[j].param1 > obfv_m_RegisteredSubMenus[obfv_maxIndex].param1 ) {
					obfv_maxIndex = j
				}
			}

			if ( obfv_maxIndex != i ) {
				obfv_m_RegisteredSubMenus.SwapItems( i, obfv_maxIndex );
			}
		}

		foreach( auto obfv_prams: obfv_m_RegisteredSubMenus ) {
			typename obfv_type = obfv_prams.param2.ToType();

			if ( !obfv_type ) {
				continue;
			}

			NSgSubInventoryMenuButton obfv_button = new NSgSubInventoryMenuButton( this, obfv_m_SubButtonsRoot );

			NSgSubInventoryMenuBase obfv_menu = NSgSubInventoryMenuBase.Cast( obfv_type.Spawn() );
			obfv_menu.Init( obfv_m_SubInventoryBackground );
			obfv_menu.OnCreateRootButton( obfv_button );

			obfv_m_SubInventoryMenus.Insert( obfv_button, obfv_menu );
		}

		obfv_m_RegisteredSubMenus = null;

		if ( obfv_m_SubInventoryMenus.Count() == 0 ) {
			obfv_m_SubButtonsRoot.Show( false );
		}
	}

	private void RegisterNSgSubMenu();
	
	// Порядок кнопок зависит от приоритета, чем он выше тем левее он будет
	private void AddNSgSubMenu(int priority, string menu)
	{
		obfv_m_RegisteredSubMenus.Insert( new Param2<int, string>( priority, menu ) );
	}

	private void RemoveNSgSubMenu(string menu)
	{
		for ( int i = 0; i < obfv_m_RegisteredSubMenus.Count(); i++ ) {
			auto obfv_data = obfv_m_RegisteredSubMenus[i];

			if ( obfv_data.param2 == menu ) {
				obfv_m_RegisteredSubMenus.RemoveOrdered(i);
				return;
			}
		}
	}

	override void OnShow()
	{
		super.OnShow();

		if ( !NewStoryGroupCoreSettings.REQUEST_INVENTORY_SUB_MENU ) {
			return;
		}

		obfv_m_SubButtonsRoot.Show( true );

		if ( obfv_m_SubButtonCurrent == obfv_m_SubButtonInventory ) {
			return;
		}

		layoutRoot.Show( false );
		obfv_m_SubInventoryBackground.Show( true );

		Mission obfv_mission = GetGame().GetMission();

		if ( obfv_mission && obfv_mission.GetHud() && obfv_mission.GetHud().IsInherited( IngameHud ) ) {
			IngameHud.Cast( obfv_mission.GetHud() ).Show( false );
		}
	}

	override void OnHide()
	{
		super.OnHide();

		if ( !NewStoryGroupCoreSettings.REQUEST_INVENTORY_SUB_MENU ) {
			return;
		}

		obfv_m_SubButtonsRoot.Show( false );
		obfv_m_SubInventoryBackground.Show( false );

		Mission obfv_mission = GetGame().GetMission();

		if ( obfv_mission && obfv_mission.GetHud() && obfv_mission.GetHud().IsInherited( IngameHud ) ) {
			IngameHud.Cast( obfv_mission.GetHud() ).Show( true );
		}
	}

	void obfm_OnSelectSubMenu(NSgSubInventoryMenuButton obfv_button)
	{
		obfm_HideSubMenu( obfv_m_SubButtonCurrent );
		obfm_ShowSubMenu( obfv_button );

		obfv_m_SubButtonCurrent.OnUnselect();
		obfv_m_SubButtonCurrent = obfv_button;
	}

	private void obfm_ShowSubMenu(NSgSubInventoryMenuButton obfv_button)
	{
		if ( obfv_button != obfv_m_SubButtonInventory ) {
			obfv_m_SubInventoryMenus[obfv_button].Show();
			return;
		}

		layoutRoot.Show( true );
		obfv_m_SubInventoryBackground.Show( false );

		Mission obfv_mission = GetGame().GetMission();

		if ( obfv_mission && obfv_mission.GetHud() && obfv_mission.GetHud().IsInherited( IngameHud ) ) {
			IngameHud.Cast( obfv_mission.GetHud() ).Show( true );
		}
	}

	private void obfm_HideSubMenu(NSgSubInventoryMenuButton obfv_button)
	{
		if ( obfv_button != obfv_m_SubButtonInventory ) {
			obfv_m_SubInventoryMenus[obfv_button].Hide();
			return;
		}

		layoutRoot.Show( false );
		obfv_m_SubInventoryBackground.Show( true );

		Mission obfv_mission = GetGame().GetMission();

		if ( obfv_mission && obfv_mission.GetHud() && obfv_mission.GetHud().IsInherited( IngameHud ) ) {
			IngameHud.Cast( obfv_mission.GetHud() ).Show( false );
		}
	}

	override void Update(float timeslice)
	{
		super.Update( timeslice );

		if ( NewStoryGroupCoreSettings.REQUEST_INVENTORY_SUB_MENU ) {
			foreach ( NSgSubInventoryMenuBase obfv_menu: obfv_m_SubInventoryMenus ) {
				obfv_menu.Update( timeslice );
			}
		}
	}

	private string GetNSgSubInventoryButtonsRootName()
	{
		return "NewStoryFramework/layouts/sub_inventory_buttons.layout";
	}

	private string GetNSgSubInventoryBackgroundName()
	{
		switch ( m_WidthType )
		{
			case ScreenWidthType.WIDE:
				return "NewStoryFramework/layouts/wide/sub_inventory_background.layout";

			case ScreenWidthType.MEDIUM:
				return "NewStoryFramework/layouts/medium/sub_inventory_background.layout";
			
			case ScreenWidthType.NARROW:
				return "NewStoryFramework/layouts/narrow/sub_inventory_background.layout";
		}

		return "NewStoryFramework/layouts/wide/sub_inventory_background.layout";
	}
}
