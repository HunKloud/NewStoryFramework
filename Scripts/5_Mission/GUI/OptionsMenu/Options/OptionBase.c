class NewStoryGroupOptionBase
{
    protected NewStoryGroupOptionBodyBase _ParentBody;

    protected Widget _Root;
    protected Widget _OptionWidget;

    protected string _Label;

    void NewStoryGroupOptionBase(string name)
    {
        _Label = name;
    }

    void Init(NewStoryGroupOptionBodyBase body)
    {
        _ParentBody = body;

        _Root           = GetGame().GetWorkspace().CreateWidgets( "NewStoryFramework/layouts/OptionItem.layout", _ParentBody.GetContent() );
        _OptionWidget   = _Root.FindAnyWidget( "SettingItem_option" );

        TextWidget.Cast( _Root.FindAnyWidget( "SettingItem_label" ) ).SetText( _Label );

        InitOption( _ParentBody.GetRootMenu(), _OptionWidget );
    }

    protected void InitOption(ScriptedWidgetEventHandler rootMenu, Widget optionWidget);

    Widget GetSettingOptionWidget()
    {
        return _OptionWidget;
    }

    protected OptionsMenu GetOptionMenu()
    {
        return OptionsMenu.Cast( GetGame().GetUIManager().GetMenu() );
    }

    void OnChanged()
    {
        OptionsMenu menu = GetOptionMenu();

        if ( menu ) {
            menu.OnChanged();
        }
    }
    
    string GetDetailTitle();
    string GetDetailDescription();
    string GetDetailCustomLayout();

    bool EnableOption(int key);
    bool DisableOption(int key);

    void Apply();
    bool IsChanged();
    void Revert();
    void SetToDefaults();
}
