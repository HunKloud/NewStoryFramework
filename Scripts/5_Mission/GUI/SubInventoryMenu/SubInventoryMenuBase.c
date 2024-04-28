class NSgSubInventoryMenuBase: ScriptedWidgetEventHandler
{
	void Init(Widget parent);

	void Show();
	void Hide();

	void Update(float deltaTime);

	void OnCreateRootButton(NSgSubInventoryMenuButton button);
}
