#include <argtable2.h>
#include <iostream>
#include <cstdlib>

#include <ModeloCatalogo.h>
#include <CatalogoActualizar.hpp>
#include <CatalogoInfo.hpp>
#include <CatalogoDebug.hpp>

#include <GuruDirectorio.hpp>
#include <GuruSHA1.hpp>
#include <GuruSHA4.hpp>
#include <GuruMD5.hpp>

#ifdef WIN32
#include <tre/regex.h>      /* REG_ICASE */
#else 
#include <regex.h>      /* REG_ICASE */
#endif

using namespace std;

struct parametros_buscar {
	enum {distinguir=0x01, // no distingue entre mayusculas y minusculas
			  exacta=0x02		// busca la palabra exacta
		};
public:
	parametros_buscar(uint32_t opciones=!distinguir|!exacta):opciones_buscar(opciones) {};
	uint32_t 	opciones_buscar;

	template <typename T>
	T exacta_palabra(const T& palabra) {
		return (opciones_buscar & exacta) ? string (palabra) : string ("%" + palabra + "%");
	}

	string distinguir_palabra(const string& palabra) {
		return (opciones_buscar & distinguir) ? string (palabra) : string ("lower(" + palabra + ")");
	}
};

#define DISTINGUIR_PALABRA(x, y) ((y & parametros_buscar::distinguir) ? string (x) : string ("lower(" + x + ")"));

struct parametros_catalogar {
	enum {sha1=0x01,		// que catalogo el sha1 de cada archivo
			sha512=0x02,	// que catalogo el sha512 de cada archivo
			md5=0x04,		// que catalogo el md5 de cada archivo
			nocrear=0x08,	// no crear la base de datos si no existe
	};
public:
	parametros_catalogar(uint32_t opciones=!sha1|!sha512|!md5):opciones_catalogar(opciones) {};
	uint32_t 	opciones_catalogar;
};

int main_buscar(struct parametros_buscar& opciones, const string& bdd, const string& etiqueta, const string& archivo, const string& tamanio, const string& fecha, const string& directorio, const string& sha1, const string& sha512, const string& md5, const string& todo) {
    ModeloCatalogo modelo(bdd);
    iterator_fdb itfinal(modelo);
    string c("select b.NOMBRE, a.NOMBRE \
                from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
                where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
                    and c.ID_ETIQUETA = d.ID_ETIQUETA");

	if (!todo.empty()) {
  		string palabra(todo);
		if (!(opciones.opciones_buscar & parametros_buscar::exacta))
			palabra = "%" + todo + "%";

//		c +=" and (" +  opciones.distinguir_palabra("a.nombre") + " like " + opciones.distinguir_palabra(palabra) + " or " + opciones.distinguir_palabra("a.sha1") + " like " + opciones.distinguir_palabra(palabra) + " or " + opciones.distinguir_palabra("a.sha512") + " like " + opciones.distinguir_palabra(palabra) + " or " + opciones.distinguir_palabra("a.md5") + " like " + opciones.distinguir_palabra(palabra) + " or " + opciones.distinguir_palabra("b.nombre") + " like " + opciones.distinguir_palabra(palabra) + ")";

		if (opciones.opciones_buscar & parametros_buscar::distinguir)
			c+=	" and ( a.nombre like \'" + palabra + "\' or a.sha1 like \'" + palabra + "\' or a.sha512 like \'" + palabra + "\' or a.md5 like \'" + palabra + "\' or b.nombre like  \'" + palabra + "\')";
		else
			c+= " and ( lower(a.nombre) like lower(\'" + palabra + "\') or lower(a.sha1) like lower(\'" + palabra + "\') or lower(a.sha512) like lower(\'" + palabra + "\') or lower(a.md5) like lower(\'" + palabra + "\') or lower(b.nombre) like lower( \'" + palabra + "\') )";

	} else {
		if (!etiqueta.empty()) 	c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and d.nombre like \'" + etiqueta + "\'" : " and lower(d.nombre) like lower(\'" + etiqueta + "\')";
		if (!archivo.empty())  	c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and a.nombre like \'" + archivo + "\'" : " and lower(a.nombre) like lower(\'" + archivo + "\')";
		if (!tamanio.empty())  	c += " and a.tamanio=\'" + tamanio + "\'";
		if (!fecha.empty())		c += " and a.fecha=\'" + fecha + "\'";
		if (!directorio.empty()) c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and b.nombre like \'" + directorio + "\'" : " and lower(b.nombre) lower(like \'" + directorio + "\')";
		if (!sha1.empty()) 		c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and a.sha1=\'" + sha1 + "\'" : " and lower(a.sha1)=lower(\'" + sha1 + "\')";
		if (!sha512.empty())	c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and a.sha512=\'" + sha512 + "\'" : " and lower(a.sha512)=lower(\'" + sha512 + "\')";
		if (!md5.empty()) 	c += (opciones.opciones_buscar & parametros_buscar::distinguir) ? " and a.md5=\'" + md5 + "\'" : " and lower(a.md5)=lower(\'" + md5 + "\')";
	}

//	cout << c << endl;

    for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
        cout << it->columna(1).como<string>() << it->columna(2).como<string>() << endl;
    }

	return 0;
};

