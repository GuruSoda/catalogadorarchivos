#include "ModeloCatalogo.h"

int32_t ModeloCatalogo::agregar_directorio(const string& directorio)
{
	int32_t id=0;
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("insert into directorio(id_directorio, nombre) values (next value for SEQ_ID_DIRECTORIO, ?) returning id_directorio;");
		st->Set(1, directorio);
		st->Execute();

        st->Get(1, id);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return id;
}

int32_t ModeloCatalogo::id_directorio(const string& directorio)
{
	voDirectorio d;

	d.id_directorio = 0;
/*
	if (opciones & contenedor_directorios) {
		d=data_directorio(directorio);
		if (!d.nombre.empty()) 
			return d.id_directorio;
	}
*/
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("select id_directorio, nombre, estado from directorio where nombre = ?;");
		st->Set(1, directorio);
		st->Execute();

		if (st->Fetch()) {
			st->Get(1, d.id_directorio);
			st->Get(2, d.nombre);
			st->Get(3, d.estado);
		}

		tr->Commit();

//		directorios.insert(d);
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return d.id_directorio;
}

int32_t	ModeloCatalogo::cargar_directorios() {
	iterator_fdb itfinal(*this);
	
	string c("select id_directorio, nombre, estado from directorio");

	directorios.clear();

	voDirectorio d;
	for (iterator_fdb it(*this, c);it!=itfinal;++it) {
		d.id_directorio=it->columna(1).como<int32_t>();
		d.nombre=it->columna(2).como<string>();
		d.estado=it->columna(3).como<int16_t>();

		directorios.insert(d);
	}

	return static_cast<int32_t>(directorios.size());
}

ModeloCatalogo::voDirectorio ModeloCatalogo::data_directorio(const string& nombre) {
	voDirectorio d;

	if (directorios.empty()) {
		if (cargar_directorios() == 0)
			return d;
	}

	d.nombre=nombre;

	set<voDirectorio>::iterator it(directorios.find(d));

	if (it != directorios.end()) return *it;

	return d;
}

int	ModeloCatalogo::cargar_etiquetas() {
	iterator_fdb itfinal(*this);
	
	string c("select id_etiqueta, nombre, directorio, filtro, opciones from etiqueta");

	etiquetas.clear();

	voEtiqueta e;
	for (iterator_fdb it(*this, c);it!=itfinal;++it) {
		e.id_etiqueta=it->columna(1).como<int16_t>();
		e.etiqueta=it->columna(2).como<string>();
		e.directorio=GuruDirectorio::comoDirectorioSinBarraFinal(it->columna(3).como<string>());
		e.filtro=it->columna(4).como<string>();
		e.opciones=it->columna(5).como<int16_t>();

		etiquetas.insert(e);
	}

	return static_cast<int>(etiquetas.size());
}

int32_t ModeloCatalogo::agregar_etiqueta(const string& etiqueta, const string& directorio, const string& filtro, int16_t opciones)
{
	int32_t id=0;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("insert into etiqueta (id_etiqueta, nombre, directorio, filtro, opciones) values (next value for SEQ_ID_ETIQUETA, ?, ?, ?, ?) returning id_etiqueta;");
		st->Set(1, etiqueta);
		st->Set(2, GuruDirectorio::comoDirectorioSinBarraFinal(directorio));
		st->Set(3, filtro);
		st->Set(4, opciones);
		st->Execute();

		st->Get(1, id);

		cargar_etiquetas();

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		throw string(e.ErrorMessage());
	};

	return id;
}

string ModeloCatalogo::directorio_etiqueta(const string& etiqueta)
{
	string directorio;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);
		st->Prepare("select directorio from etiqueta where nombre = ?;");
		st->Set(1, etiqueta);
		st->Execute();

		if (st->Fetch()) st->Get(1, directorio);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return string(directorio);
}

