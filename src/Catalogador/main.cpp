/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     
// Author:      Fabricio D. Rossi
// Modified by: 
// Created:     20/03/2008 19:37:16
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

#include "main.h"

////@begin XPM images

////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( CatalogadorApp )
////@end implement app


/*!
 * CatalogadorApp type definition
 */

IMPLEMENT_CLASS( CatalogadorApp, wxApp )


/*!
 * CatalogadorApp event table definition
 */

BEGIN_EVENT_TABLE( CatalogadorApp, wxApp )

////@begin CatalogadorApp event table entries
////@end CatalogadorApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for CatalogadorApp
 */

CatalogadorApp::CatalogadorApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void CatalogadorApp::Init()
{
////@begin CatalogadorApp member initialisation
////@end CatalogadorApp member initialisation
}

/*!
 * Initialisation for CatalogadorApp
 */

bool CatalogadorApp::OnInit()
{    
////@begin CatalogadorApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end CatalogadorApp initialisation

	CatalogadordeArchivos* main = new CatalogadordeArchivos(NULL);

	main->Show(true);

	return true;
}


/*!
 * Cleanup for CatalogadorApp
 */

int CatalogadorApp::OnExit()
{    
////@begin CatalogadorApp cleanup
    return wxApp::OnExit();
////@end CatalogadorApp cleanup
}
