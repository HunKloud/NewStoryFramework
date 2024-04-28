class NewStoryGroupProfilesOptionFloat: NewStoryGroupProfilesOptionBase
{
    private float _DefValue;
    private float _Value;

    void NewStoryGroupProfilesOptionFloat(float def)
    {
        _DefValue = def;
    }

    override float GetVaribleFloat()
    {
        return _Value;
    }

    override void SetVaribleFloat(float value)
    {
        _Value = value;
    }

    override void SetVaribleString(string value)
    {
        _Value = value.ToFloat();
    }

    override string GetVaribleString()
    {
        return _Value.ToString();
    }

    override void SetToDefault()
    {
        _Value = _DefValue;
    }
}
