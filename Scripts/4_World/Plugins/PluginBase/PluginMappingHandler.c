class NewStoryGroupPluginMappingHandler: PluginBase
{
	// ECE_SETUP|ECE_UPDATEPATHGRAPH|ECE_CREATEPHYSICS|ECE_NOLIFETIME
	static const int DEFAULT_MAPPING_FLAG = 4195362;

	private ref map<BuildingBase, ref Param3<vector, vector, float>> m_Objects;

	Object SpawnMapObject(string type, vector position, vector orientation = "0 0 0", float scale = 1.0, bool requestRespawn = false, int flags = DEFAULT_MAPPING_FLAG)
	{
		Object obj = GetGame().CreateObjectEx( type, position, flags );

		if ( !obj ) {
			return null;
		}

		obj.SetOrientation( orientation );
		obj.SetScale( scale );
		obj.SetFlags( EntityFlags.STATIC, false );
		obj.SetAffectPathgraph( true, false );
		obj.Update();

		if ( requestRespawn && obj.IsInherited( BuildingBase ) ) {
			BuildingBase building = BuildingBase.Cast( obj );
			building.SetNewStoryGroupRequestRespawn( true, flags );

			if ( !m_Objects ) {
				m_Objects = new map<BuildingBase, ref Param3<vector, vector, float>>();
			}

			m_Objects.Insert( building, new Param3<vector, vector, float>( position, orientation, scale ) );
		}

		return obj;
	}

	void DestroyMapObjects(array<Object> objects)
	{
		foreach ( Object obj: objects ) {
			DestroyMapObject( obj );
		}
	}

	void DestroyMapObject(Object obj)
	{
		if ( obj.IsInherited( BuildingBase ) ) {
			BuildingBase building = BuildingBase.Cast( obj );

			if ( building.IsNewStoryGroupRequestRespawn() ) {
				building.SetNewStoryGroupRequestRespawn( false );
				m_Objects.Remove( building );
			}
		}

		obj.Delete();
	}

	void OnDeleteBuilding(BuildingBase obj)
	{
		Param3<vector, vector, float> params;

		if ( !m_Objects || m_Objects.Find( obj, params ) ) {
			return;			
		}

		string type					= obj.GetType();
		bool requestRespawn = obj.IsNewStoryGroupRequestRespawn();
		int flags						= obj.GetNewStoryGroupSpawnFlags();

		BuildingBase newObj = BuildingBase.Cast( SpawnMapObject( type, params.param1, params.param2, params.param3, requestRespawn, flags ) );
		m_Objects.ReplaceKey( obj, newObj );
	}

	static bool CanCreateObject(string type)
	{
		string path = "CfgVehicles " + type;

		if ( !NewStoryGroupObjectFunction.ItemIsKindOf( "", "House", path ) ) {
			return false;
		}

		int storageCategory = GetGame().ConfigGetInt( path + " storageCategory" );
		return storageCategory == 0 || storageCategory == 7;
	}
}
