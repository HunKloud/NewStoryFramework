class NewStoryGroupProfilesOptionInt: NewStoryGroupProfilesOptionBase
{
	private int _DefValue;
	private int _Value;

	void NewStoryGroupProfilesOptionInt(int def)
	{
		_DefValue = def;
	}

	override int GetVaribleInt()
	{
		return _Value;
	}

	override void SetVaribleInt(int value)
	{
		_Value = value;
	}

	override void SetVaribleString(string value)
	{
		_Value = value.ToInt();
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
