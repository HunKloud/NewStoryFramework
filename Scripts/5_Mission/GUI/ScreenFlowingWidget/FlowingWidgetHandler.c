class NewStoryGroupFlowingWidgetHandler
{
    private static NewStoryGroupFlowingWidgetHandler _Instance;

    private ref array<NewStoryGroupFlowingWidgetBase> _Widgets;

    private bool _IsVisible         = true;
    private bool _IsVisibleLocal    = true;

    void NewStoryGroupFlowingWidgetHandler()
    {
        _Instance   = this;
        _Widgets    = new array<NewStoryGroupFlowingWidgetBase>();

        GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert( UpdateGUI );
    }

    void ~NewStoryGroupFlowingWidgetHandler()
    {
        if ( GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM) ) {
            GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove( UpdateGUI );
        }
    }

    static NewStoryGroupFlowingWidgetHandler GetInstance()
    {
        return _Instance;
    }

    void AddFlowingWidget(NewStoryGroupFlowingWidgetBase widget)
    {
        widget.Show( _IsVisible && _IsVisibleLocal );
        _Widgets.Insert( widget );
    }

    void RemoveFlowingWidget(NewStoryGroupFlowingWidgetBase widget)
    {
        _Widgets.RemoveItemUnOrdered( widget );
    }

    void ShowIngameHud(bool value)
    {
        for ( int i = _Widgets.Count() - 1; i >= 0; i-- ) {
            NewStoryGroupFlowingWidgetBase widget = _Widgets[i];

            if ( !widget ) {
                _Widgets.Remove(i);
                continue;
            }

            if ( widget.DependsOnHUD() ) {
                widget.HudIsVisible( value );
            }
        }
    }

    void Show(bool value)
    {
        _IsVisible = value;

        for ( int i = _Widgets.Count() - 1; i >= 0; i-- ) {
            NewStoryGroupFlowingWidgetBase widget = _Widgets[i];

            if ( !widget ) {
                _Widgets.Remove(i);
                continue;
            }

            widget.Show( value );
        }
    }

    private void ShowLocal(bool value)
    {
        _IsVisibleLocal = value;

        for ( int i = _Widgets.Count() - 1; i >= 0; i-- ) {
            NewStoryGroupFlowingWidgetBase widget = _Widgets[i];

            if ( !widget ) {
                _Widgets.Remove(i);
                continue;
            }

            widget.Show( value );
            widget.Update();
        }
    }

    bool IsVisible()
    {
        return _IsVisible;
    }

    void UpdateGame(float dTime)
    {
        if ( !_IsVisible ) {
            return;
        }

        PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
        bool canShow = player && player.IsAlive() && !player.IsUnconscious();

        if ( _IsVisibleLocal != canShow ) {
            ShowLocal( canShow );
        }
    }

    void UpdateGUI(float dTime)
    {
        if ( !_IsVisible || !_IsVisibleLocal ) {
            return;
        }

        for ( int i = _Widgets.Count() - 1; i >= 0; i-- ) {
            NewStoryGroupFlowingWidgetBase widget = _Widgets[i];

            if ( !widget ) {
                _Widgets.Remove(i);
                continue;
            }

            widget.Update();
        }
    }
}
