#ifndef __MODELOCATALOGO_H__
#define __MODELOCATALOGO_H__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>

#include <ibpp/ibpp.h>
#include <GuruCompat.h>
#include <GuruID.hpp>
#include <GuruFDBBase.h>
#include <GuruDirectorio.hpp>
#include <GuruSHA1.hpp>
#include <GuruSHA4.hpp>
#include <GuruMD5.hpp>

#include "ddl.h"

using namespace std;

class ModeloCatalogo: public GuruFDBBase
{
public:
	class voArchivo {
	public:
		::int32_t	id_archivo;
		string		nombre;
		::int64_t	bytes;
		time_t		fecha;
		string		md5;
		string		sha1;
		string		sha512;
		::int16_t	estado;

		friend bool operator < (const ModeloCatalogo::voArchivo& a, const ModeloCatalogo::voArchivo& b){
			return (a.id_archivo < b.id_archivo) ? true : false;
		}
/*
		friend bool operator == (const voArchivo& a, const voArchivo& b) {
			return (a.nombre.compare(b.nombre) && a.bytes==b.bytes && a.fecha==b.fecha && GuruStringAlgo::stringequals(a.md5, b.md5) && GuruStringAlgo::stringequals(a.sha1, b.sha1) && GuruStringAlgo::stringequals(a.sha512,b.sha512));
		}
*/
		friend bool operator == (const ModeloCatalogo::voArchivo& a, const ModeloCatalogo::voArchivo& b) {
			if (a.nombre.compare(b.nombre) == 0) {
				if (a.bytes == b.bytes) {
					if (a.fecha == b.fecha) {
						return true;
					}
				}
			}

			return false;
		}

		friend bool operator != (const ModeloCatalogo::voArchivo& a, const ModeloCatalogo::voArchivo& b) {
			return (a.nombre.compare(b.nombre) && a.bytes==b.bytes && a.fecha==b.fecha && GuruStringAlgo::stringequals(a.md5, b.md5) && GuruStringAlgo::stringequals(a.sha1, b.sha1) && GuruStringAlgo::stringequals(a.sha512,b.sha512));
		}

		struct ordenar_nombre {
			bool operator () (const ModeloCatalogo::voArchivo& a, const ModeloCatalogo::voArchivo& b) {
				return (a.nombre.compare(b.nombre) < 0) ? true : false;
			}
		};

		bool valido () {
			return !nombre.empty();
		}

	};

	struct voEtiqueta {
	public:
		::int32_t	id_etiqueta;
		string		etiqueta;
		string		directorio;
		string		filtro;
		::int16_t	opciones;

		friend bool operator < (const struct voEtiqueta& a, const struct voEtiqueta& b) {
			return a.etiqueta < b.etiqueta;
		}

		bool opt_sha1 () { return (opciones & ModeloCatalogo::sha1) ? true : false; }
		bool opt_sha512 () { return (opciones & ModeloCatalogo::sha512) ? true : false; }
		bool opt_md5 () { return (opciones & ModeloCatalogo::md5) ? true : false; }

		bool valido() {
			if (id_etiqueta == 0 && etiqueta.empty() && directorio.empty()) return false;

			return true;
		}
	};

	struct voDirectorio {
	public:
		::int32_t	id_directorio;
		string		nombre;
		int16_t		estado;

		friend bool operator < (const struct voDirectorio& a, const struct voDirectorio& b){
    		return (a.nombre.compare(b.nombre) < 0) ? true : false;
		}

		bool valido() {
			if (nombre.empty()) return false;

			return true;
		}

	};

	struct voCatalogo {
	public:
		int32_t id_catalogo;
		int16_t id_etiqueta;
		int32_t id_directorio;
		int32_t id_archivo;
		time_t	agregado;
		int16_t	estado;

		friend bool operator < (const struct voCatalogo& a, const struct voCatalogo& b){
    		return (a.id_catalogo < b.id_catalogo) ? true : false;
		}

		friend bool operator == (const struct voCatalogo& a, const struct voCatalogo& b){
    		return (a.id_catalogo == b.id_catalogo) ? true : false;
		}

		bool valido() {
			if (id_catalogo == 0) return false;

			return true;
		}

	};


public:
	ModeloCatalogo(const string& archivo_bd):GuruFDBBase(archivo_bd) {};
	~ModeloCatalogo(void){};

	enum catalogacion {	sha1=0x01,		// que catalogo el sha1 de cada archivo
			sha512=0x02,	// que catalogo el sha512 de cada archivo
			md5=0x04,		// que catalogo el md5 de cada archivo
			nocrear=0x08	// no crear la base de datos si no existe
	};

