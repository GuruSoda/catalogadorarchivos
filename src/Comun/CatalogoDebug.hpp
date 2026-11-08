#ifndef __CATALOGODEBUG_HPP__
#define __CATALOGODEBUG_HPP__

#include <fstream>

#include <ModeloCatalogo.h>
#include <GuruDirectorio.hpp>
#include <GuruBuffer.hpp>

using namespace std;

namespace CatalogoDebug {

	void dump_tabla_etiquetas(const string& bdd)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);

		string c("select id_etiqueta, nombre, directorio from etiqueta;");

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<int32_t>() << " | " << it->columna(2).como<string>() << " | " << it->columna(3).como<string>() << endl;
		}
	}

	void dump_tabla_directorios(const string& bdd)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);

		string c("select id_directorio, nombre from DIRECTORIO;");

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<int32_t>() << " | " << it->columna(2).como<string>() << endl;
		}
	}

	void dump_tabla_archivos(const string& bdd)
	{
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select id_archivo, nombre, tamanio, fecha, md5, sha1, sha512 from archivo;");

		try {
		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			cout << it->columna(1).como<int32_t>() << " | " << it->columna(2).como<string>() << " | " << it->columna(3).como<int64_t>() << " | " << it->columna(4).str() << " | " << it->columna(5).como<string>() << " | " << it->columna(6).como<string>() << " | " << it->columna(7).como<string>() << endl;
		}
		} catch (const string& e) {cout << e << endl;}
	}

	void consulta(const string& bdd, const string& query)
	{
		try {
			ModeloCatalogo modelo(bdd);
			iterator_fdb it(modelo, query);

			for (unsigned int i=1;i<=it->columnas();i++)
				cout << it->columna(i).nombre() << ((i<it->columnas()) ? " | " : "");

			cout << endl;

			iterator_fdb itfinal(modelo);
			for (;it!=itfinal;++it){
				for (unsigned int i=1;i<=it->columnas();i++)
					cout << it->columna(i).str() << ((i<it->columnas()) ? " | " : "");
				cout << endl;
			}
		} catch (string e) {cout << e;}
	}

	template <typename T>
	T formato_fdb(const T& cadena) {
		T comandosql(cadena);
		string::size_type lugar=0;
		while ((lugar=comandosql.find("\'", lugar)) != string::npos) {
			comandosql.insert(lugar, 1, '\'');
			lugar+=2;
		}

		return comandosql;
	}

	void exportar (const string& bdd)
	{
		ModeloCatalogo modelo(bdd);

		iterator_fdb itfinal(modelo);
		try {
			for (iterator_fdb it(modelo, "select id_etiqueta, nombre, directorio, filtro from etiqueta;");it!=itfinal;++it)
				cout << "insert into etiqueta (id_etiqueta, nombre, directorio, filtro) values (" <<  it->columna(1).como<int16_t>() << ",\'" << formato_fdb(it->columna(2).str()) << "\',\'" << formato_fdb(it->columna(3).str()) << "\',\'" << formato_fdb(it->columna(4).str()) << "\') " << endl;

			for (iterator_fdb it(modelo, "select id_directorio, nombre, estado from directorio;");it!=itfinal;++it)
				cout << "insert into directorio (id_directorio, nombre, estado) values (" <<  it->columna(1).como<int32_t>() << ",\'" <<  formato_fdb(it->columna(2).str()) << "\'," << it->columna(3).como<int16_t>() << ")" << endl;

			for (iterator_fdb it(modelo, "select id_archivo, nombre, tamanio, fecha, md5, sha1, sha512, estado from archivo");it!=itfinal;++it)
				cout << "insert into archivo (id_archivo, nombre, tamanio, fecha, md5, sha1, sha512, estado) values (" << it->columna(1).como<int32_t>() << ",\'" << formato_fdb(it->columna(2).str()) << "\'," << it->columna(3).como<int64_t>() << ",\'" << it->columna(4).str() << "\',\'" << it->columna(5).str() << "\',\'" << it->columna(6).str() << "\',\'" << it->columna(7).str() << "\'," << it->columna(8).como<int16_t>() << ")" << endl;

			for (iterator_fdb it(modelo, "select id_catalogo, id_etiqueta, id_directorio, id_archivo, agregado, estado from catalogo;");it!=itfinal;++it)
				cout << "insert into catalogo (id_catalogo, id_etiqueta, id_directorio, id_archivo, agregado, estado) values (" << it->columna(1).como<int32_t>() << "," << it->columna(2).como<int16_t>() << "," << it->columna(3).como<int32_t>() << "," << it->columna(4).como<int32_t>() << ",\'" <<  it->columna(5).str() << "\'," << it->columna(6).como<int16_t>() << ")" << endl;

			for (iterator_fdb it(modelo, "select next value for seq_id_etiqueta from RDB$DATABASE;");it!=itfinal;++it)
				cout << "alter sequence seq_id_etiqueta restart with " << it->columna(1).str() << endl;

			for (iterator_fdb it(modelo, "select next value for seq_id_directorio from rdb$database;");it!=itfinal;++it)
				cout << "alter sequence seq_id_directorio restart with " << it->columna(1).str() << endl;

			for (iterator_fdb it(modelo, "select next value for seq_id_archivo from rdb$database;");it!=itfinal;++it)
				cout << "alter sequence seq_id_archivo restart with " << it->columna(1).str() << endl;

		} catch (const string& e) {cout << e << endl;}
	}

	void importar (const string& bdd, const string& sql)
	{
		if (!GuruDirectorio::propiedades_archivo(sql).Existe()) {
			cout << "No existe archivo " << sql << endl;
			return ;
		}

		ModeloCatalogo modelo(bdd);
		modelo.crear_catalogo();

		ifstream ifs(sql.c_str(), std::ios::in);

		if (!ifs) return ;

		char linea[1024];

		while (ifs.getline(linea, 1024)) {
			string comandosql(linea);
			if (comandosql.empty()) continue;

			modelo.ejecutar(comandosql);

		}

		ifs.close();
	}

}

#endif
