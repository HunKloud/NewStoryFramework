class NewStoryGroupProfilesOptionBase
{
	private string _Name;

	void SetName(string name)
	{
		_Name = name;
	}

	string GetName()
	{
		return _Name;
	}

	bool GetVaribleBool();
	void SetVaribleBool(bool value);

	int GetVaribleInt();
	void SetVaribleInt(int value);

	float GetVaribleFloat();
	void SetVaribleFloat(float value);

	void SetVaribleString(string value);
	string GetVaribleString();
	void SetToDefault();
}
