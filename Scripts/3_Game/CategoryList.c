class NewStoryGroupCategoryData
{
    int ID;
    ref map<string, string> Settings = new map<string, string>();

    NewStoryGroupCategoryData Clone()
    {
        NewStoryGroupCategoryData data = new NewStoryGroupCategoryData();
        data.ID = ID;
        data.Settings.Copy( Settings );

        return data;
    }
}

class NewStoryGroupCategoryConfig
{
    ref array<ref NewStoryGroupCategoryData> Categories = new array<ref NewStoryGroupCategoryData>();
    ref array<ref NewStoryGroupCategoryData> SubCategories = new array<ref NewStoryGroupCategoryData>();

    void CreateDefaultConfig()
    {
        Categories.Insert( new NewStoryGroupCategoryData() );
        Categories.Insert( new NewStoryGroupCategoryData() );
        Categories.Insert( new NewStoryGroupCategoryData() );

        Categories[0].ID = 10;
        Categories[0].Settings.Insert( "Name", "Category 1" );
        Categories[0].Settings.Insert( "Description", "Description should be here" );
        Categories[0].Settings.Insert( "Icon", "set:ccgui_enforce image:HudBuild" );

        Categories[1].ID = 20;
        Categories[1].Settings.Insert( "Name", "Category 2" );
        Categories[1].Settings.Insert( "Description", "Description should be here" );
        Categories[1].Settings.Insert( "Icon", "set:ccgui_enforce image:HudBuild" );

        Categories[2].ID = 30;
        Categories[2].Settings.Insert( "Name", "Category 3" );
        Categories[2].Settings.Insert( "Description", "Description should be here" );
        Categories[2].Settings.Insert( "Icon", "set:ccgui_enforce image:HudBuild" );

        SubCategories.Insert( new NewStoryGroupCategoryData() );
        SubCategories.Insert( new NewStoryGroupCategoryData() );
        SubCategories.Insert( new NewStoryGroupCategoryData() );

        SubCategories[0].ID = 10;
        SubCategories[0].Settings.Insert( "Name", "Sub category 1" );

        SubCategories[1].ID = 20;
        SubCategories[1].Settings.Insert( "Name", "Sub category 2" );

        SubCategories[2].ID = 30;
        SubCategories[2].Settings.Insert( "Name", "Sub category 3" );
    }

    void Sorted()
    {
        
    }

    NewStoryGroupCategoryConfig Clone()
    {
        NewStoryGroupCategoryConfig config = new NewStoryGroupCategoryConfig();

        int i = 0;

        for ( ; i < Categories.Count(); i++ ) {
            config.Categories.Insert( Categories[i].Clone() );
        }

        for ( i = 0; i < SubCategories.Count(); i++ ) {
            config.SubCategories.Insert( SubCategories[i].Clone() );
        }

        return config;
    }
}

class NewStoryGroupCategoryList
{
    private static const string CONFIG_PATH = NEW_STORY_GROUP_CONFIG_FOLDER + "Categories.json";

    private ref NewStoryGroupCategoryConfig _Config;

    void NewStoryGroupCategoryList()
    {
        NewStoryGroup().RPC().Add( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_CATEGORY_LIST,   ScriptCaller.Create( ReadSyncConfig ),      ENewStoryGroupRPCInstance.CLIENT );
        NewStoryGroup().RPC().Add( "NewStoryGroupFramework", ENewStoryGroupRPCManager.UPDATE_CATEGORY_LIST, ScriptCaller.Create( ReadUpdateConfig ),    ENewStoryGroupRPCInstance.SERVER );
    }

    void OnCreateMissionServer()
    {
        JsonFileLoader<NewStoryGroupCategoryConfig>.JsonLoadFile( CONFIG_PATH, _Config );

        if ( !_Config ) {
            _Config = new NewStoryGroupCategoryConfig();
            _Config.CreateDefaultConfig();
            JsonFileLoader<NewStoryGroupCategoryConfig>.JsonSaveFile( CONFIG_PATH, _Config );
        }

        NewStoryGroup().GetPlayerManagers().Event_OnStartConnection().Insert( OnPlayerStartConnection );
    }

    void OnDestroyMission()
    {
        _Config = null;
    }

    void OnPlayerStartConnection(PlayerIdentity identity)
    {
        Param param = new Param1<ref NewStoryGroupCategoryConfig>( _Config );
        NewStoryGroup().RPC().SendSingleParam( "NewStoryGroupFramework", ENewStoryGroupRPCManager.SYNC_CATEGORY_LIST, param, identity );
    }
    
    void ReadSyncConfig(PlayerIdentity sender, ParamsReadContext ctx)
    {
        Param1<ref NewStoryGroupCategoryConfig> paramConfig;
        if ( !ctx.Read( paramConfig ) ) {
            ErrorEx( "Can't read param 'sync categories'", ErrorExSeverity.WARNING );
            return;
        }

        _Config = paramConfig.param1;
    }
    
    void ReadUpdateConfig(PlayerIdentity sender, ParamsReadContext ctx)
    {

    }

    bool HasConfig()
    {
        return _Config != null;
    }

    NewStoryGroupCategoryConfig GetCloneConfig()
    {
        if ( _Config ) {
            return _Config.Clone();
        }
        
        return null;
    }

    array<ref NewStoryGroupCategoryData> GetCategories()
    {
        if ( _Config ) {
            return _Config.Categories;
        }

        return null;
    }

    array<ref NewStoryGroupCategoryData> GetSubCategories()
    {
        if ( _Config ) {
            return _Config.SubCategories;
        }

        return null;
    }

    NewStoryGroupCategoryData GetCategory(int id)
    {
        if ( !_Config ) {
            return null;
        }

        for ( int i = 0; i < _Config.Categories.Count(); i++ ) {
            if ( _Config.Categories[i].ID == id ) {
                return _Config.Categories[i];
            }
        }

        return null;
    }

    NewStoryGroupCategoryData GetSubCategory(int id)
    {
        if ( !_Config ) {
            return null;
        }

        for ( int i = 0; i < _Config.SubCategories.Count(); i++ ) {
            if ( _Config.SubCategories[i].ID == id ) {
                return _Config.SubCategories[i];
            }
        }

        return null;
    }

    bool HasCategories(int id)
    {
        if ( !_Config ) {
            return false;
        }

        for ( int i = 0; i < _Config.Categories.Count(); i++ ) {
            if ( _Config.Categories[i].ID == id ) {
                return true;
            }
        }

        return false;
    }

    bool HasSubCategories(int id)
    {
        if ( !_Config ) {
            return false;
        }

        for ( int i = 0; i < _Config.SubCategories.Count(); i++ ) {
            if ( _Config.SubCategories[i].ID == id ) {
                return true;
            }
        }

        return false;
    }
}
