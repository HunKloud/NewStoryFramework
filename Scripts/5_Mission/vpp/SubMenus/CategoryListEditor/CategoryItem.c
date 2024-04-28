// #ifdef VPPADMINTOOLS
// class NewStoryGroupCategoryListEditorVPP_Item
// {
//     private Widget          _Root;
//     private CheckBoxWidget  _CheckWidget;
//     private TextWidget      _NameWidget;

//     private NewStoryGroupCategoryData _Data;

//     private ScriptCaller _SelectCallback;
//     private ScriptCaller _ClickCallback;

//     void NewStoryGroupCategoryListEditorVPP_Item(NewStoryGroupCategoryData data, Widget parent)
//     {
//         _Root = GetGame().GetWorkspace().CreateWidgets( "NewStoryGroup/NewStoryFramework/layouts/vpp/CategoryList/CategoryItem.layout", parent );

//         _CheckWidget    = CheckBoxWidget.Cast( _Root.FindAnyWidget( "Check" ) );
//         _NameWidget     = TextWidget.Cast( _Root.FindAnyWidget( "Name" ) );
        
//         _Data = data;

//         WidgetEventHandler.GetInstance().RegisterOnClick( _CheckWidget, this, "OnClickCheckBox" );
//         WidgetEventHandler.GetInstance().RegisterOnClick( _NameWidget, this, "OnClickName" );
//         WidgetEventHandler.GetInstance().RegisterOnMouseEnter( _NameWidget, this, "OnMouseEnter" );
//         WidgetEventHandler.GetInstance().RegisterOnMouseLeave( _NameWidget, this, "OnMouseLeave" );

//         Update();
//     }

//     void ~NewStoryGroupCategoryListEditorVPP_Item()
//     {
//         if ( _Root ) {
//             _Root.Unlink();
//         }
//     }

//     void SetSelectCallback(ScriptCaller callback)
//     {
//         _SelectCallback = callback;
//         _CheckWidget.ClearFlags( WidgetFlags.DISABLED );
//     }

//     void SetClickCallback(ScriptCaller callback)
//     {
//         _ClickCallback = callback;
//         _NameWidget.ClearFlags( WidgetFlags.IGNOREPOINTER );
//         _NameWidget.SetColor( 0xFFFFFFFF );
//     }

//     Widget GetRoot()
//     {
//         return _Root;
//     }

//     NewStoryGroupCategoryData GetData()
//     {
//         return _Data;
//     }

//     void OnClickCheckBox()
//     {
//         if ( _SelectCallback ) {
//             _SelectCallback.Invoke( this );
//         }
//     }

//     void OnClickName()
//     {
//         if ( _ClickCallback ) {
//             _NameWidget.SetFlags( WidgetFlags.IGNOREPOINTER );
//             _ClickCallback.Invoke( this );
//         }
//     }

//     void OnMouseEnter()
//     {
//         _NameWidget.SetColor( 0xFFFF0000 );
//     }

//     void OnMouseLeave()
//     {
//         if ( ( _NameWidget.GetFlags() & WidgetFlags.IGNOREPOINTER ) != WidgetFlags.IGNOREPOINTER ) {
//             _NameWidget.SetColor( 0xFFFFFFFF );
//         }
//     }

//     void Deselect()
//     {
//         _NameWidget.SetColor( 0xFFFFFFFF );
//         _NameWidget.ClearFlags( WidgetFlags.IGNOREPOINTER );
//     }

//     bool IsChecked()
//     {
//         return _NameWidget.IsChecked();
//     }

//     void ClearChecked()
//     {
//         _NameWidget.SetChecked( false );
//     }

//     void Update()
//     {
//         string name;
//         if ( !data.Settings.Find( "Name", name ) || name == "" ) {
//             name = "Unknow";
//         }
//         else {
//             name = Widget.TranslateString( name );
//         }

//         _NameWidget.SetText( string.Format( "[%1] %2", data.ID, name ) );
//     }
// }
// #endif
