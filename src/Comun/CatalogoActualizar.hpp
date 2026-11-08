#ifndef __CATALOGOACTUALIZAR_HPP__
#define __CATALOGOACTUALIZAR_HPP__

#include <ModeloCatalogo.h>
#include <GuruDirectorio.hpp>

#include <set>

namespace CatalogoActualizar {

	struct parametros_actualizar {
		enum { solomostrar=0x01,	// solo muestra los archivos a depurar del catalogo
			solocatalogo=0x02,		// solo verifica la base de datos contra el directorio.
			solodirectorio=0x04,	// solo verifica el directorio contra el catalogo.
			simple=0x08,			// Para saber si un archivo esta catalogado solo pregunta por nombre,tamanio y fecha, no por md5/sha1/sha512
			completo=0x10,			// Para saber si un archivo esta catalogado o no tambien pregunta por su hash
			cache_dir=0x20,			// Carga la tabla de directorios en memoria
			cache_archivo=0x40,		// Carga la tabla de archivos en memoria
			restaurar=0x80,			// Solo Verifica por nombre, tamanio y fecha en la tabla archivo y lo agrega la tabla catalogo
			regenerar=0x100			// Regenera el catalogo en base a la informacion almacenada, no creao nuevos directorios
		};
	public:
		parametros_actualizar(uint32_t opciones=!solomostrar|!solocatalogo|!solodirectorio|simple|!restaurar):opciones_actualizar(opciones) {};
		uint32_t 	opciones_actualizar;
	};

	uint32_t tablaDirectorio_contenedor(set<ModeloCatalogo::voDirectorio> dir, const string& bdd, struct parametros_actualizar opciones=parametros_actualizar::simple) {

		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select d.id_directorio, d.nombre, d.estado from DIRECTORIO b");

		dir.clear();

		ModeloCatalogo::voDirectorio a;
		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			a.id_directorio= it->columna(1).como<int32_t>();
			a.nombre = it->columna(1).como<string>();
			a.estado = it->columna(1).como<int16_t>();

			dir.insert(a);
		}

