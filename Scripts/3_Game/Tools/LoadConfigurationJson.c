class NewStoryGroupLoadConfigurationJson<Class T>: Managed
{
	private ref map<string, ref T> _Settings;

	void NewStoryGroupLoadConfigurationJson(string folder)
	{
		_Settings = new map<string, ref T>();
		array<string> files = NewStoryGroupSystemFunction.FindFilesOnDirectory( folder + "*.json" );

		if ( files && files.Count() > 0 ) {
			foreach(string file: files) {
				LoadFile(folder, file);
			}

			AfterLoadFiles();
		}
	}

	private void LoadFile(string folder, string file_name)
	{
		T config;
		JsonFileLoader<ref T>.JsonLoadFile( folder + file_name, config );

		if ( !config ) {
			return;
		}
	
		file_name = file_name.Substring( 0, file_name.Length() - 5 );
		file_name.ToLower();

		_Settings.Insert( file_name, config );
		OnLoadFile( file_name, config );
	}

	private void OnLoadFile(string file_name, T config);
	private void AfterLoadFiles();

	array<string> GetFileNames()
	{
		return _Settings.GetKeyArray();
	}

	string GetFileName(T config)
	{
		return _Settings.GetKeyByValue(config);
	}

	T GetFileSettings(string file_name)
	{
		file_name.ToLower();
		return _Settings.Get(file_name);
	}

	array<ref T> GetFileSettings(array<string> names)
	{
		array<ref T> configs = new array<ref T>();

		foreach( string name: names ) {
			T config = GetFileSettings(name);

			if ( config ) {
				configs.Insert( config );
			}
		}
		
		return configs;
	}

	map<string, ref T> GetAllSettings()
	{
		return _Settings;
	}

	array<ref T> GetSettings()
	{
		array<ref T> settings = new array<ref T>();

		foreach (T setting: _Settings) {
			settings.Insert(setting);
		}

		return settings;
	}

	bool HasFile(string file_name)
	{
		file_name.ToLower();
		return _Settings.Contains(file_name);
	}
}
