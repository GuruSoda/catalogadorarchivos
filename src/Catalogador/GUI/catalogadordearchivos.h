/////////////////////////////////////////////////////////////////////////////
// Name:        catalogadordearchivos.h
// Purpose:     
// Author:      Fabricio D. Rossi
// Modified by: 
// Created:     20/03/2008 19:49:53
// RCS-ID:      
// Copyright:   Las Chanchadas del Guru
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CATALOGADORDEARCHIVOS_H_
#define _CATALOGADORDEARCHIVOS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/splitter.h"
#include "wx/treectrl.h"
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxTreeCtrl;
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_CATALOGADORDEARCHIVOS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CATALOGADORDEARCHIVOS_TITLE _("Catalogador de Archivos")
#define SYMBOL_CATALOGADORDEARCHIVOS_IDNAME ID_CATALOGADORDEARCHIVOS
#define SYMBOL_CATALOGADORDEARCHIVOS_SIZE wxSize(400, 300)
#define SYMBOL_CATALOGADORDEARCHIVOS_POSITION wxDefaultPosition
////@end control identifiers

// Declare a class to hold tree item data
class DatoArbol : public wxTreeItemData
{
public:
	DatoArbol(const wxString& desc) : m_desc(desc) { }
	const wxString& GetDesc() const { return m_desc; }
private:
	wxString m_desc;
};

/*!
 * CatalogadordeArchivos class declaration
 */

class CatalogadordeArchivos: public wxFrame
{    
    DECLARE_CLASS( CatalogadordeArchivos )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CatalogadordeArchivos();
    CatalogadordeArchivos( wxWindow* parent, wxWindowID id = SYMBOL_CATALOGADORDEARCHIVOS_IDNAME, const wxString& caption = SYMBOL_CATALOGADORDEARCHIVOS_TITLE, const wxPoint& pos = SYMBOL_CATALOGADORDEARCHIVOS_POSITION, const wxSize& size = SYMBOL_CATALOGADORDEARCHIVOS_SIZE, long style = SYMBOL_CATALOGADORDEARCHIVOS_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CATALOGADORDEARCHIVOS_IDNAME, const wxString& caption = SYMBOL_CATALOGADORDEARCHIVOS_TITLE, const wxPoint& pos = SYMBOL_CATALOGADORDEARCHIVOS_POSITION, const wxSize& size = SYMBOL_CATALOGADORDEARCHIVOS_SIZE, long style = SYMBOL_CATALOGADORDEARCHIVOS_STYLE );

    /// Destructor
    ~CatalogadordeArchivos();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	void CargaArbol();

////@begin CatalogadordeArchivos event handler declarations

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_ARBOLETIQUETAS
    void OnSeleccionEtiqueta( wxTreeEvent& event );

////@end CatalogadordeArchivos event handler declarations

////@begin CatalogadordeArchivos member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CatalogadordeArchivos member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin CatalogadordeArchivos member variables
    wxTreeCtrl* ArbolEtiquetas;
    wxListCtrl* ListaArchivos;
    /// Control identifiers
    enum {
        ID_CATALOGADORDEARCHIVOS = 10000,
        ID_MENUITEM = 10002,
        ID_MENUITEM1 = 10003,
        ID_TOOLBAR1 = 10004,
        ID_STATUSBAR1 = 10005,
        ID_SPLITTERWINDOW1 = 10006,
        ID_ARBOLETIQUETAS = 10007,
        ID_LISTAARCHIVOS = 10008
    };
////@end CatalogadordeArchivos member variables
};

#endif
    // _CATALOGADORDEARCHIVOS_H_