int main_borrar(const string& bdd) {
	cout << "main_borrar" << endl;
	return 0;
};

int main_catalogar(const string& bdd, const string& etiqueta, const string& directorio, const string& filtro, struct parametros_catalogar& opciones) {
	try {
		ModeloCatalogo modelo(bdd);

		// No existe el archivo de catalogo y no tengo la opcion de nocrear
		if (!GuruDirectorio::propiedades_archivo(bdd).Existe() && !(opciones.opciones_catalogar & parametros_catalogar::nocrear)) {
				modelo.crear_catalogo();
				cout << "Se crea el catalogo" << endl;
		} else {
				if (!GuruDirectorio::propiedades_archivo(bdd).Existe()) {
					cout << "No existe el catalogo" << endl;
					return 0;
				} 
		}

		int16_t etiquetas_opciones=0;
		if (opciones.opciones_catalogar & parametros_catalogar::sha1) 
			etiquetas_opciones |= ModeloCatalogo::sha1;
		if (opciones.opciones_catalogar & parametros_catalogar::sha512) 
			etiquetas_opciones |= ModeloCatalogo::sha512;
		if (opciones.opciones_catalogar & parametros_catalogar::md5) 
			etiquetas_opciones |= ModeloCatalogo::md5;

		modelo.agregar_etiqueta(etiqueta, directorio, (filtro.empty() ? "*.*" : filtro), etiquetas_opciones);

		GuruDirectorio::iterator_recursivo itfin;

		string sha1,sha512,md5, directorio_actual;

		for (GuruDirectorio::iterator_recursivo it(directorio, (filtro.empty() ? "*.*" : filtro)); it!=itfin;++it){
			cout << it.dir() << it->sacar_nombre() << endl;
			directorio_actual = it.dir().substr(directorio.length()-1);
		
			sha1.clear();sha512.clear();md5.clear();
			if (opciones.opciones_catalogar & parametros_catalogar::sha1) 
				sha1 = GuruSHA1::archivo(it.dir() + it->sacar_nombre());
			if (opciones.opciones_catalogar & parametros_catalogar::sha512) 
				sha512 = GuruSHA4::archivo_sha512(it.dir() + it->sacar_nombre());
			if (opciones.opciones_catalogar & parametros_catalogar::md5) 
				md5 = GuruMD5::archivo(it.dir() + it->sacar_nombre());

			modelo.catalogar(etiqueta, directorio_actual, it->sacar_nombre(), it->sacar_tamanio(), it->sacar_fecha(), md5, sha1, sha512);
		}
	} catch (string& e) {
		cout << "Error al catalogar:" << e << endl;
	}

	return 0;
};

