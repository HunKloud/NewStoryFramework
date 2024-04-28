modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

		actions.Insert(ActionNewStoryGroupOpenEventContainer);
		actions.Insert(ActionNewStoryGroupUnlockEventContainerSigle);
		actions.Insert(ActionNewStoryGroupUnlockEventContainerContinuous);
    }
}
