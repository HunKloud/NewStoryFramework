// #ifdef VPPADMINTOOLS
// class NewStoryGroupCategoryListEditorVPP: AdminHudSubMenu
// {
//	 private Widget _CategoriesButtonWidget;
//	 private Widget _SubCategoriesButtonWidget;

//	 private GridSpacerWidget _CategoriesListWidget;
//	 private GridSpacerWidget _SubCategoriesListWidget;

//	 private GridSpacerWidget _CategorySettingsWidget;
//	 private GridSpacerWidget _SubCategorySettingsWidget;

//	 private Widget _CreateButtonWidget;
//	 private Widget _DeleteButtonWidget;
//	 private Widget _SaveButtonWidget;

//	 private ref NewStoryGroupCategoryConfig _Settings;

//	 private ref array<ref NewStoryGroupCategoryListEditorVPP_Item> _CategoriesList;
//	 private ref array<ref NewStoryGroupCategoryListEditorVPP_Item> _SubCategoriesList;

//	 private ref ScriptCaller _SelectCategoryItemCallback;
//	 private ref ScriptCaller _ClickCategoryItemCallback;
//	 private ref ScriptCaller _WarningCallback;

//	 private bool _IsActiveSubCategoryEditor;

//	 private ref array<NewStoryGroupCategoryListEditorVPP_Item> _Selected;
//	 private NewStoryGroupCategoryListEditorVPP_Item _CurrentItem;

//	 private ref map<string, EditBoxWidget> _CategorySettings;
//	 private ref map<string, EditBoxWidget> _SubCategorySettings;

//	 private ref NewStoryGroupWarningTextWidget _WarningText;

//	 void NewStoryGroupCategoryListEditorVPP()
//	 {
//		 _Settings = NewStoryGroup().GetCategoryList().GetCloneConfig();

//		 _SelectCategoryItemCallback = ScriptCaller.Create( OnSelectCategoryItem );
//		 _ClickCategoryItemCallback  = ScriptCaller.Create( OnClickCategoryItem );
//		 _WarningCallback			= ScriptCaller.Create( OnWarningClose );

//		 _CategoriesList	 = new array<ref NewStoryGroupCategoryListEditorVPP_Item>();
//		 _SubCategoriesList  = new array<ref NewStoryGroupCategoryListEditorVPP_Item>();
//		 _Selected		   = new array<NewStoryGroupCategoryListEditorVPP_Item>();

//		 _CategorySettings	   = new map<string, EditBoxWidget>();
//		 _SubCategorySettings	= new map<string, EditBoxWidget>();
//	 }

//	 override void OnCreate(Widget RootW)
//	 {
//		 super.OnCreate( RootW );

// 		M_SUB_WIDGET  = CreateWidgets( "NewStoryGroup/NewStoryFramework/layouts/vpp/CategoryList/Menu.layout" );
// 		M_SUB_WIDGET.SetHandler(this);

// 		m_TitlePanel  = Widget.Cast( M_SUB_WIDGET.FindAnyWidget( "Header") );
// 		m_closeButton = ButtonWidget.Cast( M_SUB_WIDGET.FindAnyWidget( "BtnClose") );

//		 _CategoriesButtonWidget	 = M_SUB_WIDGET.FindAnyWidget( "CategoriesButton" );
//		 _SubCategoriesButtonWidget  = M_SUB_WIDGET.FindAnyWidget( "SubCategoriesButton" );

//		 _CategoriesListWidget	   = GridSpacerWidget.Cast( M_SUB_WIDGET.FindAnyWidget( "CategoriesList" ) );
//		 _SubCategoriesListWidget	= GridSpacerWidget.Cast( M_SUB_WIDGET.FindAnyWidget( "SubCategoriesList" ) );

//		 _CategorySettingsWidget	 = GridSpacerWidget.Cast( M_SUB_WIDGET.FindAnyWidget( "CategorySettings" ) );
//		 _SubCategorySettingsWidget  = GridSpacerWidget.Cast( M_SUB_WIDGET.FindAnyWidget( "SubCategorySettings" ) );

//		 _CreateButtonWidget		 = M_SUB_WIDGET.FindAnyWidget( "CreateButton" );
//		 _DeleteButtonWidget		 = M_SUB_WIDGET.FindAnyWidget( "DeleteButton" );
//		 _SaveButtonWidget		   = M_SUB_WIDGET.FindAnyWidget( "SaveButton" );

//		 LoadCategories();
//		 LoadSubCategories();

//		 LoadCategoriesSettings();
//		 LoadSubCategoriesSettings();

// 		m_Init = true;
//	 }

//	 private void LoadCategories()
//	 {
//		 array<ref NewStoryGroupCategoryData> list = _Settings.Categories;
		
//		 for ( int i = 0; i < list.Count(); i++ ) {
//			 auto item = new NewStoryGroupCategoryListEditorVPP_Item( list[i], _CategoriesListWidget );

//			 item.SetSelectCallback( _SelectCategoryItemCallback );
//			 item.SetClickCallback( _ClickCategoryItemCallback );

//			 _CategoriesList.Insert( item );
//		 }
//	 }

//	 private void LoadSubCategories()
//	 {
//		 array<ref NewStoryGroupCategoryData> list = _Settings.SubCategories;
		
//		 for ( int i = 0; i < list.Count(); i++ ) {
//			 auto item = new NewStoryGroupCategoryListEditorVPP_Item( list[i], _CategoriesListWidget );

//			 item.SetSelectCallback( _SelectCategoryItemCallback );
//			 item.SetClickCallback( _ClickCategoryItemCallback );