	enum opciones {
			contenedor_etiquetas=0x01,		// carga toda la tabla de etiquetas a memoria
			contenedor_directorios=0x02,	// carga toda la tabla de directorios a memoria
			contenedor_archivos=0x04		// carga toda la tabla de archivos a memoria
	};

	void crear_catalogo() {
		DDL;
		crear_bdd(catalogo);
	}

	void		configurar(ModeloCatalogo::opciones opt) {
		opciones |= opt;
	}

	int32_t		id_directorio(const string& directorio);
	int32_t		agregar_directorio(const string& directorio);
	int32_t		cargar_directorios();
	ModeloCatalogo::voDirectorio data_directorio(const string& nombre);

	int			cargar_etiquetas();
	voEtiqueta	data_etiqueta(const string& etiqueta);
	int32_t		id_etiqueta(const string& etiqueta);
	int32_t		agregar_etiqueta(const string& etiqueta, const string& directorio, const string& filtro, int16_t opciones);
	string		directorio_etiqueta(const string& etiqueta);
	vector<pair<string, string> >& etiquetas_directorios(vector<pair<string, string> >&);

	voArchivo	data_catalogacion(const string& etiqueta, const string& directorio, const string& archivo);
	voArchivo	data_archivo(const string& nombre, uint64_t bytes, time_t fecha);
	int32_t		cargar_archivos();
	int32_t		id_archivo(const string& nombre, time_t fecha, uint64_t tamanio, const string& md5="0", const string& sha1="0", const string& sha512="0");
	int32_t		agregar_archivo(const string& nombre, time_t fecha, uint64_t tamanio, const string& md5, const string& sha1, const string& sha512);

	int32_t existe_catalogo(const string& etiqueta, const string& directorio, const string& archivo);
	int32_t existe_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo);
	bool existe_catalogo(const string& etiqueta, const string& directorio, const string& archivo, time_t fecha, uint64_t tamanio, const string& md5="0", const string& sha1="0", const string& sha512="0");

	bool restaurar(const string& ruta_archivo, const string& etiqueta);
	bool catalogar(const string& ruta_archivo, const string& etiqueta);
	void catalogar(const string& etiqueta, const string& directorio, const string& archivo, uint64_t tamanio = 0, time_t fecha = 0, const string& md5 = "0", const string& sha1 = "0", const string& sha512 = "0");
	void catalogar(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo);
	ModeloCatalogo::voCatalogo data_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo);
	void catalogar_directorio(const string& etiqueta, const string& directorio);
	int64_t		cargar_catalogo();

	void borrar_etiqueta(const string& etiqueta);
	void borrar_directorio(const string& directorio);
	void borrar_archivo(const string& nombre, const string& fecha, const string& tamanio, const string& md5, const string& sha1, const string& sha512);
	void borrar_archivo(int64_t id_archivo);
	void borrar_catalogo(const string& etiqueta, const string& directorio, const string& nombre);
	bool borrar_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo);
	bool borrar_catalogo(int32_t id_catalogo);

	bool actualizar_archivo(int32_t id_archivo, uint64_t tamanio, time_t fecha = 0, const string& md5 = "0", const string& sha1 = "0", const string& sha512 = "0");

	vector<string>& subdirectorios(const string& etiqueta, const string& dirbase, vector<string>& subdirs);

	// archivos con el mismo nombre, tamanio, md5, sha1 y sha512
	vector<voArchivo>& repetidos(vector<voArchivo>& archivos);

	// archivo con el mismo tamanio, md5, sha1 y sha512
	map<voArchivo, vector<voArchivo> >& iguales(map<voArchivo, vector<voArchivo> >& archivos);
