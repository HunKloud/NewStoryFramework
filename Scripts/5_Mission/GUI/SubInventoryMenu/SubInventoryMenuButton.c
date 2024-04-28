class NSgSubInventoryMenuButton: ScriptedWidgetEventHandler
{
	private InventoryMenu m_InventoryMenu;
	private Widget m_RootWidget;

	private TextWidget m_TitleWidget;

	private ref SizeToChild m_Resizer;

	void NSgSubInventoryMenuButton(InventoryMenu menu, Widget parent)
	{
		m_RootWidget = GetGame().GetWorkspace().CreateWidgets( GetLayoutName(), parent );
		m_InventoryMenu = menu;

		OnInit();

		m_RootWidget.SetHandler( this );
	}

	private void OnInit()
	{
		m_TitleWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget( "title" ) );
		m_RootWidget.GetScript( m_Resizer );
	}

	void SetTitleText(string title)
	{
		m_TitleWidget.SetText( title );

		if ( m_Resizer ) {
			m_Resizer.ResizeParentToChild();
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_RootWidget ) {
			m_TitleWidget.SetColor( 0xFFFFFFFF );
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( w != m_RootWidget ) {
			return false;
		}

		if ( ( w.GetFlags() & WidgetFlags.IGNOREPOINTER ) != WidgetFlags.IGNOREPOINTER ) {
			m_TitleWidget.SetColor( 0xFFE6E6E6 );
		}

		return true;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		m_RootWidget.SetFlags( WidgetFlags.IGNOREPOINTER );
		OpenSubMenu();

		return true;
	}

	void OpenSubMenu()
	{
		m_InventoryMenu.obfm_OnSelectSubMenu( this );
	}

	void OnSelect()
	{
		m_RootWidget.SetFlags( WidgetFlags.IGNOREPOINTER );
		m_TitleWidget.SetColor( 0xFFFFFFFF );
	}

	void OnUnselect()
	{
		m_RootWidget.ClearFlags( WidgetFlags.IGNOREPOINTER );
		m_TitleWidget.SetColor( 0xFFE6E6E6 );
	}

	void SetInventoryData()
	{
		SetTitleText( "#STR_Controls_Inventory" );
	}

	private string GetLayoutName()
	{
		return "NewStoryFramework/layouts/sub_inv_button.layout";
	}
}
