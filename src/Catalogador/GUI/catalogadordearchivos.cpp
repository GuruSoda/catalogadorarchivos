/////////////////////////////////////////////////////////////////////////////
// Name:        catalogadordearchivos.cpp
// Purpose:     
// Author:      Fabricio D. Rossi
// Modified by: 
// Created:     20/03/2008 19:49:53
// RCS-ID:      
// Copyright:   Las Chanchadas del Guru
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "catalogadordearchivos.h"

////@begin XPM images
////@end XPM images


/*!
 * CatalogadordeArchivos type definition
 */

IMPLEMENT_CLASS( CatalogadordeArchivos, wxFrame )


/*!
 * CatalogadordeArchivos event table definition
 */

BEGIN_EVENT_TABLE( CatalogadordeArchivos, wxFrame )

////@begin CatalogadordeArchivos event table entries
    EVT_TREE_SEL_CHANGED( ID_ARBOLETIQUETAS, CatalogadordeArchivos::OnSeleccionEtiqueta )

////@end CatalogadordeArchivos event table entries

END_EVENT_TABLE()


/*!
 * CatalogadordeArchivos constructors
 */

CatalogadordeArchivos::CatalogadordeArchivos()
{
    Init();
}

CatalogadordeArchivos::CatalogadordeArchivos( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * CatalogadordeArchivos creator
 */

bool CatalogadordeArchivos::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CatalogadordeArchivos creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end CatalogadordeArchivos creation
    return true;
}


/*!
 * CatalogadordeArchivos destructor
 */

CatalogadordeArchivos::~CatalogadordeArchivos()
{
////@begin CatalogadordeArchivos destruction
////@end CatalogadordeArchivos destruction
}


/*!
 * Member initialisation
 */

void CatalogadordeArchivos::Init()
{
////@begin CatalogadordeArchivos member initialisation
    ArbolEtiquetas = NULL;
    ListaArchivos = NULL;
////@end CatalogadordeArchivos member initialisation
}


/*!
 * Control creation for CatalogadordeArchivos
 */

void CatalogadordeArchivos::CreateControls()
{    
////@begin CatalogadordeArchivos content construction
    CatalogadordeArchivos* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENUITEM, _("Item"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(ID_MENUITEM1, _("Item"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("Menu"));
    itemFrame1->SetMenuBar(menuBar);

    wxToolBar* itemToolBar6 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR1 );
    itemToolBar6->Realize();
    itemFrame1->SetToolBar(itemToolBar6);

    wxStatusBar* itemStatusBar7 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar7->SetFieldsCount(2);
    itemFrame1->SetStatusBar(itemStatusBar7);

    wxSplitterWindow* itemSplitterWindow8 = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW1, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow8->SetMinimumPaneSize(0);

    ArbolEtiquetas = new wxTreeCtrl( itemSplitterWindow8, ID_ARBOLETIQUETAS, wxDefaultPosition, wxSize(100, 100), wxTR_HIDE_ROOT|wxTR_SINGLE );

    ListaArchivos = new wxListCtrl( itemSplitterWindow8, ID_LISTAARCHIVOS, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT );

    itemSplitterWindow8->SplitVertically(ArbolEtiquetas, ListaArchivos, 50);

////@end CatalogadordeArchivos content construction

	CargaArbol();
}


/*!
 * Should we show tooltips?
 */

bool CatalogadordeArchivos::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CatalogadordeArchivos::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CatalogadordeArchivos bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CatalogadordeArchivos bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CatalogadordeArchivos::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CatalogadordeArchivos icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CatalogadordeArchivos icon retrieval
}


/*!
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_TREECTRL1
 */

void CatalogadordeArchivos::OnSeleccionEtiqueta( wxTreeEvent& event )
{
	OutputDebugString("Seleccion!\n");
////@begin wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_TREECTRL1 in CatalogadordeArchivos.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_TREECTRL1 in CatalogadordeArchivos. 
}

void CatalogadordeArchivos::CargaArbol()
{
	wxTreeItemId rootId = ArbolEtiquetas->AddRoot(wxT("Raiz"), 0, 0, new DatoArbol(wxT("Root item")));
	wxTreeItemId itemId1 = ArbolEtiquetas->AppendItem(rootId, wxT("File 1"), -1, -1, new DatoArbol(wxT("File item 1")));
	wxTreeItemId itemId2 = ArbolEtiquetas->AppendItem(rootId, wxT("File 2"), -1, -1, new DatoArbol(wxT("File item 2")));
	wxTreeItemId itemId3 = ArbolEtiquetas->AppendItem(rootId, wxT("File 3"), -1, -1, new DatoArbol(wxT("File item 3")));
	wxTreeItemId itemId4 = ArbolEtiquetas->AppendItem(rootId, wxT("File 4"), -1, -1, new DatoArbol(wxT("File item 4")));
	wxTreeItemId itemId5 = ArbolEtiquetas->AppendItem(rootId, wxT("File 5"), -1, -1, new DatoArbol(wxT("File item 5")));
}
