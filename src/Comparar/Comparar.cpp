#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/progress.hpp>
#include <string>

#include <ModeloCatalogo.h>
#include <ConfigCatalogar.h>
#include <GuruIBPP.h>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace boost;
using namespace std;

void procesar_parametros(int argc, char* argv[], po::variables_map& vm, config_catalogar& configuracion)
{
	po::options_description desc("Parametros Validos");
	desc.add_options()
		("ayuda,h", "Esta pantalla.")
		("bd", po::value<string>(), "Base de Datos a guardar el catalogo.")
		("directorio,d", po::value<string>(), "Directorio a catalogar.")
		("etiqueta,e", po::value<string>(), "Etiqueta del nuevo catalogo.")
		("bd2dir", "Compara el catalogo contra el directorio.")
		("dir2bd", "Comparar el directorio contra el catalogo.")
		("nocrc", "No verifica el CRC de cada archivo (CRC=0).")
		("nosha1", "No verifica el sha1 de cada archivo (SHA1=0).")
		("filtro,f", po::value<string>(), "Expresion regular para catalogar archivos.")
		("excluir-dir", po::value<string>(), "Expresion regular para excluir directorios.")
		("excluir-archivos", po::value<string>(), "Expresion regular para excluir archivos.")
	;

//	po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

	if (vm.size() == 0 || vm.count("ayuda")) cout << desc << endl;

	if (!vm.count("bd")) throw string("Falta especificar la base de datos.");
	else configuracion.basededatos = vm["bd"].as<string>();

	if (!vm.count("directorio")) throw string("Falta especificar el directorio a catalogar.");

	if (!vm.count("etiqueta")) throw string("Falta especificar la etiqueta del nuevo catalogo.");

	configuracion.directorio = vm["directorio"].as<string>();
	configuracion.etiqueta = vm["etiqueta"].as<string>();
	configuracion.crc = (vm.count("nocrc")) ? false : true;
	configuracion.sha1 = (vm.count("nosha1")) ? false : true;

	if (vm.count("filtro")) configuracion.filtro = vm["filtro"].as<string>();

	configuracion.corregir_parametros();

	cout << "Base de Datos: " << configuracion.basededatos << endl;
	cout << "Directorio: " << configuracion.directorio << endl;
	cout << "Etiqueta: " << configuracion.etiqueta << endl;
}

void consultar(config_catalogar& configuracion)
{
	GuruIBPP it("select user_agent \"Agente\" from user_agent;", "D:\\Portal\\cdcs.fdb");

	while(it)
		{
		}
}

int main(int argc, char* argv[])
{
	config_catalogar	configuracion;

	try{
		consultar(configuracion);

		po::variables_map vm;

		procesar_parametros(argc, argv, vm, configuracion);

		consultar(configuracion);
	}catch(std::exception& e) {
		std::cout << e.what() << "\n";
        return 1;
    }catch(std::string& e) {
		std::cout << e << "\n";
        return 1;
    }

	return 0;
}
