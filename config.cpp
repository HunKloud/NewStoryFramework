class CfgPatches
{
	class NewStoryGroupFramework
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "NewStoryGroupLibrary" };
	};
};

class CfgMods
{
	class NewStoryGroupFramework
	{
		name = "NSG Framework";
		author = "HunKloud";
		authorID = "76561197972196282";
		credits = "NEW STORY GROUP";
		defines[] = { "NEW_STORY_GROUP_FRAMEWORK" };
		version = 1.0;
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"NewStoryFramework/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"NewStoryFramework/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"NewStoryFramework/Scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class Inventory_Base;
	class NewStoryGroupEventContainerBase: Inventory_Base
	{
		scope = 0;
		storageCategory = 1;
		carveNavmesh = 1;
		class Cargo
		{
			itemsCargoSize[] = { 10, 100 };
			openable = 0;
			allowOwnedCargoManipulation = 0;
		};
	};

	class NewStoryGroupEventContainer_WoodenCrate: NewStoryGroupEventContainerBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Wooden_Case0";
		descriptionShort = "$STR_CfgVehicles_Wooden_Case1";
		model = "\DZ\gear\camping\wooden_case.p3d";
	};

	class NewStoryGroupServerContainer: Inventory_Base
	{
		scope = 0;
		displayName = "IS NOT FOR USED (DO NOT SPAWN!)";
		class Cargo
		{
			itemsCargoSize[] = { 1000, 1000 };
			openable = 0;
			allowOwnedCargoManipulation = 0;
		};
	};
};
