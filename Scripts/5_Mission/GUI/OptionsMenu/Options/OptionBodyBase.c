class NewStoryGroupOptionBodyBase
{
	protected Widget _Root;
	protected Widget _Content;
	protected Widget _DetailsRoot;

	protected ScriptedWidgetEventHandler _RootMenu;

	protected ref array<ref NewStoryGroupOptionBase>				_Options;
	// (Param3( header, detail, custom_details_widget ))
	protected ref map<Widget, ref Param3<string, string, Widget>>   _DetailsList;

	protected string _Header;

	void NewStoryGroupOptionBodyBase(string header)
	{
		_Header = header;
	}

	void Init(Widget parent, Widget details_root, ScriptedWidgetEventHandler rootMenu)
	{
		_Root	   = GetGame().GetWorkspace().CreateWidgets( "NewStoryFramework/layouts/OptionBody.layout", parent );
		_Content	= _Root.FindAnyWidget( "Content" );

		TextWidget.Cast( _Root.FindAnyWidget( "HeaderText" ) ).SetText( _Header );

		_DetailsRoot	= details_root;
		_RootMenu	   = rootMenu;

		_Options = new array<ref NewStoryGroupOptionBase>();

		LoadOptions();
		LoadOptionsDetails();
	}

	protected void LoadOptions();

	protected void AddOption(NewStoryGroupOptionBase option)
	{
		option.Init( this );
		_Options.Insert( option );
	}

	protected void LoadOptionsDetails()
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			NewStoryGroupOptionBase option = _Options[i];

			Widget settingOption		= option.GetSettingOptionWidget();
			string title				= option.GetDetailTitle();
			string description		  = option.GetDetailDescription();
			string customDetailsLayout  = option.GetDetailCustomLayout();

			AddDetails( settingOption, title, description, customDetailsLayout );
		}
	}

	protected void AddDetails(Widget setting_option, string title, string description, string customDetailsLayout = "")
	{
		if ( title == "" ) {
			return;
		}

		Widget custom;

		if ( customDetailsLayout != "" ) {
			custom = GetGame().GetWorkspace().CreateWidgets( customDetailsLayout, _DetailsRoot );
		}

		if ( description == "" && !custom ) {
			return;
		}

		if ( !_DetailsList ) {
			_DetailsList = new map<Widget, ref Param3<string, string, Widget>>();
		}

		_DetailsList.Insert( setting_option, new Param3<string, string, Widget>( title, description, custom ) );
	}

	bool GetDetails(Widget w, out Param3<string, string, Widget> details)
	{
		return _DetailsList && _DetailsList.Find( w, details );
	}

	void EnableOption(int key)
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			if ( _Options[i].EnableOption(key) ) {
				return;
			}
		}
	}

	void DisableOption(int key)
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			if ( _Options[i].DisableOption(key) ) {
				return;
			}
		}
	}

	void Apply()
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			_Options[i].Apply();
		}
	}

	bool IsChanged()
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			if ( _Options[i].IsChanged() ) {
				return true;
			}
		}

		return false;
	}

	void Revert()
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			_Options[i].Revert();
		}
	}

	void SetToDefaults()
	{
		for ( int i = 0; i < _Options.Count(); i++ ) {
			_Options[i].SetToDefaults();
		}
	}

	ScriptedWidgetEventHandler GetRootMenu()
	{
		return _RootMenu;
	}

	Widget GetContent()
	{
		return _Content;
	}
}