int main(int argc, char* argv[])
{
// sintaxis para actualizar
	struct arg_rex* cmd_actualizar		= arg_rex1(NULL, NULL, "actualizar", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_actualizar		= arg_str1("b", "bdd", "BaseFDB", "Base de Datos.");
	struct arg_str* etiqueta_actualizar	= arg_str0("e", "etiqueta", "Etiqueta", "Etiqueta a actualizar.");
	struct arg_str* cambiar_etiqueta_actualizar	= arg_str0(NULL, "nueva-etiqueta", "Etiqueta", "Cambia el nombre de la etiqueta.");
	struct arg_str* cambiar_dir_actualizar	= arg_str0(NULL, "directorio", "Directorio", "Cambia el directorio de una etiqueta.");
	struct arg_str* cambiar_filtro_actualizar	= arg_str0(NULL, "filtro", "Filtro", "Cambia el filtro para catalogar.");
	struct arg_lit* depurar_actualizar	= arg_lit0(NULL, "depurar",  "Depura archivos que no esten catalogados.");
	struct arg_lit* solomostrar_actualizar	= arg_lit0(NULL, "solomostrar", "Solo muestra los archivos a depurar del catalogo.");
	struct arg_lit* solocatalogo_actualizar	= arg_lit0(NULL, "solocatalogo", "Solo actualiza el contenido del catalgo con el directorio.");
	struct arg_lit* solodirectorio_actualizar	= arg_lit0(NULL, "solodirectorio", "Solo actualiza el directorio con el catalogo.");
	struct arg_lit* simple_actualizar	= arg_lit0(NULL, "simple", "Solo verifica nombre/bytes y fecha para actualizar el catalogo.");
	struct arg_lit* completo_actualizar	= arg_lit0(NULL, "completa", "verifica nombre/bytes/fecha y hash para actualizar el catalogo.");
	struct arg_lit* contrastar_actualizar	= arg_lit0(NULL, "contrastar", "Verifica el catalogo por falta de algun checksum.");
	struct arg_lit* cache_dir_actualizar	= arg_lit0(NULL, "cached", "Carga en memoria los directorios.");
	struct arg_lit* cache_archivo_actualizar	= arg_lit0(NULL, "cachea", "Carga en memoria los archivos.");
	struct arg_lit* restaurar_actualizar	= arg_lit0(NULL, "restaurar", "Restaura archivo borrados del catalogo, en memoria sin crear directorios.");
	struct arg_lit* regenerar_actualizar	= arg_lit0(NULL, "regenerar", "Restaura archivo borrados del catalogo, sobre la BD y creando directorios si es necesario.");
    struct arg_end* end_actualizar		= arg_end(20);
    void* argtable_actualizar[] 		= {cmd_actualizar,bdd_actualizar,etiqueta_actualizar,cambiar_etiqueta_actualizar,cambiar_dir_actualizar,cambiar_filtro_actualizar, depurar_actualizar, solomostrar_actualizar, solocatalogo_actualizar, solodirectorio_actualizar, simple_actualizar, completo_actualizar, contrastar_actualizar,cache_dir_actualizar,cache_archivo_actualizar, restaurar_actualizar, regenerar_actualizar, end_actualizar};
    int nerrors_actualizar;

// sintaxis para catalogar
	struct arg_rex* cmd_catalogar			= arg_rex1(NULL,  NULL,  "catalogar", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_catalogar			= arg_str1("b",  "bdd",  "BaseFDB", "Base de Datos.");
	struct arg_str* etiqueta_catalogar		= arg_str1("e",  "etiqueta", "Etiqueta", "Etiqueta a Actualizar.");
	struct arg_str* directorio_catalogar	= arg_str1("d",  "directorio", "Directorio", "Directorio a Catalogar.");
	struct arg_str* filtro_catalogar		= arg_str0("f",  "filtro", "Filtro", "Filtro a catalogar.");
	struct arg_lit* sha1_catalogar			= arg_lit0(NULL, "sha1",  "Cataloga el SHA1 checksum de cada archivo.");
	struct arg_lit* sha512_catalogar   		= arg_lit0(NULL, "sha512", "Cataloga el SHA512 checksum de cada archivo.");
	struct arg_lit* md5_catalogar   		= arg_lit0(NULL, "md5",  "Cataloga el md5 checksum de cada archivo.");
	struct arg_lit* nocrear_catalogar		= arg_lit0(NULL, "nocrear",  "NO crea el catalogo si no existe.");
	struct arg_end* end_catalogar			= arg_end(20);
	void* argtable_catalogar[] 				= {cmd_catalogar,bdd_catalogar, etiqueta_catalogar,directorio_catalogar,filtro_catalogar,sha1_catalogar,sha512_catalogar,md5_catalogar,nocrear_catalogar,end_catalogar};
	int nerrors_catalogar;

// sintaxis para buscar
	struct arg_rex* cmd_buscar			= arg_rex1(NULL,  NULL,  "buscar", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_buscar			= arg_str1("b",  "bdd",  "BaseFDB", "Base de Datos.");
    struct arg_str* etiqueta_buscar		= arg_str0("e",  "etiqueta", "Etiqueta", ".");
    struct arg_str* archivo_buscar		= arg_str0("a",  "archivo", "Archivo", ".");
    struct arg_str* tamanio_buscar		= arg_str0("b",  "bytes", "Bytes", ".");
    struct arg_str* fecha_buscar		= arg_str0("f",  "fecha", "Fecha", ".");
    struct arg_str* directorio_buscar	= arg_str0("d",  "directorio", "Directorio", ".");
    struct arg_str* sha1_buscar			= arg_str0(NULL, "sha1", "checksum", ".");
    struct arg_str* sha512_buscar		= arg_str0(NULL, "sha512", "checksum", ".");
    struct arg_str* md5_buscar			= arg_str0(NULL, "md5", "checksum",  ".");
    struct arg_lit* exacta_buscar		= arg_lit0(NULL, "exacta", "Busca la palabra exacta.");
    struct arg_lit* case_buscar			= arg_lit0("c", "distinguir", "Distingue entre mayusculas y minusculas.");
	struct arg_str* todo_buscar			= arg_str0(NULL, NULL, "<palabra>", "Palabra a buscar.");
    struct arg_end* end_buscar			= arg_end(20);
    void* argtable_buscar[] 			= {cmd_buscar,bdd_buscar,etiqueta_buscar, archivo_buscar, tamanio_buscar, fecha_buscar, directorio_buscar, sha1_buscar, sha512_buscar, md5_buscar, exacta_buscar, case_buscar, todo_buscar, end_buscar};
    int nerrors_buscar;

// sintaxis para borrar
	struct arg_rex* cmd_borrar			= arg_rex1(NULL,  NULL,  "borrar", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_borrar			= arg_str1("b",  "bdd",  "BaseFDB", "Base de Datos.");
	struct arg_str* etiqueta_borrar		= arg_str0("e",  "etiqueta", "Etiqueta", "Etiqueta a borrar");
    struct arg_str* archivo_borrar      = arg_str0("a",  "archivo", "Archivo", ".");
    struct arg_str* tamanio_borrar      = arg_str0("b",  "bytes", "Bytes", ".");
    struct arg_str* fecha_borrar        = arg_str0("f",  "fecha", "Fecha", ".");
 	struct arg_str* directorio_borrar	= arg_str0("d",  "directorio", "Directorio", "Directorio a borrar");
	struct arg_str* sha1_borrar   		= arg_str0(NULL, "sha1", "checksum", "SHA1 checksum a borrar.");
	struct arg_str* sha512_borrar   	= arg_str0(NULL, "sha512", "checksum", "SHA512 checksum a borrar.");
	struct arg_str* md5_borrar  		= arg_str0(NULL, "md5",  "checksum", "MD5 checksum a borrar.");
    struct arg_end* end_borrar			= arg_end(20);
    void* argtable_borrar[] 			= {cmd_borrar,bdd_borrar, etiqueta_borrar, archivo_borrar, tamanio_borrar, fecha_borrar, directorio_borrar,sha1_borrar,sha512_borrar,md5_borrar,end_borrar};
    int nerrors_borrar;

// sintaxis para info
	struct arg_rex* cmd_info		= arg_rex1(NULL,  NULL,  "info", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_info		= arg_str1("b",  "bdd",  "BaseFDB", "Base de Datos.");
	struct arg_str* etiqueta_info	= arg_str0("e",  "etiqueta", "Etiqueta", "Etiqueta a mostrar.");
	struct arg_lit* dump_info		= arg_lit0(NULL, "dump", "Vuelca el contenido completo del catalogo.");
	struct arg_lit* dump_etiquetas_info		= arg_lit0(NULL, "dump-etiquetas", "Vuelca las etiquetas en el catalogo.");
	struct arg_lit* dump_directorios_info	= arg_lit0(NULL, "dump-directorios", "Vuelca los directorios del catalogo.");
	struct arg_lit* dump_archivos_info		= arg_lit0(NULL, "dump-archivos", "Vuelca los archivos del catalogo.");
	struct arg_end* end_info		= arg_end(10);
	void* argtable_info[] 			= {cmd_info,bdd_info,etiqueta_info, dump_info, dump_etiquetas_info, dump_directorios_info, dump_archivos_info, end_info};
	int nerrors_info;

	// sintaxis para debug
	struct arg_rex* cmd_debug		= arg_rex1(NULL,  NULL,  "debug", NULL, REG_ICASE, NULL);
	struct arg_str* bdd_debug		= arg_str1("b",  "bdd",  "BaseFDB", "Base de Datos.");
	struct arg_str* etiqueta_debug	= arg_str0("e",  "etiqueta", "Etiqueta", NULL);
	struct arg_lit* dump_tabla_etiquetas_debug	= arg_lit0(NULL, "dump-etiquetas", "Vuelca la tabla etiqueta.");
	struct arg_lit* dump_tabla_directorios_debug	= arg_lit0(NULL, "dump-directorios", "Vuelca la tabla directorio.");
	struct arg_lit* dump_tabla_archivos_debug		= arg_lit0(NULL, "dump-archivos", "Vuelca la tabla archivo.");
	struct arg_str* consulta_debug	= arg_str0("c", "consulta", "Query", "Realiza un query a la base.");
	struct arg_lit* exportar_debug	= arg_lit0(NULL, "exportar", "Vuelco en formato SQL.");
	struct arg_str* importar_debug	= arg_str0(NULL, "importar", "Archivo", "Importa en formato SQL.");
	struct arg_end* end_debug		= arg_end(20);
	void* argtable_debug[] 			= {cmd_debug,bdd_debug,etiqueta_debug, dump_tabla_etiquetas_debug, dump_tabla_directorios_debug, dump_tabla_archivos_debug, consulta_debug, exportar_debug, importar_debug, end_debug};
	int nerrors_debug;

    /* verify all argtable[] entries were allocated sucessfully */
     if (arg_nullcheck(argtable_actualizar)!=0 || arg_nullcheck(argtable_buscar)!=0 || arg_nullcheck(argtable_borrar)!=0 || arg_nullcheck(argtable_catalogar)!=0 ) {
         /* NULL entries were detected, some allocations must have failed */
         cout << "insufficient memory" << endl;
     }

     /* Above we defined a separate argtable for each possible command line syntax */
     /* and here we parse each one in turn to see if any of them are successful    */
	nerrors_actualizar	= arg_parse(argc,argv,argtable_actualizar);
	nerrors_buscar 		= arg_parse(argc,argv,argtable_buscar);
	nerrors_borrar 		= arg_parse(argc,argv,argtable_borrar);
	nerrors_catalogar 	= arg_parse(argc,argv,argtable_catalogar);
	nerrors_info 		= arg_parse(argc,argv,argtable_info);
	nerrors_debug 		= arg_parse(argc,argv,argtable_debug);

	if (nerrors_actualizar==0) {
		CatalogoActualizar::parametros_actualizar opciones;
		if (solomostrar_actualizar->count > 0) opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::solomostrar;
		if (cache_dir_actualizar->count > 0) opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::cache_dir;
		if (cache_archivo_actualizar->count > 0) opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::cache_archivo;

		if (cambiar_dir_actualizar->count > 0) {
			if (etiqueta_actualizar->count > 0) {
				CatalogoActualizar::actualizar_directorio(bdd_actualizar->sval[0], etiqueta_actualizar->sval[0], cambiar_dir_actualizar->sval[0]);
			} else {
				cout << "Falta especificar la etiqueta." << endl;
			}
		} else if (cambiar_filtro_actualizar->count > 0) {
            if (etiqueta_actualizar->count > 0) {
                CatalogoActualizar::actualizar_filtro(bdd_actualizar->sval[0], etiqueta_actualizar->sval[0], cambiar_filtro_actualizar->sval[0]);
            } else {
                cout << "Falta especificar la etiqueta." << endl;
            }
		} else if (cambiar_etiqueta_actualizar->count > 0) {
		            if (etiqueta_actualizar->count > 0) {
        		        CatalogoActualizar::actualizar_etiqueta(bdd_actualizar->sval[0], etiqueta_actualizar->sval[0], cambiar_etiqueta_actualizar->sval[0]);
		            } else {
                		cout << "Falta especificar la etiqueta." << endl;
            		}
		} else if (contrastar_actualizar->count > 0) {
					contrastar_catalogo(opciones, bdd_actualizar->sval[0], etiqueta_actualizar->sval[0]);		            
		} else {		// no se especifica ningun parametro, se chequean los dos.
				if (solocatalogo_actualizar->count == 0 && solodirectorio_actualizar->count == 0) {
					opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::solocatalogo;
					opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::solodirectorio;
				} else {
					if (solocatalogo_actualizar->count > 0) opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::solocatalogo;
					if (solodirectorio_actualizar->count > 0) opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::solodirectorio;
				}

				// si esta la opcion de completo deshabilito la opcion de simple que viene por default.
				if (completo_actualizar->count > 0) {
					opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::completo;
					opciones.opciones_actualizar ^= CatalogoActualizar::parametros_actualizar::simple;
				}

				if (regenerar_actualizar->count > 0) {
					opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::regenerar;
					CatalogoActualizar::regenerar_catalogo(opciones, bdd_actualizar->sval[0], etiqueta_actualizar->sval[0]);
				} else {
					if (restaurar_actualizar->count > 0) 
						opciones.opciones_actualizar |= CatalogoActualizar::parametros_actualizar::restaurar;

					actualizar_catalogoV2(opciones, bdd_actualizar->sval[0], etiqueta_actualizar->sval[0]);
				}
		}
	} else if (nerrors_buscar==0) {
		struct parametros_buscar opciones;
		if (case_buscar->count > 0) opciones.opciones_buscar |=parametros_buscar::distinguir;
		if (exacta_buscar->count > 0 ) opciones.opciones_buscar |=parametros_buscar::exacta;

		if ((todo_buscar->count == 0) && (etiqueta_buscar->count == 0 &&  archivo_buscar->count == 0 && tamanio_buscar->count == 0 && fecha_buscar->count == 0 && directorio_buscar->count == 0 && sha1_buscar->count == 0 && sha512_buscar->count == 0 && md5_buscar->count == 0))
			arg_print_glossary_gnu(stdout,argtable_buscar);
		else
			main_buscar(opciones, bdd_buscar->sval[0], etiqueta_buscar->sval[0], archivo_buscar->sval[0], tamanio_buscar->sval[0], fecha_buscar->sval[0], directorio_buscar->sval[0], sha1_buscar->sval[0], sha512_buscar->sval[0], md5_buscar->sval[0], todo_buscar->sval[0]);
	} else if (nerrors_borrar==0)
		main_borrar(bdd_borrar->sval[0]);
	else if (nerrors_catalogar==0) { 
		struct parametros_catalogar opciones;
		if (sha1_catalogar->count > 0) opciones.opciones_catalogar |=parametros_catalogar::sha1;
		if (sha512_catalogar->count > 0 ) opciones.opciones_catalogar |=parametros_catalogar::sha512;
		if (md5_catalogar->count > 0 ) opciones.opciones_catalogar |=parametros_catalogar::md5;
		if (nocrear_catalogar->count > 0 ) opciones.opciones_catalogar |=parametros_catalogar::nocrear;

		string filtro;
		filtro = (filtro_catalogar->count > 0) ? filtro_catalogar->sval[0] : "*.*";

		main_catalogar(bdd_catalogar->sval[0], etiqueta_catalogar->sval[0], GuruDirectorio::comoDirectorio(string(directorio_catalogar->sval[0])), filtro, opciones);
	} else if (nerrors_info==0) {
		if (dump_info->count > 0)
			CatalogoInfo::dump_catalogo(bdd_info->sval[0], etiqueta_info->sval[0]);
		else if (dump_etiquetas_info->count > 0)
			CatalogoInfo::dump_etiquetas(bdd_info->sval[0]);
		else if (dump_directorios_info->count > 0)
			CatalogoInfo::dump_directorios(bdd_info->sval[0], etiqueta_info->sval[0]);
		else if (dump_archivos_info->count > 0)
			CatalogoInfo::dump_archivos(bdd_info->sval[0], etiqueta_info->sval[0]);
		else
			arg_print_glossary_gnu(stdout,argtable_info);
	} else if (nerrors_debug==0) {
		if (dump_tabla_etiquetas_debug->count > 0)
			CatalogoDebug::dump_tabla_etiquetas(bdd_debug->sval[0]);
		else if (dump_tabla_directorios_debug->count > 0)
			CatalogoDebug::dump_tabla_directorios(bdd_debug->sval[0]);
		else if (dump_tabla_archivos_debug->count > 0)
			CatalogoDebug::dump_tabla_archivos(bdd_debug->sval[0]);
		else if (consulta_debug->count > 0)
			CatalogoDebug::consulta(bdd_debug->sval[0], consulta_debug->sval[0]);
		else if (exportar_debug->count > 0)
			CatalogoDebug::exportar(bdd_debug->sval[0]);
		else if (importar_debug->count > 0)
			CatalogoDebug::importar(bdd_debug->sval[0], importar_debug->sval[0]);
	} else {
          /* We get here if the command line matched none of the possible syntaxes */
		if (cmd_actualizar->count > 0) {
			arg_print_errors(stdout,end_actualizar,NULL);
			arg_print_glossary(stdout,argtable_actualizar,"      %-30s %s\n");
		} else if (cmd_buscar->count > 0) {
			arg_print_errors(stdout,end_buscar,argv[0]);
	//		arg_print_glossary(stdout,argtable_buscar,"      %-30s %s\n");
			arg_print_glossary_gnu(stdout,argtable_buscar);
		} else if (cmd_borrar->count > 0) {
			arg_print_errors(stdout,end_borrar,argv[0]);
			arg_print_glossary(stdout,argtable_borrar,"      %-30s %s\n");
		} else if (cmd_catalogar->count > 0) {
			arg_print_errors(stdout,end_catalogar,argv[0]);
			arg_print_glossary(stdout,argtable_catalogar,"      %-30s %s\n");
		} else if (cmd_info->count > 0) {
			arg_print_errors(stdout,end_info,argv[0]);
			arg_print_glossary(stdout,argtable_info,"      %-30s %s\n");
		} else if (cmd_debug->count > 0) {
			arg_print_errors(stdout,end_debug,argv[0]);
	//		arg_print_glossary(stdout,argtable_buscar,"      %-30s %s\n");
			arg_print_glossary_gnu(stdout,argtable_debug);
		} else {
			cout << "Falta comando <catalogar|actualizar|buscar|borrar|info>" << endl;
			cout << "Uso 1:"; arg_print_syntax(stdout,argtable_catalogar,"\n");
			cout << "Uso 2:"; arg_print_syntax(stdout,argtable_actualizar,"\n");
			cout << "Uso 3:"; arg_print_syntax(stdout,argtable_buscar,"\n");
			cout << "Uso 4:"; arg_print_syntax(stdout,argtable_borrar,"\n");
			cout << "Uso 5:"; arg_print_syntax(stdout,argtable_info,"\n");
		}
	}

	arg_freetable(argtable_catalogar,sizeof(argtable_catalogar)/sizeof(argtable_catalogar[0]));
	arg_freetable(argtable_actualizar,sizeof(argtable_actualizar)/sizeof(argtable_actualizar[0]));
	arg_freetable(argtable_borrar,sizeof(argtable_borrar)/sizeof(argtable_borrar[0]));
	arg_freetable(argtable_buscar,sizeof(argtable_buscar)/sizeof(argtable_buscar[0]));
	arg_freetable(argtable_info,sizeof(argtable_info)/sizeof(argtable_info[0]));
	arg_freetable(argtable_debug,sizeof(argtable_debug)/sizeof(argtable_debug[0]));

	return 0;
}
