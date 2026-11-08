#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <string>
#include <vector>

#include <ModeloCatalogo.h>

namespace po = boost::program_options;

using namespace boost;
using namespace std;

int main(int argc, char* argv[])
{
	try {
		po::options_description desc("Parametros Validos");
		desc.add_options()
			("ayuda,h", "Esta pantalla.")
			("bdd", po::value<string>(), "Base de Datos a consultar.")
			("directorio,d", po::value<string>(), "Busca en un directorio.")
			("etiqueta,e", po::value<string>(), "Etiqueta del catalogo.")
			("md5", po::value<string>(), "Busca por MD5.")
			("sha1", po::value<string>(), "Busca por SHA1.")
			("sha512", po::value<string>(), "Busca por SHA512.")
			("nombre", po::value<string>(), "Busca por el nombre del archivo.")
			("tamanio", po::value<string>(), "Busca por un tamanio.")
			("fecha", po::value<string>(), "Busca por una fecha determinada.")
			("columnas,c", po::value<string>(), "Columnas a mostrar.")
			("separador,s", po::value<string>(), "Separador de columnas.")
			("repetidos,r", "Lista los archivos repetidos.")
			("iguales,i", "Lista los archivos iguales.")
			("sub,s",  po::value<string>(), "Lista los SubDirectorios.")

		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.size() == 0 || vm.count("ayuda")) {cout << desc << endl; return 1;};

		if (!vm.count("bdd")) throw string("Falta especificar la base de datos.");

		string	bd, directorio, etiqueta, tamanio, crc32, sha1, sha512, nombre, fecha;
		string	columnas,separador;

		bd = vm["bdd"].as<string>();

		if (vm.count("directorio")) directorio = vm["directorio"].as<string>();
		if (vm.count("etiqueta")) etiqueta = vm["etiqueta"].as<string>();
		if (vm.count("md5")) crc32 = vm["md5"].as<string>();
		if (vm.count("sha1")) sha1 = vm["sha1"].as<string>();
		if (vm.count("sha512")) sha512 = vm["sha512"].as<string>();
		if (vm.count("nombre")) nombre = vm["nombre"].as<string>();
		if (vm.count("tamanio")) tamanio = vm["tamanio"].as<string>();
		if (vm.count("fecha")) fecha = vm["fecha"].as<string>();
		if (vm.count("columnas")) columnas = vm["columnas"].as<string>();
		else columnas = "etiqueta,nombre,tamanio,sha1,directorio";
		if (vm.count("separador")) separador = vm["separador"].as<string>();
		else separador = " | ";

		ModeloCatalogo modelo(bd);

		if (vm.count("sub")) {
			vector<string> subs;
			modelo.subdirectorios(vm["etiqueta"].as<string>(), vm["sub"].as<string>() + "%", subs);

			for (vector<string>::iterator it=subs.begin();it!=subs.end();++it)
				cout << *it << endl;

			return 0;
		}


		if (vm.count("repetidos")) {
			vector<voArchivo> voa;
			modelo.repetidos(voa);

			for (vector<voArchivo>::iterator it=voa.begin();it!=voa.end();++it)
				cout << it->directorio << ((it->directorio.compare("/") == 0) ? "" : "/") << it->nombre << endl;

			return 0;
		}

		if (vm.count("iguales")) {
			map<voArchivo, vector<voArchivo> > voa;
			modelo.iguales(voa);

			for (map<voArchivo, vector<voArchivo> >::iterator it=voa.begin();it!=voa.end();++it){
				cout << it->first.tamanio << "," << it->first.md5 << "," << it->first.sha1 << endl;
				for (vector<voArchivo>::iterator it2=it->second.begin();it2!=it->second.end();++it2)
					cout << "\t" << it2->directorio << ((it2->directorio.compare("/") == 0) ? "" : "/") << it2->nombre << endl;
			}
			return 0;
		}

//		tokenizer<escaped_list_separator<char> > token(columnas);

		boost::char_separator<char> separador_columnas(",");
		tokenizer<char_separator<char> > token(columnas, separador_columnas);
		string columna;

		for (ModeloCatalogo::iterator it = modelo.begin(etiqueta, directorio, nombre, tamanio, fecha, crc32, sha1, sha512);it!=modelo.end(); ++it){
			for(tokenizer<char_separator<char> >::iterator it2=token.begin(); it2!=token.end();++it2){
				columna = *it2;
				if (columna.compare("etiqueta") == 0) {
					cout << it->etiqueta + separador;
					continue;
				}else if (columna.compare("nombre") == 0) {
					cout << it->nombre + separador;
					continue;
				}else if (columna.compare("tamanio") == 0) {
					cout << it->tamanio << separador;
					continue;
				}else if (columna.compare("md5") == 0) {
					cout << it->md5 + separador;
					continue;
				}else if (columna.compare("sha1") == 0) {
					cout << it->sha1 + separador;
					continue;
				}else if (columna.compare("sha512") == 0) {
					cout << it->sha512 + separador;
					continue;
				}else if (columna.compare("fecha") == 0) {
					cout << "Sin Fecha" + separador;
					continue;
				}else if (columna.compare("directorio") == 0) {
					cout << it->directorio + separador;
					continue;
				}
			}

			cout << endl;
//			cout << it->etiqueta << " | " << it->nombre << " | " << it->tamanio <<  " | " << it->sha1 << " | " << it->directorio << endl;
//			cout << registro << endl;
		}

	}catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}
