class NewStoryGroupAPI
{
	static const string URI_IMAGE_NEW_STORY_LOGO	= "https://i.ibb.co/LgWPK0c/logo.png";
	private const string URI_DISCORD_WEBHOOK		= "https://discord.com/api/webhooks/";

	private RestContext _DiscordRestContext;

	private RestContext GetDiscordRestContext()
	{
		if ( !_DiscordRestContext )
		{
			RestApi restApi = GetRestApi();

			if ( !restApi )
			{
				restApi = CreateRestApi();
				restApi.EnableDebug(false);
			}

			_DiscordRestContext = restApi.GetRestContext( URI_DISCORD_WEBHOOK );
			_DiscordRestContext.SetHeader("application/json");
		}

		return _DiscordRestContext;
	}

	void SendDiscordMsg(NewStoryGroupDiscordBody data, string id, string token)
	{
		string request = id + "/" + token;
		string body = JsonFileLoader<NewStoryGroupDiscordBody>.JsonMakeData( data );
		GetDiscordRestContext().POST( new NewStoryGroupDiscordRestCallback(), request, body );
	}
}
