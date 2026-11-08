///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __BuscarGUI__
#define __BuscarGUI__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_ABOUT 1000

///////////////////////////////////////////////////////////////////////////////
/// Class Buscador
///////////////////////////////////////////////////////////////////////////////
class Buscador : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* barraMenu;
		wxMenu* menuArchivo;
		wxMenu* menuAcerca;
		wxToolBar* barra_herramientas;
		wxStatusBar* barraEstado;
		wxSplitterWindow* m_splitter6;
		wxPanel* m_panelIzquierdo;
		wxTreeCtrl* ArbolEtiquetas;
		wxComboBox* historialCatalogos;
		wxPanel* m_panelDerecho;
		wxListCtrl* EntradasEtiquetas;
		
		// Virtual event handlers, overide them in your derived class
		virtual void AbrirCatalogo( wxCommandEvent& event ){ event.Skip(); }
		virtual void Terminar( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnMenuContextualArbol( wxTreeEvent& event ){ event.Skip(); }
		virtual void MostrarSeleccion( wxTreeEvent& event ){ event.Skip(); }
		virtual void EleccionCombo( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnContextMenuEntrada( wxListEvent& event ){ event.Skip(); }
		
	
	public:
		Buscador( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Buscador"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 695,444 ), long style = wxDEFAULT_FRAME_STYLE|wxSYSTEM_MENU );
		~Buscador();
		void m_splitter6OnIdle( wxIdleEvent& )
		{
		m_splitter6->SetSashPosition( 213 );
		m_splitter6->Disconnect( wxEVT_IDLE, wxIdleEventHandler( Buscador::m_splitter6OnIdle ), NULL, this );
		}
		
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Busqueda
///////////////////////////////////////////////////////////////////////////////
class Busqueda : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		wxTextCtrl* m_textCtrl1;
		wxChoice* m_choice1;
		wxButton* m_button4;
		wxListCtrl* m_listCtrl2;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
	
	public:
		Busqueda( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Resultado Busqueda"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 547,342 ), long style = wxDEFAULT_DIALOG_STYLE );
		~Busqueda();
	
};

#endif //__BuscarGUI__
