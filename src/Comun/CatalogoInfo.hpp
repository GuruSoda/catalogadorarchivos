#ifndef __CATALOGOINFO_HPP__
#define __CATALOGOINFO_HPP__

#include <ModeloCatalogo.h>

using namespace std;

namespace CatalogoInfo {

	void dump_catalogo(const string& bdd, const string& etiqueta)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select d.directorio, b.NOMBRE, a.NOMBRE \
					from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    				and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<string>() << it->columna(2).como<string>() << it->columna(3).como<string>() << endl;
		}
	}

	void dump_etiquetas(const string& bdd)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);

		string c("select nombre from etiqueta;");

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<string>() << endl;
		}
	}

	void dump_directorios(const string& bdd, const string& etiqueta)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);

		string c("select distinct(b.NOMBRE) \
					from DIRECTORIO b, CATALOGO c, ETIQUETA d \
					where c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    				and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<string>() << endl;
		}
	}

	void dump_archivos(const string& bdd, const string& etiqueta)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select a.NOMBRE, a.fecha, a.tamanio \
					from ARCHIVO a, CATALOGO c, ETIQUETA d \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<string>() << " | " << it->columna(2).str() << " | " << it->columna(3).como<int64_t>() << endl;
		}
	}

}

#endif
