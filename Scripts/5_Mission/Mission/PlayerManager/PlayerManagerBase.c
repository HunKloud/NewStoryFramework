class NewStoryGroupPlayerManagerBase: INewStoryGroupPlayerManagerBase
{
	protected ref map<typename, ref NewStoryGroupPlayerModuleBase> m_Modules;

	void NewStoryGroupPlayerManagerBase()
	{
		m_Modules = new map<typename, ref NewStoryGroupPlayerModuleBase>();
	}

	void LoadModules()
	{
		set<typename> modulesTypes = new set<typename>();

		auto list = new NewStoryGroupPlayerManagerModulesMission();
		
		list.RegisterModules( modulesTypes );
		Sort( modulesTypes, modulesTypes.Count() );

		// Create and initialize modules
		for ( int i = 0; i < modulesTypes.Count(); i++ ) {
			typename type = modulesTypes.Get(i);

			if ( !m_Modules.Contains(type) ) {
				NewStoryGroupPlayerModuleBase module = NewStoryGroupPlayerModuleBase.Cast( type.Spawn() );
				m_Modules.Insert( type, module );

				PreInitModule(module);
				module.OnInit();
				AfterInitModule(module);
			}
		}
	}

	override void OnRPC(ParamsReadContext ctx)
	{
		Param1<int> param;

		if ( !ctx.Read(param) ) {
			ErrorEx( "[NSG PlayerManager] OnRPC :: Can't read 'param'!", ErrorExSeverity.WARNING );
			return;
		}

		for ( int i = 0; i < m_Modules.Count(); i++ ) {
			NewStoryGroupPlayerModuleBase module = m_Modules.GetElement(i);
			if ( module.OnRPC( param.param1, ctx ) ) {
				break;
			}
		}
	}

	override NewStoryGroupPlayerModuleBase GetModule(typename type)
	{
		return m_Modules.Get(type);
	}
}
