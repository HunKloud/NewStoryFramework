class NewStoryGroupFlowingWidgetBase
{
	protected Widget	_RootWidget;
	protected bool	  _IsVisible;
	protected bool	  _HudIsVisible;

	void NewStoryGroupFlowingWidgetBase()
	{
		_RootWidget	 = CreateRootWidget();
		_IsVisible	  = true;
		_HudIsVisible   = true;

		Init();
	}

	void ~NewStoryGroupFlowingWidgetBase()
	{
		if ( _RootWidget ) {
			_RootWidget.Unlink();
		}
	}
	
	protected Widget CreateRootWidget();
	protected void Init();

	protected float GetDistance()
	{
		return vector.Distance( GetGame().GetPlayer().GetPosition(), GetWorldPosition() );
	}

	// Возвращает координату, на которой надо отоборазить виджет
	//  может отличаться от фактической позиции предмета (если используется)
	protected vector GetWorldPosition();

	protected bool CanShow()
	{
		return _IsVisible && _HudIsVisible;
	}

	void Update()
	{
		if ( !CanShow() ) {
			_RootWidget.Show(false);
			return;
		}

		PreCheckOutsideScreen();

		vector screenPosition;
		vector worldPosition = GetWorldPosition();
		// Определяем, находится ли за пределами монитора
		if ( OutsideScreen( worldPosition, screenPosition ) ) {
			OnOutsideScreen( screenPosition );
			_RootWidget.Show(false);
			return;
		}

		_RootWidget.Show(true);

		PreSetPosition( screenPosition );
		_RootWidget.SetPos( screenPosition[0], screenPosition[1] );
		AfterSetPosition( screenPosition );
	}

	void Show(bool value)
	{
		_IsVisible = value;
	}

	void HudIsVisible(bool value)
	{
		_HudIsVisible = value;
	}

	// Если нам не надо автоматически скрывыть вместе с интерфейсом
	//  выставляем 'false'
	bool DependsOnHUD()
	{
		return true;
	}

	// Вызывается перед тем как получить позицию на экране
	protected void PreCheckOutsideScreen();
	// Вызывается, если находится за пределами монитора
	protected void OnOutsideScreen(vector screenPosition);
	// Вызывается перед выставлением позиции на мониторе
	protected void PreSetPosition(vector screenPosition);
	// Вызывается после выставления позиции на мониторе
	protected void AfterSetPosition(vector screenPosition);

	// Принимает координаты из мира, возвращает координаты на экране, где x и y округлены в большую сторону
	protected bool OutsideScreen(vector worldPos, out vector screenPosition)
	{
		screenPosition = GetGame().GetScreenPos( worldPos );

		screenPosition[0] = Math.Ceil( screenPosition[0] );
		screenPosition[1] = Math.Ceil( screenPosition[1] );

		if ( screenPosition[0] <= 0 || screenPosition[1] <= 0 || screenPosition[2] < 0 ) {
			return true;
		}

		int screenWidth, screenHeight;
		GetScreenSize( screenWidth, screenHeight );

		return screenPosition[0] >= screenWidth || screenPosition[1] >= screenHeight;
	}
}
