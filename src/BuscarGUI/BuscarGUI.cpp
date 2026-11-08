///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "BuscarGUI.h"

///////////////////////////////////////////////////////////////////////////

Buscador::Buscador( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	barraMenu = new wxMenuBar( 0 );
	menuArchivo = new wxMenu();
	wxMenuItem* menuAbrir;
	menuAbrir = new wxMenuItem( menuArchivo, wxID_OPEN, wxString( wxT("Abrir Catalogo") ) , wxEmptyString, wxITEM_NORMAL );
	menuArchivo->Append( menuAbrir );
	
	wxMenuItem* menuSalir;
	menuSalir = new wxMenuItem( menuArchivo, wxID_EXIT, wxString( wxT("Salir") ) , wxEmptyString, wxITEM_NORMAL );
	menuArchivo->Append( menuSalir );
	
	barraMenu->Append( menuArchivo, wxT("Archivo") );
	
	menuAcerca = new wxMenu();
	wxMenuItem* menuItemAcercade;
	menuItemAcercade = new wxMenuItem( menuAcerca, ID_ABOUT, wxString( wxT("Acerca de..") ) , wxEmptyString, wxITEM_NORMAL );
	menuAcerca->Append( menuItemAcercade );
	
	barraMenu->Append( menuAcerca, wxT("Acerca") );
	
	this->SetMenuBar( barraMenu );
	
	barra_herramientas = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	barra_herramientas->AddTool( wxID_ANY, wxT("tool"), wxBitmap( wxT("Battery.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	barra_herramientas->AddTool( wxID_ANY, wxT("tool"), wxBitmap( wxT("Battery.ico"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	barra_herramientas->Realize();
	
	barraEstado = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitter6 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter6->Connect( wxEVT_IDLE, wxIdleEventHandler( Buscador::m_splitter6OnIdle ), NULL, this );
	m_panelIzquierdo = new wxPanel( m_splitter6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	ArbolEtiquetas = new wxTreeCtrl( m_panelIzquierdo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HAS_BUTTONS|wxTR_SINGLE|wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	bSizer10->Add( ArbolEtiquetas, 1, wxALL|wxEXPAND, 5 );
	
	historialCatalogos = new wxComboBox( m_panelIzquierdo, wxID_ANY, wxT("Elija!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxTE_PROCESS_ENTER );
	historialCatalogos->Append( wxT("/opt/catalogos/GuruChuchis.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/libros.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/priya.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/denise.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/GuruPelis.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/GuruCompilados.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/mimame.fdb") );
	historialCatalogos->Append( wxT("/opt/catalogos/imagenes.fdb") );
	bSizer10->Add( historialCatalogos, 0, wxALL|wxEXPAND, 5 );
	
	m_panelIzquierdo->SetSizer( bSizer10 );
	m_panelIzquierdo->Layout();
	bSizer10->Fit( m_panelIzquierdo );
	m_panelDerecho = new wxPanel( m_splitter6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	EntradasEtiquetas = new wxListCtrl( m_panelDerecho, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ALIGN_LEFT|wxLC_REPORT|wxSUNKEN_BORDER );
	bSizer11->Add( EntradasEtiquetas, 1, wxALL|wxEXPAND, 5 );
	
	m_panelDerecho->SetSizer( bSizer11 );
	m_panelDerecho->Layout();
	bSizer11->Fit( m_panelDerecho );
	m_splitter6->SplitVertically( m_panelIzquierdo, m_panelDerecho, 213 );
	bSizer1->Add( m_splitter6, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( menuAbrir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Buscador::AbrirCatalogo ) );
	this->Connect( menuSalir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Buscador::Terminar ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( Buscador::AbrirCatalogo ) );
	ArbolEtiquetas->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( Buscador::OnMenuContextualArbol ), NULL, this );
	ArbolEtiquetas->Connect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( Buscador::MostrarSeleccion ), NULL, this );
	historialCatalogos->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( Buscador::EleccionCombo ), NULL, this );
	EntradasEtiquetas->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( Buscador::OnContextMenuEntrada ), NULL, this );
}

Buscador::~Buscador()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Buscador::AbrirCatalogo ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Buscador::Terminar ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( Buscador::AbrirCatalogo ) );
	ArbolEtiquetas->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( Buscador::OnMenuContextualArbol ), NULL, this );
	ArbolEtiquetas->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( Buscador::MostrarSeleccion ), NULL, this );
	historialCatalogos->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( Buscador::EleccionCombo ), NULL, this );
	EntradasEtiquetas->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( Buscador::OnContextMenuEntrada ), NULL, this );
}

Busqueda::Busqueda( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textCtrl1 = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_textCtrl1, 0, wxALL, 5 );
	
	wxString m_choice1Choices[] = { wxT("Nombre"), wxT("Tamaño"), wxT("Fecha"), wxT("CRC32"), wxT("SHA1"), wxT("SHA512") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 5 );
	bSizer8->Add( m_choice1, 0, wxALL, 5 );
	
	m_button4 = new wxButton( m_panel4, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel4->SetSizer( bSizer8 );
	m_panel4->Layout();
	bSizer8->Fit( m_panel4 );
	bSizer6->Add( m_panel4, 0, wxALL|wxEXPAND, 5 );
	
	m_listCtrl2 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer6->Add( m_listCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	bSizer4->Add( bSizer6, 1, wxEXPAND, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2->Realize();
	bSizer4->Add( m_sdbSizer2, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
}

Busqueda::~Busqueda()
{
}
