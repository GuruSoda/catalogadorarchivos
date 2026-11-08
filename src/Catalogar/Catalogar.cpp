// #define BOOST_REGEX_SOURCE BOOST_REGEX_NO_LIB BOOST_ALL_NO_LIB BOOST_REGEX_USE_CPP_LOCALE

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/progress.hpp>
#include <boost/regex.hpp>
#include <GuruCompat.h>
#include <GuruCRC.h>
#include <GuruSHA1.h>
#include <GuruSHA4.h>
#include <exception>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

#include <GuruDirectorio.hpp>
#include <ModeloCatalogo.h>
#include <ConfigCatalogar.h>

namespace po = boost::program_options;

using namespace boost;
using namespace std;

void procesar_parametros(int argc, char* argv[], po::variables_map& vm, config_catalogar& configuracion);
void catalogar(config_catalogar& configuracion, boost::progress_display* barra);
int crear_db(config_catalogar& configuracion);
::uint32_t total_archivos (config_catalogar& configuracion);
bool listar_repetidos(config_catalogar& configuracion);
void dump_txt(config_catalogar& configuracion);

// --bd "D:\Projectos\Desarrollo\Catalogador de Archivos\debug\Catalogo.fdb" -e musica -d E:\Musica
// catalogar --bd "D:\Projectos\Desarrollo\Catalogador de Archivos\debug\catalogo-full.fdb" -d D:\ -e Imagenes --filtro jpg$
int main2(int argc, char* argv[])
{
	config_catalogar	configuracion;
	::uint32_t total=0;

	try{
		po::variables_map vm;

		procesar_parametros(argc, argv, vm, configuracion);

		cout << "Contando Archivos ..." << endl;
		total = total_archivos(configuracion);
		cout << "Archivos Encontrados: " << total << endl;

		cout << "Cargando ..." << endl;
		boost::progress_display barra(total, std::cout);

		catalogar(configuracion, &barra);
	}catch(std::exception& e) {
		std::cerr << e.what() << "\n";
        return 1;
    }catch(std::string& e) {
		std::cerr << e << "\n";
        return 1;
    }

	return 0;
}

void procesar_parametros(int argc, char* argv[], po::variables_map& vm, config_catalogar& configuracion)
{
	po::options_description desc("Parametros Validos");
	desc.add_options()
		("ayuda,h", "Esta pantalla.")
		("bdd", po::value<string>(), "Base de Datos a guardar el catalogo.")
		("crear,c", "Crea la base de datos para el catalogo.")
		("directorio,d", po::value<string>(), "Directorio a catalogar.")
		("etiqueta,e", po::value<string>(), "Etiqueta del nuevo catalogo.")
		("nocrc", "No verifica el CRC de cada archivo.")
		("nosha1", "No verifica el sha1 de cada archivo.")
		("nosha512", "No verifica el sha512 de cada archivo.")
		("filtro,f", po::value<string>(), "Expresion regular para catalogar archivos.")
		("excluir-dir", po::value<string>(), "Expresion regular para excluir directorios.")
		("excluir-archivos", po::value<string>(), "Expresion regular para excluir archivos.")
		("repetidos,r", "Lista los archivos repetidos.")
		("dump-txt", "Lista el catalogo.")
	;

//	po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

	if (vm.size() == 0 || vm.count("ayuda")) cout << desc << endl;

	if (!vm.count("bdd")) throw string("Falta especificar la base de datos.");
	else configuracion.basededatos = vm["bdd"].as<string>();

	if (vm.count("dump-txt")) {
		dump_txt(configuracion);
		throw string("Base de datos volcada.");
	}

	if (vm.count("crear"))
		if (!crear_db(configuracion)) throw string("Base de datos creada correctamente.");

	if (vm.count("repetidos"))
		if (listar_repetidos(configuracion)) throw string("");

	if (!vm.count("directorio")) throw string("Falta especificar el directorio a catalogar.");

	if (!vm.count("etiqueta")) throw string("Falta especificar la etiqueta del nuevo catalogo.");

	configuracion.directorio = vm["directorio"].as<string>();
	configuracion.etiqueta = vm["etiqueta"].as<string>();
	if (vm.count("nocrc"))
		configuracion.crc = false;
	if (vm.count("nosha1"))
		configuracion.sha1 = false;
	if (vm.count("nosha512"))
		configuracion.sha512 = false;

	if (vm.count("filtro")) configuracion.filtro = vm["filtro"].as<string>();

	configuracion.corregir_parametros();

	cout << "Base de Datos: " << configuracion.basededatos << endl;
	cout << "Directorio: " << configuracion.directorio << endl;
	cout << "Etiqueta: " << configuracion.etiqueta << endl;
}

