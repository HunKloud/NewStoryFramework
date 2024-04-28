modded class OptionsMenu: UIScriptedMenu
{
    override bool IsAnyTabChanged()
    {
        return super.IsAnyTabChanged() || m_SoundsTab.NewStoryGroupIsChanged();
    }

    override void Apply()
    {
        super.Apply();
		
		if ( m_SoundsTab.NewStoryGroupIsChanged() ) {
			m_SoundsTab.NewStoryGroupApply();
		}
    }
}
