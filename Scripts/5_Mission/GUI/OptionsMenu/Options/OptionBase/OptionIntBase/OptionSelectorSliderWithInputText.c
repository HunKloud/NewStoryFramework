class NewStoryGroupOptionSelectorSliderWithInputTextIntBase: NewStoryGroupOptionIntBase
{
    protected ref NewStoryGroupOptionSelectorSliderWithInputText _OptionSelectorSlider;

    override protected void InitOption(ScriptedWidgetEventHandler rootMenu, Widget optionWidget)
    {
        bool disabled;
        float min, max;
        float step;
        string unit;

        LoadOptionSelectorSlider( disabled, min, max, step, unit );
        int value = NewStoryGroup().GetProfilesOptions().GetOptionInt( _OptionIndx );

        _OptionSelectorSlider = new NewStoryGroupOptionSelectorSliderWithInputText( optionWidget, value, rootMenu, disabled, min, max, unit );
        _OptionSelectorSlider.SetStep( step );
        _OptionSelectorSlider.m_OptionChanged.Insert( OnChanged );
    }
    
    protected void LoadOptionSelectorSlider(out bool disabled, out float min, out float max, out float step, out string unit);

    override void OnChanged()
    {
        float value = _OptionSelectorSlider.GetValue();
        int checkValue = Math.Floor( value );
        int def = Math.Floor( checkValue - value );

        _OptionSelectorSlider.SetValue( checkValue, false );

        if ( def != 0 ) {
            super.OnChanged();
        }
    }

    override protected int GetValue()
    {
        return _OptionSelectorSlider.GetValue();
    }

    override protected void SetValue(int value)
    {
        _OptionSelectorSlider.SetValue( value, false );
    }
}
