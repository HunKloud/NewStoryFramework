class NewStoryGroupPluginSpawnItemPresets: PluginBase
{
	private ref map<int, ref NewStoryGroupSpawnItemPresetData> m_Presets;

	private int m_StartIndexPath;

	override void OnInit()
	{
		m_Presets = new map<int, ref NewStoryGroupSpawnItemPresetData>();

		NewStoryGroupSystemFunction.CreateFolder( NEW_STORY_GROUP_SPAWN_ITEM_PRESETS );

		array<string> buffer = new array<string>();
		NEW_STORY_GROUP_SPAWN_ITEM_PRESETS.Split( "\\", buffer );
		m_StartIndexPath = buffer.Count();

		ScriptCaller callbackFunc = ScriptCaller.Create( ProcessLoadFile );
		NewStoryGroupSystemFunction.ProcessAllFilesOnDirectory( NEW_STORY_GROUP_SPAWN_ITEM_PRESETS, callbackFunc, ".json" );
	}

	void ProcessLoadFile(string fileDir, string fileName)
	{
		NewStoryGroupSpawnItemPresetData preset;
		JsonFileLoader<NewStoryGroupSpawnItemPresetData>.JsonLoadFile( fileDir + fileName, preset );

		if ( !preset ) {
			return;
		}

		int i = preset.List.Count() - 1;

		for ( ; i >= 0; i-- ) {
			NewStoryGroupItemWithAttachment itemData = preset.List[i];
			itemData.Init();

			if ( !itemData.IsCorrect() ) {
				preset.List.Remove(i);
			}
		}

		if ( preset.List.Count() == 0 ) {
			return;
		}

		array<string> buffer = new array<string>();
		fileDir.Split( "\\", buffer );

		string newFileName = "";

		for ( i = m_StartIndexPath; i < buffer.Count(); i++ ) {
			newFileName += buffer[i] + "-";
		}

		newFileName += fileName.Substring( 0, fileName.Length() - 5 );
		newFileName.ToLower();

		int fileNameHash = newFileName.Hash();
		
		m_Presets.Insert( fileNameHash, preset );

		Print( "[NSG PluginSpawnItemPresets] Load file with name: " + newFileName );
	}

	void CorrectedPresetList(notnull array<int> presets)
	{
		if ( !m_Presets ) {
			presets.Clear();
		}

		for ( int i = presets.Count() - 1; i >= 0; i-- ) {
			if ( !m_Presets.Contains( presets[i] ) ) {
				presets.Remove(i);
			}
		}
	}

	bool HasPreset(int presetHash)
	{
		return m_Presets && m_Presets.Contains( presetHash );
	}

	bool SpawnPreset(notnull EntityAI container, int presetHash, bool ignorePresetChances = false, bool ignoreItemChances = false)
	{
		string chance;

		NewStoryGroupItemWithAttachment itemData = null;
		NewStoryGroupSpawnItemPresetData presetData = null;

		if ( !m_Presets.Find( presetHash, presetData ) ) {
			return false;
		}

		if ( !ignorePresetChances && Math.RandomFloat01() > presetData.Chance ) {
			return false;
		}

		int selectItemCount = 0;

		for ( int i = 0; i < presetData.List.Count(); i++ ) {
			itemData = presetData.List[i];

			if ( !ignoreItemChances && itemData.GetArgumentValue( "Chance", chance ) && Math.RandomFloat01() > chance.ToFloat() ) {
				continue;
			}

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( NewStoryGroupItemWithAttachmentMethods.SpawnInInvetory, itemData, container, false );
			selectItemCount++;
		}

		return selectItemCount != 0;
	}

	bool SpawnPresets(notnull EntityAI container, notnull array<int> presets, bool ignorePresetChances = false, bool ignoreItemChances = false)
	{
		string chance;

		NewStoryGroupItemWithAttachment itemData = null;
		NewStoryGroupSpawnItemPresetData presetData = null;

		int selectItemCount = 0;

		for ( int i = 0; i < presets.Count(); i++ ) {
			if ( !m_Presets.Find( presets[i], presetData ) ) {
				continue;
			}

			if ( !ignorePresetChances && Math.RandomFloat01() > presetData.Chance ) {
				continue;
			}

			for ( int j = 0; j < presetData.List.Count(); j++ ) {
				itemData = presetData.List[j];

				if ( !ignoreItemChances && itemData.GetArgumentValue( "Chance", chance ) && Math.RandomFloat01() > chance.ToFloat() ) {
					continue;
				}

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( NewStoryGroupItemWithAttachmentMethods.SpawnInInvetory, itemData, container, false );
				selectItemCount++;
			}
		}

		return selectItemCount != 0;
	}

	bool SpawnOneFromPresets(notnull EntityAI container, notnull array<int> presets, bool ignorePresetChances = false, bool ignoreItemChances = false)
	{
		string chance;
		
		array<NewStoryGroupSpawnItemPresetData> selectPresets = new array<NewStoryGroupSpawnItemPresetData>();

		NewStoryGroupItemWithAttachment itemData = null;
		NewStoryGroupSpawnItemPresetData presetData = null;
		NewStoryGroupSpawnItemPresetData presetDataWithMaxChance = null;

		int i = 0;

		for ( ; i < presets.Count(); i++ ) {
			if ( !m_Presets.Find( presets[i], presetData ) ) {
				continue;
			}

			if ( !ignorePresetChances ) {
				if ( !presetDataWithMaxChance || presetDataWithMaxChance.Chance < presetData.Chance ) {
					presetDataWithMaxChance = presetData;
				}

				if ( Math.RandomFloat01() > presetData.Chance ) {
					continue;
				}
			}

			selectPresets.Insert(presetData);
		}

		if ( selectPresets.Count() != 0 ) {
			presetData = selectPresets.GetRandomElement();
		}
		else {
			presetData = presetDataWithMaxChance;
		}

		if ( presetData ) {
			int selectItemCount = 0;

			for ( i = 0; i < presetData.List.Count(); i++ ) {
				itemData = presetData.List[i];

				if ( !ignoreItemChances && itemData.GetArgumentValue( "Chance", chance ) && Math.RandomFloat01() > chance.ToFloat() ) {
					continue;
				}

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( NewStoryGroupItemWithAttachmentMethods.SpawnInInvetory, itemData, container, false );
				selectItemCount++;
			}

			return selectItemCount != 0;
		}

		return false;
	}
}
