class NewStoryGroupProfilesOptionBool: NewStoryGroupProfilesOptionBase
{
    private bool _DefValue;
    private bool _Value;

    void NewStoryGroupProfilesOptionBool(bool def)
    {
        _DefValue = def;
    }

    override bool GetVaribleBool()
    {
        return _Value;
    }

    override void SetVaribleBool(bool value)
    {
        _Value = value;
    }

    override void SetVaribleString(string value)
    {
        if ( value == "1" || value == "true" ) {
            _Value = true;
        }
        else if ( value == "0" || value == "false" ) {
            _Value = false;
        }
    }

    override string GetVaribleString()
    {
        if ( _Value ) {
            return "1";
        }

        return "0";
    }

    override void SetToDefault()
    {
        _Value = _DefValue;
    }
}