ModeloCatalogo::voEtiqueta ModeloCatalogo::data_etiqueta(const string& etiqueta) {

	voEtiqueta e;

	if (etiquetas.empty()) {
		if (cargar_etiquetas() == 0)
			return e;
	}

	e.etiqueta=etiqueta;

	set<voEtiqueta>::iterator it(etiquetas.find(e));

	if (it != etiquetas.end()) return *it;

	return e;
}

int32_t ModeloCatalogo::id_etiqueta(const string& etiqueta)
{
	int32_t id=0;
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);
		st->Prepare("select id_etiqueta from etiqueta where nombre = ?;");
		st->Set(1, etiqueta);
		st->Execute();

		if (st->Fetch()) st->Get(1, id);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return id;
}

int32_t	ModeloCatalogo::cargar_archivos() {
	iterator_fdb itfinal(*this);
	
	string c("select id_archivo, nombre, tamanio, fecha, md5, sha1, sha512, estado from archivo order by id_archivo");

	archivos.clear();

	voArchivo a;
	for (iterator_fdb it(*this, c);it!=itfinal;++it) {
		a.id_archivo=it->columna(1).como<int32_t>();
		a.nombre=it->columna(2).como<string>();
		a.bytes=it->columna(3).como<int64_t>();
		a.fecha=FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>());
		a.md5=it->columna(5).como<string>();
		a.sha1=it->columna(6).como<string>();
		a.sha512=it->columna(7).como<string>();
		a.estado=it->columna(8).como<int16_t>();

/* // por debug
		if (a.id_archivo == 855)
			a.id_archivo = a.id_archivo;

		std::pair<set<voArchivo>::iterator,bool> adentro;

		adentro=archivos.insert(a);
*/
		archivos.insert(a);
	}

	return static_cast<int32_t>(archivos.size());
}

ModeloCatalogo::voArchivo ModeloCatalogo::data_archivo(const string& nombre, uint64_t bytes, time_t fecha) {

	voArchivo a;

	if (archivos.empty()) {
		if (cargar_archivos() == 0)
			return a;
	}

	for (set<voArchivo>::iterator it=archivos.begin();it!=archivos.end();++it) {
		if (it->nombre.compare(nombre) == 0 && it->bytes == bytes && it->fecha == fecha)
			return *it;
	}

	return a;
}

int32_t ModeloCatalogo::id_archivo(const string& nombre, time_t fecha, uint64_t tamanio, const string& md5, const string& sha1, const string& sha512)
{
	int32_t id = 0;

	if (opciones & contenedor_archivos) {

	}

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("select id_archivo from archivo where nombre=? and tamanio=? and fecha=? and md5=? and sha1=? and sha512=?;");
		st->Set(1, nombre);
		st->Set(2, static_cast<int64_t>(tamanio));
		struct tm* _fecha = localtime(&fecha);
		st->Set(3, IBPP::Timestamp(_fecha->tm_year + 1900, _fecha->tm_mon + 1, _fecha->tm_mday, _fecha->tm_hour, _fecha->tm_min, _fecha->tm_sec));
		st->Set(4, (md5.empty() ? "0" : md5));
		st->Set(5, (sha1.empty() ? "0" : sha1));
		st->Set(6, (sha512.empty() ? "0" : sha512));

		st->Execute();

		if (st->Fetch()) st->Get(1, id);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return id;
}

int32_t ModeloCatalogo::agregar_archivo(const string& nombre, time_t fecha, uint64_t tamanio, const string& md5, const string& sha1, const string& sha512)
{
	int32_t id=0;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("insert into archivo(id_archivo, nombre, tamanio, md5, sha1, sha512, fecha) values(next value for SEQ_ID_ARCHIVO,?,?,?,?,?,?) returning id_archivo;");
		st->Set(1, nombre);
		st->Set(2, static_cast<int64_t>(tamanio));
		st->Set(3, (md5.empty() ? "0" : md5));
		st->Set(4, (sha1.empty() ? "0" : sha1));
		st->Set(5, (sha512.empty() ? "0" : sha512));
		
		struct tm* _fecha = localtime(&fecha);

		st->Set(6, IBPP::Timestamp(_fecha->tm_year + 1900, _fecha->tm_mon + 1, _fecha->tm_mday, _fecha->tm_hour, _fecha->tm_min, _fecha->tm_sec));
		st->Execute();

        st->Get(1, id);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
//		struct tm* _fecha = localtime(&fecha);
//		IBPP::Timestamp a(_fecha->tm_year + 1900, _fecha->tm_mon + 1, _fecha->tm_mday, _fecha->tm_hour, _fecha->tm_min, _fecha->tm_sec);
	};

	return id;
}

