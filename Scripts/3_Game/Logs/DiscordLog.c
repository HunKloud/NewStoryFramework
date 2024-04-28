class NewStoryGroupDiscordLogData
{
	bool IsActive;

	string WebHookID;
	string WebHookToken;

	string BotName;
	string BotAvatar;
}

class NewStoryGroupDiscordLog
{
	private ref map<string, ref array<ref NewStoryGroupDiscordLogData>> _Data;

	void NewStoryGroupDiscordLog()
	{
		array<string> modules = new array<string>();

		InitModules(modules);

		if ( modules.Count() == 0 )
			return;

		JsonFileLoader<map<string, ref array<ref NewStoryGroupDiscordLogData>>>.JsonLoadFile( NEW_STORY_GROUP_DISCORD_LOG_CONFIG, _Data );

		int notFoundElement = 0;

		for( int i = 0; i < modules.Count(); i++ ) {
			if ( !_Data ) {
				_Data = new map<string, ref array<ref NewStoryGroupDiscordLogData>>();
			}

			if ( !_Data.Contains( modules[i] ) ) {
				notFoundElement += 1;
					
				array<ref NewStoryGroupDiscordLogData> elementData = new array<ref NewStoryGroupDiscordLogData>();
				elementData.Insert( new NewStoryGroupDiscordLogData() );
				_Data.Insert( modules[i], elementData );
			}
		}

		if ( notFoundElement > 0 ) {
			JsonFileLoader<map<string, ref array<ref NewStoryGroupDiscordLogData>>>.JsonSaveFile( NEW_STORY_GROUP_DISCORD_LOG_CONFIG, _Data );
		}
		
		if ( notFoundElement == modules.Count() ) {
			delete _Data;
		}

		if ( _Data ) {
			foreach ( array<ref NewStoryGroupDiscordLogData> list: _Data ) {
				foreach ( NewStoryGroupDiscordLogData item: list ) {
					if ( item.IsActive ) {
						item.IsActive = item.WebHookID != "" && item.WebHookToken != "";
					}
				}
			}
		}
	}

	private void InitModules(array<string> modules);

	bool IsActiveModule(string name)
	{
		array<ref NewStoryGroupDiscordLogData> list;

		if ( !_Data || !_Data.Find(name, list) )
			return false;

		foreach ( NewStoryGroupDiscordLogData item: list )
			if ( item.IsActive )
				return true;

		return false;
	}

	void Send(string name, NewStoryGroupDiscordBody body)
	{
		array<ref NewStoryGroupDiscordLogData> list;

		if ( !body || !_Data || !_Data.Find(name, list) )
			return;
		
		foreach ( NewStoryGroupDiscordLogData item: list )
		{
			if ( item.IsActive )
			{
				if ( item.BotName == "serverName" )
					body.SetUserName( NewStoryGroup().GetServerName() );
				else
					body.SetUserName( item.BotName );

				body.SetAvatarUrl( item.BotAvatar );

				NewStoryGroup().GetAPI().SendDiscordMsg( body, item.WebHookID, item.WebHookToken );
			}
		}
	}

	void SendSystem(string name, string addTitle, string description, int color, string imageUrl)
	{
		NewStoryGroupDiscordEmbeds discordEmbed = new NewStoryGroupDiscordEmbeds();

		discordEmbed.SetColor( color );

		if ( addTitle != "" ) {
			discordEmbed.SetTitle( "__[Системное уведомление]__\n\n" + addTitle );
		}
		else {
			discordEmbed.SetTitle( "__[Системное уведомление]__" );
		}

		discordEmbed.SetDescription( description );
		discordEmbed.SetEmbedImage( imageUrl );
		discordEmbed.SetNewStoryGroupGroup();
		discordEmbed.SetTimeStamp();

		NewStoryGroupDiscordBody discordBody = new NewStoryGroupDiscordBody();
		
		discordBody.AddEmbed( discordEmbed );

		Send( name, discordBody );
	}
}

bool NewStoryGroupIsActiveDiscordLog(string name)
{
	if ( !NewStoryGroup() || !NewStoryGroup().GetDiscordLog() ) {
		return false;
	}
	
	return NewStoryGroup().GetDiscordLog().IsActiveModule(name);
}

void NewStoryGroupSendDiscordLog(string name, NewStoryGroupDiscordBody body)
{
	if ( NewStoryGroup() && NewStoryGroup().GetDiscordLog() ) {
		NewStoryGroup().GetDiscordLog().Send(name, body);
	}
}

void NewStoryGroupSendDiscordSystemLog(string name, string addTitle, string description, int color = 0xFFFFFF, string imageUrl = "")
{
	if ( NewStoryGroup() && NewStoryGroup().GetDiscordLog() ) {
		NewStoryGroup().GetDiscordLog().SendSystem( name, addTitle, description, color, imageUrl );
	}
}
