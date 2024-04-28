modded class OptionsMenuGame: ScriptedWidgetEventHandler
{
    protected Widget            _NewStoryGroupLastCustomDetails;

    protected ref array<ref NewStoryGroupOptionBodyBase> _NewStoryGroupBodyOptions;

    void OptionsMenuGame(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
    {
        LoadNewStoryGroupOptionBody();
    }

    protected void LoadNewStoryGroupOptionBody();

    protected void AddNewStoryGroupOptionBody(NewStoryGroupOptionBodyBase body)
    {
        if ( !_NewStoryGroupBodyOptions ) {
            _NewStoryGroupBodyOptions = new array<ref NewStoryGroupOptionBodyBase>();
        }

        body.Init( m_Root.FindAnyWidget("game_settings_root"), m_DetailsRoot, this );

        _NewStoryGroupBodyOptions.Insert( body );
    }

    override bool OnFocus(Widget w, int x, int y)
    {
        if ( !super.OnFocus( w, x, y ) ) {
            return false;
        }

        if ( !w.IsInherited( SliderWidget ) && _NewStoryGroupBodyOptions && !UpdateNewStoryGroupDetails( w ) ) {
            if ( _NewStoryGroupLastCustomDetails ) {
                _NewStoryGroupLastCustomDetails.Show(false);
            }
        }

        return true;
    }

    protected bool UpdateNewStoryGroupDetails(Widget w)
    {
        Param3<string, string, Widget> details;

        for ( int i = 0; i < _NewStoryGroupBodyOptions.Count(); i++ ) {
            if ( _NewStoryGroupBodyOptions[i].GetDetails( w, details ) ) {
                m_DetailsRoot.Show( true );
                m_DetailsText.Show( details.param2 != "" );

                m_DetailsLabel.SetText( details.param1 );
                m_DetailsText.SetText( details.param2 );

                _NewStoryGroupLastCustomDetails = details.param3;

                if ( _NewStoryGroupLastCustomDetails ) {
                    _NewStoryGroupLastCustomDetails.Show( true );
                }
                
                m_DetailsRoot.Update();
                return true;
            }
        }

        return false;
    }

    override void Apply()
	{
        super.Apply();

        if ( _NewStoryGroupBodyOptions ) {
            for ( int i = 0; i < _NewStoryGroupBodyOptions.Count(); i++ ) {
                _NewStoryGroupBodyOptions[i].Apply();
            }
        }
    }

    override bool IsChanged()
    {
        if ( super.IsChanged() ) {
            return true;
        }

        if ( _NewStoryGroupBodyOptions ) {
            for ( int i = 0; i < _NewStoryGroupBodyOptions.Count(); i++ ) {
                if ( _NewStoryGroupBodyOptions[i].IsChanged() ) {
                    return true;
                }
            }
        }

        return false;
    }

    override void Revert()
    {
        super.Revert();

        if ( _NewStoryGroupBodyOptions ) {
            for ( int i = 0; i < _NewStoryGroupBodyOptions.Count(); i++ ) {
                _NewStoryGroupBodyOptions[i].Revert();
            }
        }
    }

    override void SetToDefaults()
    {
        super.SetToDefaults();

        NewStoryGroup().GetProfilesOptions().Reset();

        if ( _NewStoryGroupBodyOptions ) {
            for ( int i = 0; i < _NewStoryGroupBodyOptions.Count(); i++ ) {
                _NewStoryGroupBodyOptions[i].SetToDefaults();
            }
        }
    }
}