/*
	class iterator;
	friend class iterator;
	class iterator
	{
		ModeloCatalogo& catalogo;
		string			_etiqueta;
		voArchivo		registro;

		IBPP::Database		bd;
		IBPP::Transaction	tr;
		IBPP::Statement		st;

		bool				elfinal;
	public:
		iterator(ModeloCatalogo& modcat, const string& etiqueta = "", const string& directorio = "", const string& nombre = "", const string& tamanio = "", const string& fecha = "", const string& md5 = "", const string& sha1 = "", const string& sha512 = ""): catalogo(modcat), _etiqueta(etiqueta), elfinal(false) { inicio(_etiqueta, directorio, nombre, tamanio, fecha, md5, sha1, sha512);}
		iterator(ModeloCatalogo& modcat, bool final): catalogo(modcat), elfinal(final) {}
		iterator(ModeloCatalogo& modcat): catalogo(modcat) {}

		void inicio(const string& etiqueta = "", const string& directorio = "", const string& nombre = "", const string& tamanio = "", const string& fecha = "", const string& md5 = "", const string& sha1 = "", const string& sha512 = "") {
			try{
				tr = IBPP::TransactionFactory(catalogo.conexion());
				tr->Start();
				st = IBPP::StatementFactory(catalogo.bd, tr);

				string consulta;
				int indice = 1;

				consulta = "SELECT C.nombre, A.id_archivo, A.nombre, A.tamanio, A.md5, A.sha1, A.sha512, A.fecha, D.nombre \
FROM ARCHIVO A, CATALOGO B, ETIQUETA C, DIRECTORIO D \
WHERE B.ID_ARCHIVO = A.ID_ARCHIVO \
AND B.ID_ETIQUETA = C.ID_ETIQUETA \
AND B.ID_DIRECTORIO = D.ID_DIRECTORIO";

				if (!etiqueta.empty()) consulta += " and C.NOMBRE like ? ";
				if (!directorio.empty()) consulta += " and D.NOMBRE like ? ";
				if (!nombre.empty()) consulta += " and A.NOMBRE like ? ";
				if (!tamanio.empty()) consulta += " and A.TAMANIO = ? ";
				if (!fecha.empty()) consulta += " and A.FECHA = ? ";
				if (!md5.empty()) consulta += " and A.md5 = ? ";
				if (!sha1.empty()) consulta += " and A.SHA1 = ? ";
				if (!sha512.empty()) consulta += " and A.SHA512 = ? ";

				st->Prepare(consulta);

				if (!etiqueta.empty()) st->Set(indice++, etiqueta);
				if (!directorio.empty()){
					if (directorio.at(directorio.length()-1) == '/') {
						string tmp(directorio);
						tmp.erase(directorio.length()-1);
						st->Set(indice++, tmp);
					} else
						st->Set(indice++, directorio);
				}
				if (!nombre.empty()) st->Set(indice++, nombre);
				if (!tamanio.empty()) st->Set(indice++, (int32_t)atol(tamanio.c_str()));
				if (!fecha.empty()) st->Set(indice++, fecha);
				if (!md5.empty()) st->Set(indice++, md5);
				if (!sha1.empty()) st->Set(indice++, sha1);
				if (!sha512.empty()) st->Set(indice++, sha512);

				st->Execute();

				incrementar();
		//		if (!st->Fetch()) elfinal = true;
			}catch (IBPP::Exception& e)	{
				std::cerr << e.ErrorMessage();
			};
			}

		void fin() {
			tr->Commit();
		}

		void incrementar(){
			if (st->Fetch()) {
				IBPP::Timestamp fecha;

				st->Get(1, registro.etiqueta);
				st->Get(2, registro.id_archivo);
				st->Get(3, registro.nombre);
				st->Get(4, registro.tamanio);
				st->Get(5, registro.md5);
				st->Get(6, registro.sha1);
				st->Get(7, registro.sha512);
				st->Get(8, fecha);
				st->Get(9, registro.directorio);

				struct tm timeinfo = {0};
				fecha.GetDate(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);
				fecha.GetTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

//				cout << timeinfo.tm_year << "/" << timeinfo.tm_mon << "/" << timeinfo.tm_mday;
//				cout << " - " << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << endl;

				// acomodo los miembros para mktime
				timeinfo.tm_mon--;
				timeinfo.tm_year -= 1900;

				registro.fecha = mktime(&timeinfo);

			} else {elfinal = true;fin();}
		}

		void operator++() {
			incrementar();
		}

//		iterator& operator=(iterator& otro){return *this;}

		bool operator!=(const iterator& it2) const {
			if (this->elfinal != it2.elfinal) return true;
			else return false;
		}

		voArchivo* operator->() const {
			return (voArchivo*)&registro;
		}
	};

	iterator begin(const string& etiqueta = "", const string& directorio = "", const string& nombre = "", const string& tamanio = "", const string& fecha = "", const string& md5 = "", const string& sha1 = "", const string& sha512 = "") {
		return iterator(*this, etiqueta, directorio, nombre, tamanio, fecha, md5, sha1, sha512);
		}

	iterator end() { return iterator(*this, true);}
*/
protected:
	set<voEtiqueta>		etiquetas;
	set<voArchivo>		archivos;
	set<voDirectorio>	directorios;
	set<voCatalogo>		catalogo;

	uint16_t			opciones;
};

#endif
