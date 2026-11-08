#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/progress.hpp>
#include <GuruCompat.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <ConfigCatalogar.h>
#include <contararchivos.h>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

void cargar_archivos(config_catalogar& configuracion, std::map<std::string, dword>& directorios, std::multimap<dword, std::string>& archivos, boost::progress_display* display);
// dword total_archivos (config_catalogar& configuracion);

int main(int argc, char* argv[])
{
	boost::progress_timer t( std::clog );
	std::string			directorio;
	config_catalogar	configuracion;

	try{
		po::options_description desc("Parametros validos");
		desc.add_options()
			("ayuda", "Esta pantalla")
			("directorio,d", po::value<string>(), "Directorio a buscar archivos")
			("filtro,f", po::value<string>(), "Expresion regular para catalogar archivos.")
        ;

		po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    
        if (vm.count("ayuda")) {
			std::cout << "Uso: \n";
			std::cout << desc;
            return 0;
        }

		if (!vm.count("directorio")) throw string("Falta especificar el directorio a catalogar.");
		configuracion.directorio = vm["directorio"].as<string>();

		if (vm.count("filtro")) 
			configuracion.filtro = vm["filtro"].as<string>();
		
		configuracion.corregir_parametros();

		std::map<std::string, dword> directorios;
		std::multimap<dword, std::string> archivos;

		std::cout << "Contando Archivos ..." << std::endl;
		dword total = total_archivos(configuracion);
		std::cout << "Archivos Encontrados: " << total << std::endl;

		std::cout << "Cargando ..." << std::endl;
		boost::progress_display a(total, std::cout);

		cargar_archivos(configuracion, directorios, archivos, &a);

		std::cout << "Total archivos cargados: " << archivos.size() << std::endl;
		std::cout << "Total directorios cargados: " << directorios.size() << std::endl;
/*
		dword kb = 0;
		for (std::multimap<dword, std::string>::iterator it=archivos.begin();it!=archivos.end();++it)
			{
			kb += it->second.tamanio;
		//	std::cout << it->first << " " << it->second << std::endl;
			}

		std::cout << "Total Kb encontrados: " << directorios.size() << std::endl;
*/
		std::cout << std::endl << "Tiempo transcurrido: ";

	}catch(std::exception& e) {
		std::cerr << e.what() << "\n";
		return 1;
	}catch(const std::string& e) {
		std::cerr << e << "\n";
		return 1;
	}


	return 0;
}

void cargar_archivos(config_catalogar& configuracion, std::map<std::string, dword>& directorios, std::multimap<dword, std::string>& archivos, boost::progress_display* display)
{
	dword indice=1;
	std::map<std::string, dword>::iterator lugar;
	fs::recursive_directory_iterator itfinal;
	boost::smatch what;

	for ( fs::recursive_directory_iterator it(configuracion.directorio);it != itfinal;++it) {
		// si existe el indice del directorio
		if (is_regular(it->path())){
			if(regex_search(it->leaf(), what, configuracion.expresion)) {
				if ((lugar = directorios.find(it->path().branch_path().string())) != directorios.end())
					archivos.insert(make_pair(lugar->second, it->path().leaf()));
				else{ // si no existe el indice del directorio agrego el directorio y archivo
					directorios.insert(make_pair(it->path().branch_path().string(), indice));
					archivos.insert(make_pair(indice++, it->path().leaf()));
				}
			++(*display);
			}
		}
	}
}
