/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     
// Author:      Fabricio D. Rossi
// Modified by: 
// Created:     20/03/2008 19:37:16
// RCS-ID:      
// Copyright:   Las Chanchadas del Guru
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "GUI/catalogadordearchivos.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * CatalogadorApp class declaration
 */

class CatalogadorApp: public wxApp
{    
    DECLARE_CLASS( CatalogadorApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    CatalogadorApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin CatalogadorApp event handler declarations
////@end CatalogadorApp event handler declarations

////@begin CatalogadorApp member function declarations
////@end CatalogadorApp member function declarations

////@begin CatalogadorApp member variables
////@end CatalogadorApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(CatalogadorApp)
////@end declare app

#endif
    // _MAIN_H_
