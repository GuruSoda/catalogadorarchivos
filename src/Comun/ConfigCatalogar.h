#ifndef __CONFIG_CATALOGAR_H__
#define __CONFIG_CATALOGAR_H__

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <string>

using namespace boost;
using namespace std;

class config_catalogar
{
public:
	config_catalogar() {crc=true;sha1=true;sha512=false;filtro="^.*$";};

	void corregir_parametros() {
		trim(etiqueta);trim(directorio);
		trim(basededatos);trim(filtro);

		if (!ends_with(directorio, "\\") && !ends_with(directorio, "/"))
			directorio += "/";

		replace_all(directorio, "\\", "/");

		// boost::regex::basic
		// boost::regex::extended
		// boost::regex::grep
		expresion.set_expression(filtro, boost::regex::extended|boost::regex::icase);
	};

// atributos;
	string	basededatos;
	string	directorio;
	string	etiqueta;
	bool	crc;
	bool	sha1;
	bool	sha512;
	
	string	filtro;
	boost::regex expresion;
};

#endif