//			 _SubCategoriesList.Insert( item );
//		 }
//	 }

//	 private void LoadCategoriesSettings()
//	 {
//		 array<ref Param3<string ,string, int>> settings = NewStoryGroup().GetCategoryList().GetCategoriesSettings();

//		 for ( int i = 0; i < settings.Count(); i++ ) {
//			 Widget w			= GetGame().GetWorkspace().CreateWidgets( "NewStoryGroup/NewStoryFramework/layouts/vpp/CategoryList/SettingItem.layout", _CategorySettingsWidget );
//			 TextWidget name	 = TextWidget.Cast( w.FindAnyWidget( "Key" ) );
//			 EditBoxWidget edit  = EditBoxWidget.Cast( w.FindAnyWidget( "Value" ) );

//			 name.SetText( settings[i].param2 );
//			 _CategorySettings.Insert( settings[i].param1, edit );
//		 }
//	 }

//	 private void LoadSubCategoriesSettings()
//	 {
//		 array<ref Param3<string ,string, int>> settings = NewStoryGroup().GetCategoryList().GetSubCategoriesSettings();

//		 for ( int i = 0; i < settings.Count(); i++ ) {
//			 Widget w			= GetGame().GetWorkspace().CreateWidgets( "NewStoryGroup/NewStoryFramework/layouts/vpp/CategoryList/SettingItem.layout", _CategorySettingsWidget );
//			 TextWidget name	 = TextWidget.Cast( w.FindAnyWidget( "Key" ) );
//			 EditBoxWidget edit  = EditBoxWidget.Cast( w.FindAnyWidget( "Value" ) );

//			 name.SetText( settings[i].param2 );
//			 _SubCategorySettings.Insert( settings[i].param1, edit );
//		 }
//	 }

//	 void OnSelectCategoryItem(NewStoryGroupCategoryListEditorVPP_Item item)
//	 {
//		 if ( item.IsChecked() ) {
//			 _Selected.Insert( item );
//			 _DeleteButtonWidget.ClearFlags( WidgetFlags.DISABLED );
//			 return;
//		 }

//		 for ( int i = _Selected.Count() - 1; i >= 0; i-- ) {
//			 if ( _Selected[i] == item ) {
//				 _Selected.Remove(i);
//				 break;
//			 }
//		 }

//		 if ( _Selected.Count() == 0 ) {
//			 _DeleteButtonWidget.SetFlags( WidgetFlags.DISABLED );
//		 }
//	 }

//	 void OnClickCategoryItem(NewStoryGroupCategoryListEditorVPP_Item item)
//	 {
//		 string categoryName;

//		 array<ref NewStoryGroupCategoryListEditorVPP_Item> list;

//		 if ( _IsActiveSubCategoryEditor ) {
//			 list = _SubCategoriesList;
//			 categoryName = "SubCategory";
//		 }
//		 else {
//			 list = _CategoriesList;
//			 categoryName = "Category";
//		 }

//		 string idText = _CategorySettings["ID"]

//		 for ( int i = 0; i < list.Count(); i++ ) {
//			 if ( list[i] != _CurrentItem && list[i].GetData().ID == _CurrentItem.GetData().ID ) {
//				 string waringText = string.Format( "%1 with id = %2 already in use !", categoryName, 0 ); // !!!
//				 _WarningText = new NewStoryGroupWarningTextWidget( waringText, _WarningCallback, M_SUB_WIDGET );
//				 return;
//			 }
//		 }

//		 _CurrentItem = item;
//	 }

//	 void OnWarningClose()
//	 {
//		 _WarningText = null;
//	 }
	
// 	override bool OnClick(Widget w, int x, int y, int button)
// 	{
//		 if ( w == _CategoriesButtonWidget ) {
//			 _IsActiveSubCategoryEditor = false;

//			 _SubCategoriesButtonWidget.ClearFlags( WidgetFlags.IGNOREPOINTER );
//			 _CategoriesButtonWidget.SetFlags( WidgetFlags.IGNOREPOINTER );

//			 _SubCategoriesButtonWidget.GetParent().SetColor( ARGB( 150, 0, 0, 0 ) );
//			 _CategoriesButtonWidget.GetParent().SetColor( ARGB( 255, 0, 0, 0 ) );

//			 ClearSelectedCategories();

//			 return true;
//		 }

//		 if ( w == _SubCategoriesButtonWidget ) {
//			 _IsActiveSubCategoryEditor = true;
			
//			 _CategoriesButtonWidget.ClearFlags( WidgetFlags.IGNOREPOINTER );
//			 _SubCategoriesButtonWidget.SetFlags( WidgetFlags.IGNOREPOINTER );

//			 _CategoriesButtonWidget.GetParent().SetColor( ARGB( 150, 0, 0, 0 ) );
//			 _SubCategoriesButtonWidget.GetParent().SetColor( ARGB( 255, 0, 0, 0 ) );

//			 ClearSelectedCategories();

//			 return true;
//		 }

// 		return super.OnClick( w, x, y, button );
// 	}

//	 private void ClearSelectedCategories()
//	 {
//		 for ( int i = 0; i < _Selected.Count(); i++ ) {
//			 _Selected[i].ClearChecked();
//		 }

//		 _Selected.Clear();
//	 }

// 	override void OnUpdate(float timeslice)
// 	{
// 		super.OnUpdate(timeslice);

// 		if ( !IsSubMenuVisible() && !m_loaded ) {
//			 return;
//		 }
// 	}
// }
// #endif
