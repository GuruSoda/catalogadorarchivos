#include "VistaBuscador.h"

#include "icon1.xpm"
#include "icon2.xpm"
#include "icon3.xpm"
#include "icon4.xpm"
#include "icon5.xpm" 

VistaBuscador::VistaBuscador( wxWindow* parent ):Buscador(parent), mcatalogo(NULL)
{
	wxListItem item;
//	item.SetText(wxT("Directorio"));
//	EntradasEtiquetas->InsertColumn(0, item);
//	EntradasEtiquetas->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Nombre"));
	EntradasEtiquetas->InsertColumn(0, item);
	EntradasEtiquetas->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Tamaño"));
	EntradasEtiquetas->InsertColumn(1, item);
	EntradasEtiquetas->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Fecha"));
	EntradasEtiquetas->InsertColumn(2, item);
	EntradasEtiquetas->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Crc32"));
	EntradasEtiquetas->InsertColumn(3, item);
	EntradasEtiquetas->SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Sha1"));
	EntradasEtiquetas->InsertColumn(4, item);
	EntradasEtiquetas->SetColumnWidth(4, wxLIST_AUTOSIZE_USEHEADER);
	item.SetText(wxT("Sha512"));
	EntradasEtiquetas->InsertColumn(5, item);
	EntradasEtiquetas->SetColumnWidth(5, wxLIST_AUTOSIZE_USEHEADER);
}

void VistaBuscador::Terminar( wxCommandEvent& event )
{
	Close();
}

void VistaBuscador::AbrirCatalogo(wxCommandEvent& event)
{
	wxString caption = wxT("Elija el Catalogo");
	wxString wildcard = wxT("FDB files (*.fdb)|*.fdb");

	wxString defaultDir = wxT(".");
	wxString defaultFilename = wxEmptyString;
	wxFileDialog dialog(NULL, caption, defaultDir, defaultFilename, wildcard, wxOPEN);
	if (dialog.ShowModal() == wxID_OK) {
		cargarCatalogo(dialog.GetPath());
	}
}

void VistaBuscador::cargarCatalogo(const wxString& rutacatalogo)
{
	if (mcatalogo != NULL) delete mcatalogo;

	mcatalogo = new ModeloCatalogo(string(rutacatalogo.mb_str()));

    crearArbol(rutacatalogo);
}

void VistaBuscador::crearArbol(const wxString& rutaCatalogo)
{
	ArbolEtiquetas->DeleteAllItems();
	EntradasEtiquetas->DeleteAllItems();

//////////////////////////////////////////////////////////////////////////////
//	wxImageList *imagenes = new wxImageList(-1, -1, true);
	wxImageList *imagenes = new wxImageList(16, 16, true);
	wxIcon icons[4];
    icons[0] = wxIcon(icon3_xpm);   // closed
    icons[1] = wxIcon(icon3_xpm);   // closed, selected
    icons[2] = wxIcon(icon5_xpm);   // open
    icons[3] = wxIcon(icon5_xpm);   // open, selected 

	for (int i=0;i<4;i++) imagenes->Add(icons[i]);

	ArbolEtiquetas->AssignImageList(imagenes);
//////////////////////////////////////////////////////////////////////////////

	wxTreeItemId rootId = ArbolEtiquetas->AddRoot(rutaCatalogo, 1, 1);

	vector<string> etiquetas;
	mcatalogo->etiquetas(etiquetas);

	for (vector<string>::iterator it=etiquetas.begin();it!=etiquetas.end();++it){
		wxTreeItemId hijoid = ArbolEtiquetas->AppendItem(rootId, wxString (it->c_str(), wxConvUTF8), 1, 2, new itemEtiqueta(*it, "/"));
	}

	ArbolEtiquetas->Expand(rootId);
}

void VistaBuscador::MostrarDirectorios( wxTreeEvent& event )
{
	ArbolEtiquetas->SetItemImage(event.GetItem(), 2);

//	cout << "En mostrar Directorios .... " << ArbolEtiquetas->GetRootItem()<< endl;
}

void VistaBuscador::MostrarSeleccion( wxTreeEvent& event )
{
	itemEtiqueta* seleccion = (itemEtiqueta*)ArbolEtiquetas->GetItemData(event.GetItem());

	if (seleccion == NULL) return ;

	cout << "Seleccionado:" << seleccion->directorio() << endl;

	string stretiqueta, strsubdir;

	stretiqueta =  seleccion->etiqueta();
	strsubdir = seleccion->directorio();

	vector<string> subdirs;

	mcatalogo->subdirectorios(stretiqueta, strsubdir, subdirs);

	ArbolEtiquetas->DeleteChildren(event.GetItem());

	for (vector<string>::iterator it=subdirs.begin();it!=subdirs.end();++it){
		ArbolEtiquetas->AppendItem(event.GetItem(), wxString (it->c_str(), wxConvUTF8), 1, 2, new itemEtiqueta(stretiqueta, strsubdir + *it + "/"));
	}

	ArbolEtiquetas->Expand(event.GetItem());

/*
	wxTreeItemId elitem = event.GetItem();
	while (ArbolEtiquetas->GetItemParent(elitem) != ArbolEtiquetas->GetRootItem()){
		ArbolEtiquetas->SetItemImage(ArbolEtiquetas->GetItemParent(elitem), 2);
		elitem = ArbolEtiquetas->GetItemParent(elitem);
	}
*/

	EntradasEtiquetas->DeleteAllItems();

	barraEstado->SetStatusText(wxString (string("[" + stretiqueta + "]" + strsubdir).c_str(), wxConvUTF8));

	long indice=0;
	for (ModeloCatalogo::iterator it = mcatalogo->begin(stretiqueta, strsubdir);it!=mcatalogo->end(); ++it, indice++){
		EntradasEtiquetas->InsertItem(indice, -1);
//		EntradasEtiquetas->SetItem(indice, 0, wxString (it->directorio.c_str(), wxConvUTF8));
		EntradasEtiquetas->SetItem(indice, 0, wxString (it->nombre.c_str(), wxConvUTF8));
		wxString tmp; tmp << it->tamanio;
		EntradasEtiquetas->SetItem(indice, 1, tmp);
		wxDateTime fecha(it->fecha);
		EntradasEtiquetas->SetItem(indice, 2, fecha.Format());
		EntradasEtiquetas->SetItem(indice, 3, wxString (it->crc32.c_str(), wxConvUTF8));
		EntradasEtiquetas->SetItem(indice, 4, wxString (it->sha1.c_str(), wxConvUTF8));
		EntradasEtiquetas->SetItem(indice, 5, wxString (it->sha512.c_str(), wxConvUTF8));
	}
}

void VistaBuscador::OnMenuContextualArbol( wxTreeEvent& event )
{
	wxMenu* menu = new wxMenu();

	menu->Append(wxID_ANY, wxT("Primera opcion"));
	menu->Append(wxID_ANY, wxT("Segunda opcion"));
	menu->Append(wxID_ANY, wxT("Y la Tercera"));

	PopupMenu(menu);
}

void VistaBuscador::OnContextMenuEntrada( wxListEvent& event )
{
	wxMenu* menu = new wxMenu();

	menu->Append(wxID_ANY, wxT("No se 1"));
	menu->Append(wxID_ANY, wxT("No se 2"));
	menu->AppendSeparator();
	menu->Append(wxID_ANY, wxT("No se 3"));

	PopupMenu(menu);
}

void VistaBuscador::EleccionCombo( wxCommandEvent& event )
{
	event.GetString();
	cout << string(event.GetString().mb_str()) << endl;
	cargarCatalogo(event.GetString());
}
