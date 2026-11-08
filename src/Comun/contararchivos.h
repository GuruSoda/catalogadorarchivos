#include <boost/filesystem.hpp>
#include <GuruCompat.h>
#include <ConfigCatalogar.h>

namespace fs = boost::filesystem;

dword total_archivos (config_catalogar& configuracion)
{
	dword total=0;
	fs::recursive_directory_iterator itfinal;

	boost::smatch what;
	fs::path p(configuracion.directorio);
	for (fs::recursive_directory_iterator it(p);it != itfinal;++it)
		if (is_regular(it->status()))
			{
			if(regex_search(it->leaf(), what, configuracion.expresion)) {
				if (it->leaf().compare("svnscc.msi") == 0)
					cout << it->leaf() << endl;
				total++;
			}
		}else cout << it->path().string() << endl;

	return total;
}