ModeloCatalogo::voArchivo ModeloCatalogo::data_catalogacion(const string& etiqueta, const string& directorio, const string& archivo)
{
	voArchivo a;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("select a.id_archivo, a.nombre, a.tamanio, a.fecha, a.md5, a.sha1, a.sha512, a.estado \
				from CATALOGO c, DIRECTORIO d, ARCHIVO a, etiqueta e \
				where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = d.ID_DIRECTORIO and c.ID_ETIQUETA = e.ID_ETIQUETA \
				and d.NOMBRE = ? and a.nombre = ? and e.nombre=?");
		st->Set(1, directorio);
		st->Set(2, archivo);
		st->Set(3, etiqueta);

		st->Execute();

		if (st->Fetch()) { 
			IBPP::Timestamp fecha;

			st->Get(1, a.id_archivo);
			st->Get(2, a.nombre);
			st->Get(3, a.bytes);
			st->Get(4, fecha);
			st->Get(5, a.md5);
			st->Get(6, a.sha1);
			st->Get(7, a.sha512);
			st->Get(8, a.estado);

			a.fecha = FDBFechaHora_time_t(fecha);
		}

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return a;
}

int32_t ModeloCatalogo::existe_catalogo(const string& etiqueta, const string& directorio, const string& archivo)
{
	int32_t id_catalogo = 0;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("select c.id_catalogo \
				from CATALOGO c, DIRECTORIO d, ARCHIVO a, etiqueta e \
				where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = d.ID_DIRECTORIO and c.ID_ETIQUETA = e.ID_ETIQUETA \
				and d.NOMBRE = ? and a.nombre = ?");
		st->Set(1, directorio);
		st->Set(2, archivo);

		st->Execute();

		if (st->Fetch()) st->Get(1, id_catalogo);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return id_catalogo;
}

int32_t ModeloCatalogo::existe_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo)
{
	int32_t id_catalogo=0;

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("select id_etiqueta from catalogo where id_etiqueta = ? and id_directorio = ? and id_archivo = ?;");
		st->Set(1, id_etiqueta);
		st->Set(2, id_directorio);
		st->Set(3, id_archivo);
		st->Execute();

		if (st->Fetch()) st->Get(1, id_catalogo);

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return id_catalogo;
}

ModeloCatalogo::voCatalogo ModeloCatalogo::data_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo) {

	voCatalogo c;

	c.id_catalogo = 0;
	c.estado = 0;
	c.agregado = 0;

	if (catalogo.empty()) {
		if (cargar_catalogo() == 0)
			return c;
	}

	for (set<voCatalogo>::iterator it=catalogo.begin();it!=catalogo.end();++it) {
		if (it->id_etiqueta == id_etiqueta && it->id_directorio == id_directorio && it->id_archivo == id_archivo)
			return *it;
	}

	return c;
}

bool ModeloCatalogo::existe_catalogo(const string& etiqueta, const string& directorio, const string& archivo, time_t fecha, uint64_t tamanio, const string& md5, const string& sha1, const string& sha512)
{
	int32_t id_eti;
	if ((id_eti = id_etiqueta(etiqueta))==0) return false;

	int32_t id_dir;
	if ((id_dir = id_directorio(directorio))==0) return false;

	int32_t id_arch;
	if ((id_arch=id_archivo(archivo, fecha, tamanio, md5, sha1, sha512))==0) return false;

	if (!existe_catalogo(id_eti, id_dir, id_arch)) return false;

	return true;
}

