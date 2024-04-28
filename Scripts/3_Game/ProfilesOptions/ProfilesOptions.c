// По сути аналог 'DayZProfilesOptions', но т.к. там мы можем поставить только значения 'true' или 'false', он нам не подходит
class NewStoryGroupProfilesOptions
{
	private ref map<ENewStoryGroupVariblesType, ref map<int, ref NewStoryGroupProfilesOptionBase>> _Options;

	private string GetOptionName(string optionName, ENewStoryGroupVariblesType varible)
	{
		string varibleType = NewStoryGroupMiscFunctuon.GetVaribleType( varible );
		return optionName + "_" + varibleType;
	}

	private void RegisterOption(ENewStoryGroupVariblesType varible, int optionId, string optionName, NewStoryGroupProfilesOptionBase option)
	{
		if ( !_Options ) {
			_Options = new map<ENewStoryGroupVariblesType, ref map<int, ref NewStoryGroupProfilesOptionBase>>();
		}

		map<int, ref NewStoryGroupProfilesOptionBase> options;

		if ( _Options.Find( varible, options ) && options.Contains( optionId ) ) {
			string varibleType = NewStoryGroupMiscFunctuon.GetVaribleType( varible );
			ErrorEx( string.Format( "Profile option for type = %1 with name = %2 uses busy id = %3", varibleType, optionName, optionId ), ErrorExSeverity.WARNING );
			return;
		}

		if ( !options ) {
			options = new map<int, ref NewStoryGroupProfilesOptionBase>();
			_Options.Insert( varible, options );
		}

		string saveOptionName = GetOptionName( optionName, varible );

		option.SetName( saveOptionName );
		
		string saveValue;
		if ( GetGame().GetProfileString( saveOptionName, saveValue ) ) {
			option.SetVaribleString( saveValue );
		}
		else {
			option.SetToDefault();
			saveValue = option.GetVaribleString();

			GetGame().SetProfileString( saveOptionName, saveValue );
			GetGame().SaveProfile();
		}
		
		options.Insert( optionId, option );
	}

	private NewStoryGroupProfilesOptionBase GetOption(ENewStoryGroupVariblesType varible, int optionId)
	{
		map<int, ref NewStoryGroupProfilesOptionBase> options;

		if ( _Options && _Options.Find( varible, options ) ) {
			return options.Get( optionId );
		}

		return null;
	}

	private void SaveOption(NewStoryGroupProfilesOptionBase option)
	{
		string optionName   = option.GetName();
		string optionValue  = option.GetVaribleString();

		GetGame().SetProfileString( optionName, optionValue );
		GetGame().SaveProfile();
	}

	// Register option
	void RegisterOptionBool(int optionId, string optionName, bool def = true)
	{
		NewStoryGroupProfilesOptionBool option = new NewStoryGroupProfilesOptionBool( def );
		RegisterOption( ENewStoryGroupVariblesType.BOOL, optionId, optionName, option );
	}

	void RegisterOptionInt(int optionId, string optionName, int def = 0)
	{
		NewStoryGroupProfilesOptionInt option = new NewStoryGroupProfilesOptionInt( def );
		RegisterOption( ENewStoryGroupVariblesType.INT, optionId, optionName, option );
	}

	void RegisterOptionFloat(int optionId, string optionName, float def = 0)
	{
		NewStoryGroupProfilesOptionFloat option = new NewStoryGroupProfilesOptionFloat( def );
		RegisterOption( ENewStoryGroupVariblesType.FLOAT, optionId, optionName, option );
	}

	// Set option
	void SetOptionBool(int optionId, bool value)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.BOOL, optionId );

		if ( option ) {
			option.SetVaribleBool( value );
			SaveOption( option );
		}
	}

	void SetOptionInt(int optionId, int value)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.INT, optionId );

		if ( option ) {
			option.SetVaribleInt( value );
			SaveOption( option );
		}
	}

	void SetOptionFloat(int optionId, float value)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.FLOAT, optionId );

		if ( option ) {
			option.SetVaribleFloat( value );
			SaveOption( option );
		}
	}

	// Get option
	bool GetOptionBool(int optionId)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.BOOL, optionId );
		return option && option.GetVaribleBool();
	}

	int GetOptionInt(int optionId)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.INT, optionId );

		if ( option ) {
			return option.GetVaribleInt();
		}

		return 0;
	}

	float GetOptionFloat(int optionId)
	{
		NewStoryGroupProfilesOptionBase option = GetOption( ENewStoryGroupVariblesType.FLOAT, optionId );

		if ( option ) {
			return option.GetVaribleFloat();
		}

		return 0;
	}

	// Reset
	void Reset()
	{
		if ( !_Options ) {
			return;
		}

		string optionName;
		string optionValue;

		foreach( ref map<int, ref NewStoryGroupProfilesOptionBase> list: _Options ) {
			foreach( NewStoryGroupProfilesOptionBase option: list ) {
				option.SetToDefault();

				optionName   = option.GetName();
				optionValue  = option.GetVaribleString();

				GetGame().SetProfileString( optionName, optionValue );
			}
		}

		GetGame().SaveProfile();
	}
}
