class NewStoryGroupOptionBodyChangedBase: NewStoryGroupOptionBodyBase
{
	override void Apply()
	{
		bool is_changed = IsChanged();

		super.Apply();

		if ( is_changed ) {
			OnChanged();
		}
	}

	override void SetToDefaults()
	{
		bool is_changed = IsChanged();

		super.SetToDefaults();

		if ( is_changed ) {
			OnChanged();
		}
	}

	protected void OnChanged();
}