bool ModeloCatalogo::restaurar(const string& ruta_archivo, const string& etiqueta) {
	GuruDirectorio::GuruArchivo archivo_nuevo;
	if ((archivo_nuevo=GuruDirectorio::propiedades_archivo(ruta_archivo)).Existe()) {
		voEtiqueta e;		
		if ((e=data_etiqueta(etiqueta)).valido()) {
			GuruDirectorio::GuruSplitPath split(ruta_archivo);

			string directorio(split.drive() + split.dir());
			
			directorio.erase(0, e.directorio.length());

			if (directorio.empty()) directorio += DIVISOR_DIRECTORIO;

			e.etiqueta = etiqueta;

			voArchivo a;

			a.bytes = archivo_nuevo.sacar_tamanio();
			a.fecha = archivo_nuevo.sacar_fecha();
			a.nombre = split.nombre_ext();

			voArchivo b(data_catalogacion(etiqueta, directorio, split.nombre_ext()));

			if (!b.valido()) {
					
				int32_t id_dir=id_directorio(directorio);
				if (!id_dir) id_dir=agregar_directorio(directorio);

				int32_t id_arch=0;

				try {
					IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
					tr->Start();

					IBPP::Statement st = IBPP::StatementFactory(bd, tr);

					st->Prepare("select id_archivo from archivo where nombre=? and tamanio=? and fecha=?");
					st->Set(1, a.nombre);
					st->Set(2, static_cast<int64_t>(a.bytes));
					struct tm* _fecha = localtime(&a.fecha);
					st->Set(3, IBPP::Timestamp(_fecha->tm_year + 1900, _fecha->tm_mon + 1, _fecha->tm_mday, _fecha->tm_hour, _fecha->tm_min, _fecha->tm_sec));

					st->Execute();

					if (st->Fetch()) st->Get(1, id_arch);

					tr->Commit();
				}catch (IBPP::Exception& e)	{
					cerr << e.what() << endl;
					return false;
				}catch (string& e) {
					cout << e << endl;
					return false;
				};

					
				if (!id_arch) return false;

				try {
					IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
					tr->Start();

					IBPP::Statement st = IBPP::StatementFactory(bd, tr);

					st->Prepare("insert into catalogo(id_catalogo, id_etiqueta, id_directorio, id_archivo) values (next value for SEQ_ID_CATALOGO,?,?,?);");

					st->Set(1, e.id_etiqueta);
					st->Set(2, id_dir);
					st->Set(3, id_arch);

					st->Execute();

					tr->Commit();

					return true;
				}catch (IBPP::Exception& e)	{
					cerr << e.what() << endl;
					return false;
				}catch (string& e) {
					cerr << e << endl;
					return false;
				};
			}
		}
		
	} else return false;

	return true;
}

int64_t	ModeloCatalogo::cargar_catalogo() {
	iterator_fdb itfinal(*this);
	
	string c("select id_catalogo, id_etiqueta, id_directorio, id_archivo, agregado, estado from catalogo");

	catalogo.clear();

	voCatalogo cata;
	for (iterator_fdb it(*this, c);it!=itfinal;++it) {
		cata.id_catalogo=it->columna(1).como<int32_t>();
		cata.id_etiqueta=it->columna(2).como<int16_t>();
		cata.id_directorio=it->columna(3).como<int32_t>();
		cata.id_archivo=it->columna(4).como<int32_t>();
		cata.estado=it->columna(6).como<int16_t>();

		catalogo.insert(cata);
	}

	return static_cast<int64_t>(catalogo.size());
}