		return dir.size();
	}

	uint32_t tablaArchivoCatalogo_contenedor(set<ModeloCatalogo::voArchivo> dir, const string& bdd, const string& etiqueta="", struct parametros_actualizar opciones=parametros_actualizar::simple) {
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("	select c.ID_CATALOGO, d.ID_DIRECTORIO, a.NOMBRE, a.TAMANIO, a.FECHA \
					from ARCHIVO a, DIRECTORIO d, CATALOGO c, ETIQUETA e \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = d.ID_DIRECTORIO and c.ID_ETIQUETA = e.ID_ETIQUETA \
				");

		if (!etiqueta.empty()) {
			c += " and e.nombre=\'" + etiqueta + "\'";
		}

		ModeloCatalogo::voArchivo a;
		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			a.nombre = it->columna(1).como<string>() + it->columna(2).como<string>() + it->columna(3).como<string>();
			a.fecha = FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>());
			a.bytes = it->columna(5).como<int64_t>();
			//a.id_catalogo=it->columna(6).como<int32_t>();

			dir.insert(a);
		}

		return dir.size();
	}

	void actualizar_etiqueta(const string& bdd, const string& etiqueta, const string& nueva_etiqueta) {
		ModeloCatalogo modelo(bdd);

		ostringstream oss;

		oss << "update etiqueta set nombre=\'" << nueva_etiqueta << "\' where nombre=\'" << etiqueta << "\'" ;

		modelo.ejecutar(oss.str());
	}

	void actualizar_directorio(const string& bdd, const string& etiqueta, const string& directorio) {
		ModeloCatalogo modelo(bdd);

		ostringstream oss;

		oss << "update etiqueta set directorio=\'" << GuruDirectorio::comoDirectorioSinBarraFinal(directorio) << "\' where nombre=\'" << etiqueta << "\'" ;

		modelo.ejecutar(oss.str());
	}

	void actualizar_filtro(const string& bdd, const string& etiqueta, const string& filtro) {
		ModeloCatalogo modelo(bdd);

		ostringstream oss;

		oss << "update etiqueta set filtro=\'" << filtro << "\' where nombre=\'" << etiqueta << "\'" ;

		modelo.ejecutar(oss.str());
	}

	void contrastar_catalogo(struct parametros_actualizar& opciones, const string& bdd, const string& etiqueta) {
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("	select e.directorio, d.NOMBRE, a.NOMBRE, a.md5, a.sha1, a.sha512, a.id_archivo, a.tamanio, a.fecha \
					from ARCHIVO a, DIRECTORIO d, CATALOGO c, ETIQUETA e \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = d.ID_DIRECTORIO and c.ID_ETIQUETA = e.ID_ETIQUETA \
					and (a.MD5 = '0' or a.SHA1 = '0' or a.SHA512 = '0') ");

		if (!etiqueta.empty()) {
			c += " and e.nombre=\'" + etiqueta + "\'";
		}

		ModeloCatalogo::voEtiqueta e(modelo.data_etiqueta(etiqueta));
		ModeloCatalogo::voArchivo a;

		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			a.nombre = it->columna(1).como<string>() + it->columna(2).como<string>() + it->columna(3).como<string>();
			a.md5=it->columna(4).como<string>();
			a.sha1=it->columna(5).como<string>();
			a.sha512=it->columna(6).como<string>();
			a.id_archivo=it->columna(7).como<int32_t>();
			a.bytes=it->columna(8).como<int64_t>();
			a.fecha=FDBFechaHora_time_t(it->columna(9).como<IBPP::Timestamp>());
			
			if (e.opt_md5() && a.md5.compare("0") == 0) {
				if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) cout << "Actualizar md5:" << a.nombre << endl;
				else { 
					cout << "* md5:" << a.nombre << endl;
					a.md5=GuruMD5::archivo(a.nombre);
				}
			}

			if (e.opt_sha1() && a.sha1.compare("0") == 0) {
				if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) cout << "Actualizar sha1:" << a.nombre << endl;
				else {
					cout << "* sha1:" << a.nombre << endl;
					a.sha1=GuruSHA1::archivo(a.nombre);
				}
			}

			if (e.opt_sha512() && a.sha512.compare("0") == 0) {
				if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) cout << "Actualizar sha512:" << a.nombre << endl;
				else {
					cout << "* sha512:" << a.nombre << endl;
					a.sha512=GuruSHA4::archivo_sha512(a.nombre);
				}
			}

			if (!(opciones.opciones_actualizar & parametros_actualizar::solomostrar))
				modelo.actualizar_archivo(a.id_archivo, a.bytes, a.fecha, a.md5, a.sha1, a.sha512);
		}

	}

	int actualizar_catalogo(struct parametros_actualizar& opciones, const string& bdd, const string& etiqueta) {
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select d.directorio, b.NOMBRE, a.NOMBRE, a.fecha, a.tamanio ,d.id_etiqueta, b.id_directorio, a.id_archivo, a.md5, a.sha1, a.sha512 \
					from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    				and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		if (opciones.opciones_actualizar & parametros_actualizar::solocatalogo) {
			// Primero comparo la base de datos contra el sistema de archivos
			cout << "B->D" << endl;
			string full_path;
			for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
				full_path= it->columna(1).como<string>() + it->columna(2).como<string>() + it->columna(3).como<string>();
	
				GuruDirectorio::GuruArchivo archivo(GuruDirectorio::propiedades_archivo(full_path));

				// pregunto si NO existe el archivo
				if (!archivo.Existe()) {
					if (opciones.opciones_actualizar & parametros_actualizar::solomostrar)
						cout << "Faltante: " << full_path << endl;
					else if (modelo.borrar_catalogo(it->columna(6).como<int16_t>(), it->columna(7).como<int32_t>(), it->columna(8).como<int32_t>()))
							cout << "- " << full_path << endl;
				} else { // el archivo existe
					try {
						if (archivo.sacar_tamanio() != static_cast<uint64_t>(it->columna(5).como<int64_t>())) throw 0;
						if (archivo.sacar_fecha() != FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>())) throw 1;
		/*
						if (it->columna(9).como<string>().compare("0") != 0) throw 2;
						if (it->columna(10).como<string>().compare("0") != 0) throw 3;
						if (it->columna(11).como<string>().compare("0") != ) throw 4;
		*/
					} catch (int e) {
						if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) {
							cout << "Actualizar: " << full_path;
							switch (e) {
							case 0: cout << " (T)" << endl;
									break;
							case 1: cout << " (F)" << fechaHoraRFC822(archivo.sacar_fecha()) << " != " << fechaHoraRFC822(FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>())) << endl;
									break;
							}
						} else if (modelo.actualizar_archivo(it->columna(8).como<int32_t>(), archivo.sacar_tamanio(), archivo.sacar_fecha()))
								cout << "* " << full_path << endl;
					};
				}
			}

		}

		if (opciones.opciones_actualizar & parametros_actualizar::solodirectorio) {
			// Segundo comparo el sistema de archivos con la base de datos
			cout << "D->B" << endl;
			GuruDirectorio::iterator_recursivo itfin;

			string directorio;
			vector<pair<string, string> > etiquetas_directorios;

			if (!etiqueta.empty()) {
				directorio = modelo.directorio_etiqueta(etiqueta);
				etiquetas_directorios.push_back(make_pair(etiqueta, directorio));
			} else {
				modelo.etiquetas_directorios(etiquetas_directorios);
			}

			for (vector<pair<string, string> >::iterator it=etiquetas_directorios.begin();it!=etiquetas_directorios.end();++it) {
				directorio=it->second;
				string etiqueta=it->first;

				int largo_directorio=directorio.length();

				for (GuruDirectorio::iterator_recursivo it(GuruDirectorio::comoDirectorio(directorio)); it!=itfin;++it) {
		/*
					sha1.clear();sha512.clear();md5.clear();
					if (opciones.opciones_catalogar & parametros_catalogar::sha1) sha1 = GuruSHA1::archivo(it.dir() + it->sacar_nombre());
					if (opciones.opciones_catalogar & parametros_catalogar::sha512)  sha512 = GuruSHA4::archivo_sha512(it.dir() + it->sacar_nombre());
					if (opciones.opciones_catalogar & parametros_catalogar::md5) md5 = GuruMD5::archivo(it.dir() + it->sacar_nombre());
		*/
		//			cout << "" << it.dir() << it->sacar_nombre() <<endl;

					string dir_eti(it.dir());
					dir_eti.erase(0, largo_directorio);

					// existe el archivo?
					int32_t id = modelo.existe_catalogo(etiqueta, dir_eti, it->sacar_nombre());
					if (id) {
						if (opciones.opciones_actualizar & parametros_actualizar::simple) continue;

						// es el mismo?
						if (!modelo.existe_catalogo(etiqueta, dir_eti, it->sacar_nombre(), it->sacar_fecha(), it->sacar_tamanio())) {
							if (opciones.opciones_actualizar & parametros_actualizar::solomostrar)
								cout << "Actualizar: " << directorio << dir_eti << it->sacar_nombre() << endl;
							else if (modelo.actualizar_archivo(id, it->sacar_tamanio(), it->sacar_fecha()))
									cout << "* " << directorio << dir_eti << it->sacar_nombre() << endl;
						}
					} else {
							if (opciones.opciones_actualizar & parametros_actualizar::solomostrar)
								cout << "Faltante: " << directorio << dir_eti << it->sacar_nombre() << endl;
							else {
								modelo.catalogar(etiqueta, dir_eti, it->sacar_nombre(), it->sacar_tamanio(), it->sacar_fecha());
								cout << "+ " << directorio << dir_eti << it->sacar_nombre() << endl;
							}
					}
		
				}
			}
		}

		return 0;
	};

	int regenerar_catalogo(struct parametros_actualizar& opciones, const string& bdd, const string& etiqueta) {
		ModeloCatalogo modelo(bdd);

		int32_t	total_archivos = modelo.cargar_archivos();

		int32_t	total_directorios =	modelo.cargar_directorios();

		int64_t	total_catalogados =	modelo.cargar_catalogo();

		int16_t	total_etiquetas = modelo.cargar_etiquetas();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		string directorio;
		vector<pair<string, string> > etiquetas_directorios;

		if (!etiqueta.empty()) {
			directorio = modelo.directorio_etiqueta(etiqueta);
			etiquetas_directorios.push_back(make_pair(etiqueta, directorio));
		} else {
			modelo.etiquetas_directorios(etiquetas_directorios);
		}

		set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre> arch_dir;
		ModeloCatalogo::voArchivo b;

		for (vector<pair<string, string> >::iterator it=etiquetas_directorios.begin();it!=etiquetas_directorios.end();++it) {
			directorio=it->second;
			string etiqueta=it->first;

			int largo_directorio=directorio.length();
			GuruDirectorio::iterator_recursivo itfin;

			ModeloCatalogo::voEtiqueta e;
			
			e = modelo.data_etiqueta(etiqueta);

			for (GuruDirectorio::iterator_recursivo it2(GuruDirectorio::comoDirectorio(directorio)); it2!=itfin;++it2) {

				if (it2->EsDirectorio()) continue;
			
				b.nombre = it2.dir() + it2->sacar_nombre();
				b.fecha = it2->sacar_fecha();
				b.bytes = it2->sacar_tamanio();

				ModeloCatalogo::voDirectorio d;

				d = modelo.data_directorio(it2.dir().substr(largo_directorio));

				if (!b.valido()) continue;

				ModeloCatalogo::voArchivo a;

				a = modelo.data_archivo(it2->sacar_nombre(), it2->sacar_tamanio(), it2->sacar_fecha());
		
				if (!a.valido()) continue;

				ModeloCatalogo::voCatalogo c;

				c = modelo.data_catalogo(e.id_etiqueta, d.id_directorio, a.id_archivo);

				if (!c.valido()) {
					if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) {
						cout << "Faltante: " << it2.dir() << it2->sacar_nombre() << endl;
					} else {
						modelo.catalogar(e.id_etiqueta, d.id_directorio, a.id_archivo);
						cout << "~ " << it2.dir() << it2->sacar_nombre() << endl;
					}
				}
			}
				
		}

		return 0;
	}

	int actualizar_catalogoV2(struct parametros_actualizar& opciones, const string& bdd, const string& etiqueta) {
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select d.directorio, b.NOMBRE, a.NOMBRE, a.fecha, a.tamanio, c.id_catalogo \
					from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    				and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre> arch_bd;

		ModeloCatalogo::voArchivo a;
		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			a.nombre = it->columna(1).como<string>() + it->columna(2).como<string>() + it->columna(3).como<string>();
			a.fecha = FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>());
			a.bytes = it->columna(5).como<int64_t>();
			a.id_archivo = it->columna(6).como<int32_t>();