void catalogar(config_catalogar& configuracion, boost::progress_display* barra)
{
	ModeloCatalogo modelo(configuracion.basededatos);

	time_t inicio_c;
	time(&inicio_c);

	string crc, sha1, sha512;

	modelo.agregar_etiqueta(configuracion.etiqueta, configuracion.directorio);

//	boost::progress_timer t( std::clog );
	GuruDirectorio::iterator_recursivo itfinal;
	for (GuruDirectorio::iterator_recursivo it(configuracion.directorio);it != itfinal;++it) {
		if(regex_match(it->sacar_nombre(), boost::regex(configuracion.filtro))) {
			if (configuracion.crc) crc = GuruCRC::archivo_crc32(it.dir() + it->sacar_nombre());
			if (configuracion.sha1) sha1 = GuruSHA1::archivo_sha1(it.dir() + it->sacar_nombre());
			if (configuracion.sha512) sha512 = GuruSHA4::archivo_sha512(it.dir() + it->sacar_nombre());

//			cout << "Catalogando:" << it.dir() + it->sacar_nombre() << "...";
			time_t inicio;
			time(&inicio);
//			modelo.catalogar_memoria(configuracion.etiqueta, replace_all_copy(it.dir(), "\\", "/"), it->sacar_nombre(), it->sacar_tamanio(), it->sacar_fecha(), crc, sha1, sha512);
			modelo.catalogar(configuracion.etiqueta, replace_all_copy(it.dir(), "\\", "/"), it->sacar_nombre(), it->sacar_tamanio(), it->sacar_fecha(), crc, sha1, sha512);
			time_t fin;
			time(&fin);
//			cout << "Bien...(" << fin-inicio << ")" <<  endl;

			++(*barra);
		}
	}

	time_t fin_c;
	time(&fin_c);
	cout << "Tiempo de Catalogacion:" << fin_c-inicio_c << endl;
}

::uint32_t total_archivos (config_catalogar& configuracion)
{
	::uint32_t total=0;
	GuruDirectorio::iterator_recursivo itfinal;

	for (GuruDirectorio::iterator_recursivo it(configuracion.directorio);it != itfinal;++it)
		if(regex_search(it->sacar_nombre(), configuracion.expresion)) total++;

	return total;
}

int crear_db(config_catalogar& configuracion)
{
	ModeloCatalogo modelo(configuracion.basededatos);

	try{
		if (GuruDirectorio::propiedades_archivo(configuracion.basededatos).Existe()) modelo.borrar_bdd();
	}catch(IBPP::Exception& e) {cout << e.what();}

	modelo.crear_bdd();

	return 0;
}

bool listar_repetidos(config_catalogar& configuracion)
{
	ModeloCatalogo modelo(configuracion.basededatos);


	return true;
}

void dump_txt(config_catalogar& configuracion)
{
	ModeloCatalogo modelo(configuracion.basededatos);
	iterator_fdb itfinal(modelo);
	string c("select b.NOMBRE, a.NOMBRE \
				from ARCHIVO a, DIRECTORIO b, CATALOGO c, ETIQUETA d \
				where c.ID_ARCHIVO = a.ID_ARCHIVO and c.ID_DIRECTORIO = b.ID_DIRECTORIO \
	    			and c.ID_ETIQUETA = d.ID_ETIQUETA");

	for (iterator_fdb it(modelo, c);it!=itfinal;++it) {
		cout << it->columna(1).como<string>() << it->columna(2).como<string>() << endl;
	}
}