bool ModeloCatalogo::catalogar(const string& ruta_archivo, const string& etiqueta) {

	GuruDirectorio::GuruArchivo archivo_nuevo;
	if ((archivo_nuevo=GuruDirectorio::propiedades_archivo(ruta_archivo)).Existe()) {
		voEtiqueta e;		
		if ((e=data_etiqueta(etiqueta)).valido()) {
			GuruDirectorio::GuruSplitPath split(ruta_archivo);

			string directorio(split.drive() + split.dir());
			
			directorio.erase(0, e.directorio.length());

			if (directorio.empty()) directorio += DIVISOR_DIRECTORIO;

			e.etiqueta = etiqueta;

			voArchivo a;

			a.bytes = archivo_nuevo.sacar_tamanio();
			a.fecha = archivo_nuevo.sacar_fecha();

			if (e.opt_sha1()) a.sha1=GuruSHA1::archivo(ruta_archivo);
			if (e.opt_sha512()) a.sha512=GuruSHA4::archivo_sha512(ruta_archivo);
			if (e.opt_md5()) a.md5=GuruMD5::archivo(ruta_archivo);

			voArchivo b(data_catalogacion(etiqueta, directorio, split.nombre_ext()));

			if (b.valido()) {
				if (a != b)
					if (!actualizar_archivo(b.id_archivo, a.bytes, a.fecha, a.md5, a.sha1, a.sha512)) throw string("Error actualizando archivo.");
			} else {
					int32_t id_dir=id_directorio(directorio);
					int32_t id_arch=id_archivo(split.nombre_ext(), a.fecha, a.bytes, a.md5, a.sha1, a.sha512);

					if (!id_dir) id_dir=agregar_directorio(directorio);
					if (!id_arch) id_arch=agregar_archivo(split.nombre_ext(), a.fecha, a.bytes, a.md5, a.sha1, a.sha512);

					try {
						IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
						tr->Start();

						IBPP::Statement st = IBPP::StatementFactory(bd, tr);

						st->Prepare("insert into catalogo(id_catalogo, id_etiqueta, id_directorio, id_archivo, agregado) values (next value for SEQ_ID_CATALOGO,?,?,?, CURRENT_TIMESTAMP);");

						st->Set(1, e.id_etiqueta);
						st->Set(2, id_dir);
						st->Set(3, id_arch);

						st->Execute();

						tr->Commit();

						return true;
					}catch (IBPP::Exception& e)	{
						cerr << e.what() << endl;
						return false;
					}catch (string& e) {
						cout << e << endl;
						return false;
					};
			}
		}
		
	} else return false;

	return true;
}

void ModeloCatalogo::catalogar(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo)
{
	try {
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		st->Prepare("insert into catalogo(id_catalogo, id_etiqueta, id_directorio, id_archivo) values (next value for SEQ_ID_CATALOGO,?,?,?);");

		st->Set(1, id_etiqueta);
		st->Set(2, id_directorio);
		st->Set(3, id_archivo);

		st->Execute();

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		throw string(e.what());
	}catch (string& e) {
		throw e;
	};
}

void ModeloCatalogo::catalogar(const string& etiqueta, const string& directorio, const string& archivo, uint64_t tamanio, time_t fecha, const string& md5, const string& sha1, const string& sha512)
{
	try {
		int32_t id_eti=0;

		if ((id_eti=data_etiqueta(etiqueta).id_etiqueta)==0) throw string("Etiqueta no encontrada");

		int32_t id_dir;
		if ((id_dir = id_directorio(directorio))==0) id_dir = agregar_directorio(directorio);

		int32_t id_arch;
		if ((id_arch=id_archivo(archivo, fecha, tamanio, md5, sha1, sha512))==0) id_arch=agregar_archivo(archivo, fecha, tamanio, md5, sha1, sha512);

		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();

		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		if (!existe_catalogo(id_eti, id_dir, id_arch)) {
			st->Prepare("insert into catalogo(id_catalogo, id_etiqueta, id_directorio, id_archivo) values (next value for SEQ_ID_CATALOGO,?,?,?);");
			st->Set(1, id_eti);
			st->Set(2, id_dir);
			st->Set(3, id_arch);

			st->Execute();
		}

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		throw string(e.what());
	}catch (string& e) {
		throw e;
	};

}