//			a.id_archivo=it->columna(9).como<int32_t>();
//			a.md5=it->columna(10).como<string>();
//			a.sha1=it->columna(11).como<string>();
//			a.sha512=it->columna(12).como<string>();

			arch_bd.insert(a);
		}

		string directorio;
		vector<pair<string, string> > etiquetas_directorios;

		if (!etiqueta.empty()) {
			directorio = modelo.directorio_etiqueta(etiqueta);
			etiquetas_directorios.push_back(make_pair(etiqueta, directorio));
		} else {
			modelo.etiquetas_directorios(etiquetas_directorios);
		}

		set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre> arch_dir;
		ModeloCatalogo::voArchivo b;

		for (vector<pair<string, string> >::iterator it=etiquetas_directorios.begin();it!=etiquetas_directorios.end();++it) {
			directorio=it->second;
			string etiqueta=it->first;

			int largo_directorio=directorio.length();
			GuruDirectorio::iterator_recursivo itfin;

			for (GuruDirectorio::iterator_recursivo it2(GuruDirectorio::comoDirectorio(directorio)); it2!=itfin;++it2) {

				if (it2->EsDirectorio()) continue;
			
				b.nombre = it2.dir() + it2->sacar_nombre();
				b.fecha = it2->sacar_fecha();
				b.bytes = it2->sacar_tamanio();

				set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre>::iterator it3(arch_bd.find(b));

				if (it3 == arch_bd.end()) 
					arch_dir.insert(b);
				else {
					if (*it3 == b) arch_bd.erase(it3);
					else arch_dir.insert(b);
				}
			}

			//
			// El contenedor arch_dir tiene los archivo que estan en el directorio y NO en la base de datos.
			// El contenedor arch_bd tiene lo que estan en la base de datos y NO en el directorio.
			//
			cout << "Por Descatalogar:" << arch_bd.size() << endl;
			cout << "NO Catalogados  :" << arch_dir.size() << endl;

			// borro de la base de datos los archivos que no estan en el directorio
			for (set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre>::iterator it3(arch_bd.begin());it3!=arch_bd.end();++it3) {
				if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) {
					cout << "Faltante: " << it3->nombre << endl;
				} else {
					if (modelo.borrar_catalogo(it3->id_archivo))
						cout << "- " << it3->nombre << endl;
				}
			}

			// Catalogo los archivos que estan en disco y no en la base de datos.
			for (set<ModeloCatalogo::voArchivo, ModeloCatalogo::voArchivo::ordenar_nombre>::iterator it4(arch_dir.begin());it4!=arch_dir.end();++it4) {
				if (opciones.opciones_actualizar & parametros_actualizar::solomostrar) {
					cout << "Faltante: " << it4->nombre << endl;
				} else {
						if (opciones.opciones_actualizar & parametros_actualizar::restaurar) {
							if (modelo.restaurar(it4->nombre, etiqueta))
								cout << "~ " << it4->nombre << endl;
						} else {
							if (modelo.catalogar(it4->nombre, etiqueta))
								cout << "+ " << it4->nombre << endl;
						}
				}
			}
		}

		return 0;
	};


	int actualizar_catalogoV3(struct parametros_actualizar& opciones, const string& bdd, const string& etiqueta) {
		ModeloCatalogo modelo(bdd);
		iterator_fdb itfinal(modelo);
		string c("select d.directorio, b.NOMBRE, a.NOMBRE, a.fecha, a.tamanio ,d.id_etiqueta, b.id_directorio, a.id_archivo, a.md5, a.sha1, a.sha512 \
					from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
					where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    				and c.ID_ETIQUETA = d.ID_ETIQUETA");

		if (!etiqueta.empty()) {
			c += " and d.nombre=\'" + etiqueta + "\'";
		}

		set<ModeloCatalogo::voArchivo> arch_bd;

		ModeloCatalogo::voArchivo a;
		for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
			a.nombre = it->columna(1).como<string>() + it->columna(2).como<string>() + it->columna(3).como<string>();
			a.fecha = FDBFechaHora_time_t(it->columna(4).como<IBPP::Timestamp>());
			a.bytes = it->columna(5).como<int64_t>();

			arch_bd.insert(a);
		}

		string directorio;
		vector<pair<string, string> > etiquetas_directorios;

		if (!etiqueta.empty()) {
			directorio = modelo.directorio_etiqueta(etiqueta);
			etiquetas_directorios.push_back(make_pair(etiqueta, directorio));
		} else {
			modelo.etiquetas_directorios(etiquetas_directorios);
		}

		set<ModeloCatalogo::voArchivo> arch_fs;
		ModeloCatalogo::voArchivo b;

		for (vector<pair<string, string> >::iterator it=etiquetas_directorios.begin();it!=etiquetas_directorios.end();++it) {
			directorio=it->second;
			string etiqueta=it->first;

			int largo_directorio=directorio.length();
			GuruDirectorio::iterator_recursivo itfin;

			for (GuruDirectorio::iterator_recursivo it(GuruDirectorio::comoDirectorio(directorio)); it!=itfin;++it) {
				b.nombre = it.dir() + it->sacar_nombre();
				b.fecha = it->sacar_fecha();
				b.bytes = it->sacar_tamanio();

				if (b == a) {
//					arch_bd.erase();
				} else {
						arch_fs.insert(b);
				}
			}
		}

		return 0;
	};

}
#endif
