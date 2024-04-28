class NewStoryGroupOptionSelectorSliderWithInputText: OptionSelectorSliderSetup
{
	private EditBoxWidget   _InputText;
	private TextWidget	  _TextInfo;

	private string _PostfixText;

	void NewStoryGroupOptionSelectorSliderWithInputText(Widget parent, float value, ScriptedWidgetEventHandler parent_menu, bool disabled, float min, float max, string postfix = "")
	{
		m_Root					= GetGame().GetWorkspace().CreateWidgets( "NewStoryFramework/layouts/OptionSliderWithText.layout", parent );
		#ifdef PLATFORM_CONSOLE
		m_Parent				= parent.GetParent().GetParent();
		#else
		#ifdef PLATFORM_WINDOWS
		m_Parent				= parent.GetParent();
		#endif
		#endif
		
		m_SelectorType  = 1;
		m_ParentClass   = parent_menu;

		m_Slider = SliderWidget.Cast( m_Root.FindAnyWidget("Value") );
		m_Slider.SetCurrent(value);

		_InputText  = EditBoxWidget.Cast( m_Root.FindAnyWidget("InputText") );
		_TextInfo   = TextWidget.Cast( m_Root.FindAnyWidget("TextInfo") );
		
		m_MinValue  = min;
		m_MaxValue  = max;

		if ( postfix != "" ) {
			_PostfixText = " " + postfix;
		}
		
		SetValue(value);
		Enable();
		
		m_Parent.SetHandler(this);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == _InputText ) {
			OnChangeInputText(finished);
			return true;
		}

		if ( super.OnChange( w, x, y, finished ) ) {
			SetInfoText( GetValue().ToString() );
			return true;
		}

		return false;
	}

	protected void OnChangeInputText(bool finished)
	{
		string text = _InputText.GetText();
		bool changed = NewStoryGroupStringFunctuon.ValidValueCheck( text, m_MinValue < 0 );

		if ( finished ) {
			m_Slider.Show(true);
			_TextInfo.Show(true);
			_InputText.Show(false);
			float value = Math.Clamp( text.ToFloat(), m_MinValue, m_MaxValue );

			if ( value != GetValue() ) {
				SetValue( value );
			}
		}
		else if ( changed ) {
			_InputText.SetText( text );
		}
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if ( w == m_Parent || w == m_Slider ) {
			int currentValue = Math.Ceil( GetValue() );
			string current = currentValue.ToString();

			m_Slider.Show(false);
			_TextInfo.Show(false);
			_InputText.Show(true);
			_InputText.SetText( "" );
			return true;
		}
		
		return false;
	}

	override void SetValue(float value, bool update = true)
	{
		super.SetValue( value, update );
		SetInfoText( GetValue().ToString() );
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == _InputText ) {
			return true;
		}

		if ( !super.OnMouseEnter( w, x, y ) ) {
			return false;
		}

		_TextInfo.Show( true );
		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( enterW == _InputText ) {
			return true;
		}

		if ( !super.OnMouseLeave( w, enterW, x, y ) ) {
			return false;
		}

		if ( _InputText.IsVisible() ) {
			OnChangeInputText( true );
		}
		
		_TextInfo.Show( false );
		return true;
	}

	protected void SetInfoText(string text)
	{
		_TextInfo.SetText( text + _PostfixText );
	}
}