void ModeloCatalogo::catalogar_directorio(const string& etiqueta, const string& directorio)
{

}

void ModeloCatalogo::borrar_etiqueta(const string& etiqueta)
{
	ostringstream oss;

	oss << "delete from etiqueta" << " where etiqueta.nombre=\'" << etiqueta << "\';";

	ejecutar(oss.str());
}

void ModeloCatalogo::borrar_directorio(const string& directorio)
{
	ostringstream oss;

	oss << "delete from directorio" << " where nombre=\'" << directorio << "\';";

	ejecutar(oss.str());
}

void ModeloCatalogo::borrar_archivo(const string& nombre, const string& fecha, const string& tamanio, const string& md5, const string& sha1, const string& sha512)
{
	string c("delete from archivo where");

	if (!nombre.empty()) c += " nombre=\'" + nombre + "\';";
	if (!fecha.empty()) c += " fecha=\'" + fecha + "\';";
	if (!tamanio.empty()) c += " tamanio=\'" + tamanio + "\';";
	if (!md5.empty()) c += " md5=\'" + md5 + "\';";
	if (!sha1.empty()) c += " sha1=\'" + sha1 + "\';";
	if (!sha512.empty()) c += " sha512=\'" + sha512 + "\';";

	ejecutar(c);
}

void ModeloCatalogo::borrar_archivo(int64_t id_archivo)
{
	string c("delete from archivo where id_archivo =");

	c += "\'";
	c += id_archivo + "\'";

	ejecutar(c);
}

void ModeloCatalogo::borrar_catalogo(const string& etiqueta, const string& directorio, const string& nombre)
{
}

bool ModeloCatalogo::borrar_catalogo(int16_t id_etiqueta, int32_t id_directorio, int32_t id_archivo)
{
	ostringstream oss;

	oss << "delete from catalogo where id_etiqueta=" << id_etiqueta << "and id_directorio=" << id_directorio << " and id_archivo=" << id_archivo << ";";

	return ejecutar(oss.str());
}

bool ModeloCatalogo::borrar_catalogo(int32_t id_catalogo)
{
	ostringstream oss;

	oss << "delete from catalogo where id_catalogo=" << id_catalogo << ";";

	return ejecutar(oss.str());
}

