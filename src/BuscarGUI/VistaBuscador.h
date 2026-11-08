#ifndef __VistaBuscador__
#define __VistaBuscador__

#include "BuscarGUI.h"
#include "../Comun/ModeloCatalogo.h"

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>

#include <iostream>
#include <string>

using namespace std;

class itemEtiqueta: public wxTreeItemData
{
public:
	itemEtiqueta(const string& etiqueta, const string& directorio):_etiqueta(etiqueta),_directorio(directorio){};
	itemEtiqueta(){};

	string& directorio() {return _directorio;};
	string& etiqueta () {return _etiqueta;};
private:
	string _etiqueta;
	string _directorio;
};

class VistaBuscador : public Buscador
{
public:
	VistaBuscador( wxWindow* parent );
	~VistaBuscador( ) {if (mcatalogo != NULL) delete mcatalogo;};

	virtual void Terminar( wxCommandEvent& event);
	virtual void AbrirCatalogo(wxCommandEvent& event);
	virtual void crearArbol(const wxString& rutaCatalogo);
	virtual void MostrarSeleccion( wxTreeEvent& event );
	virtual void OnMenuContextualArbol( wxTreeEvent& event );
	virtual void MostrarDirectorios( wxTreeEvent& event );
	virtual void EleccionCombo( wxCommandEvent& event );
	virtual void OnContextMenuEntrada( wxListEvent& event );

	void cargarCatalogo(const wxString& rutacatalogo);
private:
	ModeloCatalogo* mcatalogo;
};

#endif
