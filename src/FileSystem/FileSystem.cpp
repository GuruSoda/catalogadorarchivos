#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
	fs::path laruta("directorio/guru.zip");

	std::cout << "observers de " << std::endl;
	std::cout << "string() : " << laruta.string() << std::endl;
//	std::cout << "file_string() : " << laruta.file_string() << std::endl;
//	std::cout << "directory_string() : " << laruta.directory_string() << std::endl;

	std::cout << "root_name() : " << laruta.root_name() << std::endl;
	std::cout << "root_directory() : " << laruta.root_directory() << std::endl;
	std::cout << "root_path() : " << laruta.root_path() << std::endl;
	std::cout << "relative_path() : " << laruta.relative_path() << std::endl;
	std::cout << "leaf() : " << laruta.leaf() << std::endl;
	std::cout << "branch_path() : " << laruta.branch_path() << std::endl;

	std::cout << "empty() : " << laruta.empty() << std::endl;
	std::cout << "is_complete() : " << laruta.is_complete() << std::endl;
	std::cout << "has_root_name() : " << laruta.has_root_name() << std::endl;
	std::cout << "has_root_directory() : " << laruta.has_root_directory() << std::endl;
	std::cout << "has_root_path() : " << laruta.has_root_path() << std::endl;
	std::cout << "has_relative_path() : " << laruta.has_relative_path() << std::endl;
	std::cout << "has_leaf() : " << laruta.has_leaf() << std::endl;
	std::cout << "has_branch_path() : " << laruta.has_branch_path() << std::endl;

	return 0;
}
