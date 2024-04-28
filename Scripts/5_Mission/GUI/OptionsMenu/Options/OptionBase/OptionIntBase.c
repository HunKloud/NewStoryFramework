class NewStoryGroupOptionIntBase: NewStoryGroupOptionBase
{
    protected int _OptionIndx;

    protected int GetValue();
    protected void SetValue(int value);

    override void Apply()
    {
        int value = GetValue();
        NewStoryGroup().GetProfilesOptions().SetOptionInt( _OptionIndx, value );
    }

    override bool IsChanged()
    {
        int value   = GetValue();
        int current = NewStoryGroup().GetProfilesOptions().GetOptionInt( _OptionIndx );

        return value != current;
    }

    override void Revert()
    {
        int current = NewStoryGroup().GetProfilesOptions().GetOptionInt( _OptionIndx );
        SetValue( current );
    }

    override void SetToDefaults()
    {
        int current = NewStoryGroup().GetProfilesOptions().GetOptionInt( _OptionIndx );
        SetValue( current );
    }
}