bool ModeloCatalogo::actualizar_archivo(int32_t id_archivo, uint64_t tamanio, time_t fecha, const string& md5, const string& sha1, const string& sha512)
{
	ostringstream oss;

	oss << "update archivo";
	oss << " set tamanio=" << tamanio;

	if (fecha != 0) {
		IBPP::Timestamp fechaFDB;
		FDBtime_t_FechaHora(fecha, fechaFDB);
		//formato: 2007-12-31 14:59
		oss << ", fecha=\'" << fechaFDB.Year() << "-" << fechaFDB.Month() << "-" << fechaFDB.Day() << " " << fechaFDB.Hours() << ":" << fechaFDB.Minutes() << ":" << fechaFDB.Seconds() << "\'";
	}

	if (md5.compare("0") != 0) oss << ", md5=\'" << md5 << "\'";
	if (sha1.compare("0") != 0) oss << ", sha1=\'" << sha1 << "\'";
	if (sha512.compare("0") != 0) oss << ", sha512=\'" << sha512 << "\'";

	oss << " where id_archivo=" << id_archivo;

	return ejecutar(oss.str());
}
/*
vector<voArchivo>& ModeloCatalogo::repetidos(vector<voArchivo>& archivos)
{
	archivos.clear();

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		tr->Start();
		st->Execute("select id_archivo,count(*) from CATALOGO \
			group by id_archivo \
			having count(*) > 1 \
			order by 2 DESC");

		int32_t	id;
		vector<int32_t> losid;
		while (st->Fetch()) {
			st->Get(1, id);			
			losid.push_back(id);
		}

		tr->Commit();

		for (vector<int32_t>::iterator it=losid.begin();it!=losid.end();++it){
			tr->Start();
			st->Prepare("select C.nombre,C.tamanio,C.md5,C.sha1,C.sha512,B.nombre from CATALOGO A,DIRECTORIO B,ARCHIVO C \
						where C.id_archivo = A.id_archivo and A.id_directorio=B.id_directorio and C.id_archivo=?");
			st->Set(1, *it);
			st->Execute();

			while (st->Fetch()) {
				voArchivo voa;

				st->Get(1, voa.nombre);
				st->Get(2, voa.tamanio);
				st->Get(3, voa.md5);
				st->Get(4, voa.sha1);
				st->Get(5, voa.sha512);
				st->Get(6, voa.directorio);
				
				voa.id_archivo = *it;
				archivos.push_back(voa);
			}

			tr->Commit();
		}

	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return archivos;
}

map<voArchivo, vector<voArchivo> >& ModeloCatalogo::iguales(map<voArchivo, vector<voArchivo> >& archivos)
{
	archivos.clear();

	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		tr->Start();
		st->Execute("select tamanio,md5,sha1,sha512,count(*) \
					from ARCHIVO \
					group by tamanio,md5,sha1,sha512 \
					having count(*) > 1 \
					order by 5 DESC");

		voArchivo voa;
		vector<voArchivo> losvoa;
		while (st->Fetch()) {
			st->Get(1, voa.tamanio);
			st->Get(2, voa.md5);
			st->Get(3, voa.sha1);
			st->Get(4, voa.sha512);

			losvoa.push_back(voa);
		}

		tr->Commit();

		for (vector<voArchivo>::iterator it=losvoa.begin();it!=losvoa.end();++it){
			tr->Start();
			st->Prepare("select C.nombre,C.tamanio,C.md5,C.sha1,C.sha512,B.nombre from CATALOGO A,DIRECTORIO B,ARCHIVO C \
						where C.id_archivo = A.id_archivo and A.id_directorio=B.id_directorio \
						and C.tamanio =? and C.md5=? and C.sha1=? and C.sha512=?");
			st->Set(1, it->tamanio);
			st->Set(2, it->md5);
			st->Set(3, it->sha1);
			st->Set(4, it->sha512);

			st->Execute();

			vector<voArchivo> igualesvoa;
			while (st->Fetch()) {
				voArchivo voa;

				st->Get(1, voa.nombre);
				st->Get(2, voa.tamanio);
				st->Get(3, voa.md5);
				st->Get(4, voa.sha1);
				st->Get(5, voa.sha512);
				st->Get(6, voa.directorio);

				igualesvoa.push_back(voa);
			}

			archivos.insert(make_pair(*it,igualesvoa));

			tr->Commit();
		}

	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return archivos;
}
*/

vector<pair<string, string> > & ModeloCatalogo::etiquetas_directorios(vector<pair<string, string> > & vector)
{
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		tr->Start();
		IBPP::Statement st = IBPP::StatementFactory(bd, tr);
		st->Execute("select nombre, directorio from etiqueta;");

		vector.clear();
		string nombre;
		string directorio;
		while (st->Fetch()) {
			st->Get(1, nombre);
			st->Get(2, directorio);
			vector.push_back(make_pair(nombre, directorio));
		}

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return vector;
}

