class NewStoryGroupWarningTextWidget: ScriptedWidgetEventHandler
{
    private Widget              m_RootWidget;
    private TextWidget          m_TextWidget;
    private Widget              m_ButtonWidget;

    private ref SizeToChild     m_Resizer;
    private ScriptCaller        m_Callback;

    void NewStoryGroupWarningTextWidget(string text, notnull ScriptCaller callback, Widget parent = null)
    {
        m_RootWidget     = GetGame().GetWorkspace().CreateWidgets( "NewStoryGroup/NewStoryFramework/layouts/Warnings/WarningText.layout", parent );
        m_TextWidget     = TextWidget.Cast( m_RootWidget.FindAnyWidget( "Content" ) );
        m_ButtonWidget   = m_RootWidget.FindAnyWidget( "Button" );

        m_RootWidget.GetChildren().GetScript( m_Resizer );
        m_Callback = callback;

        SetText(text);

        m_RootWidget.SetHandler(this);
    }

    void ~NewStoryGroupWarningTextWidget()
    {
        if ( m_RootWidget ) {
            m_RootWidget.Unlink();
        }
    }

    void SetText(string text)
    {
        m_TextWidget.SetText( text );
        m_Resizer.ResizeParentToChild();
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if ( w == m_ButtonWidget ) {
            m_Callback.Invoke();
            return true;
        }

        return false;
    }
}
