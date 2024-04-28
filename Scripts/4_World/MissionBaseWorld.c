modded class MissionBaseWorld: Mission
{
	void MissionBaseWorld()
	{
		NewStoryGroupConstructor();
	}

	protected void NewStoryGroupConstructor();

	protected bool IsNewStoryGroupMissionGameplay()
	{
		return false;
	}

	protected bool IsNewStoryGroupMissionServer()
	{
		return false;
	}

	void AddNewStoryGroupChatMessage(string name, string message, string colorName = "", int channel = 0);
}