/*
SELECT distinct(SUBSTRING(D.nombre from CHAR_LENGTH('/Chrissy Moran')+1))
FROM ARCHIVO A, CATALOGO B, ETIQUETA C, DIRECTORIO D
WHERE B.ID_ARCHIVO = A.ID_ARCHIVO AND B.ID_ETIQUETA = C.ID_ETIQUETA AND B.ID_DIRECTORIO = D.ID_DIRECTORIO and C.NOMBRE = 'GuruChuchis-003'
and D.NOMBRE like '/Chrissy Moran%'

*/
/*
vector<string>& ModeloCatalogo::subdirectorios(const string& etiqueta, const string& dirbase, vector<string>& subdirs)
{
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(pedir_conexion());
		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		tr->Start();

		cout << "Buscar=" << dirbase << endl;

		st->Prepare("select distinct(D.nombre) from directorio D, etiqueta E, catalogo C \
					where C.id_etiqueta = E.id_etiqueta and C.id_directorio = D.id_directorio and C.id_directorio = D.id_directorio \
					and E.nombre = ? and D.nombre like ?");

		st->Set(1, etiqueta);
		st->Set(2, dirbase);

		st->Execute();

//		if (dirbase.at(dirbase.size()) == '%') dirbase.erase(dirbase.size()-1, 1);

		subdirs.clear();
		string nombre;
		while (st->Fetch()) {
			st->Get(1, nombre);

			cout << "nombre=" << nombre << endl;
			nombre.erase(0, dirbase.size()-2);

			if (nombre.find("/", 1) == string::npos)
				subdirs.push_back(nombre);
		}

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return subdirs;
}
*/


/*
SELECT DISTINCT(CASE POSITION ('/' in SUBSTRING(D.nombre from CHAR_LENGTH('/Gina Lynn')+1))
							WHEN 0 THEN SUBSTRING(D.nombre from CHAR_LENGTH('/Gina Lynn')+1)
							ELSE SUBSTRING(SUBSTRING(D.nombre from CHAR_LENGTH('/Gina Lynn')+1) from 1 FOR POSITION ('/' in SUBSTRING(D.nombre from CHAR_LENGTH('/Gina Lynn')+1)))
							END)
					FROM ARCHIVO A, CATALOGO B, ETIQUETA C, DIRECTORIO D
					WHERE B.ID_ARCHIVO = A.ID_ARCHIVO AND B.ID_ETIQUETA = C.ID_ETIQUETA AND B.ID_DIRECTORIO = D.ID_DIRECTORIO and C.NOMBRE = 'GuruChuchis-003'
					and D.NOMBRE like '/Gina Lynn%'
*/
vector<string>& ModeloCatalogo::subdirectorios(const string& etiqueta, const string& dirbase, vector<string>& subdirs)
{
	try{
		IBPP::Transaction tr = IBPP::TransactionFactory(conexion());
		IBPP::Statement st = IBPP::StatementFactory(bd, tr);

		tr->Start();

		string dir(dirbase);

		if (dirbase.at(dirbase.length()-1) != '/') dir.append("/");

		ostringstream oss;

		oss << "SELECT DISTINCT(CASE POSITION ('/' in SUBSTRING(D.nombre from CHAR_LENGTH('" << dir << "')+1)) \
				WHEN 0 THEN SUBSTRING(D.nombre from CHAR_LENGTH('" << dir << "')+1) \
				ELSE SUBSTRING(SUBSTRING(D.nombre from CHAR_LENGTH('" << dir << "')+1) from 1 FOR POSITION ('/' in SUBSTRING(D.nombre from CHAR_LENGTH('" << dir << "')+1))-1) \
				END) \
		FROM ARCHIVO A, CATALOGO B, ETIQUETA C, DIRECTORIO D \
		WHERE B.ID_ARCHIVO = A.ID_ARCHIVO AND B.ID_ETIQUETA = C.ID_ETIQUETA AND B.ID_DIRECTORIO = D.ID_DIRECTORIO and C.NOMBRE = '" << etiqueta << "' \
		and D.NOMBRE like '" << dir << "%'";

		st->Prepare(oss.str());

		st->Execute();

		subdirs.clear();
		string nombre;
		while (st->Fetch()) {
			st->Get(1, nombre);

			subdirs.push_back(nombre);
		}

		tr->Commit();
	}catch (IBPP::Exception& e)	{
		std::cerr << e.ErrorMessage();
	};

	return subdirs;
}
