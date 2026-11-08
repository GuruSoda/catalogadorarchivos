#include <GuruSHA1.h>
#include <GuruSHA2.h>
#include <GuruSHA4.h>
#include <GuruCRC.h>

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Archivo: " << argv[0] << endl; 
	cout << "CRC32:" << GuruCRC::archivo_crc32(argv[0]) << endl;
	cout << "sha1:" << GuruSHA1::archivo_sha1(argv[0]) << endl;

	cout << "Soda Stereo en Checksum" << endl;
	cout << "sha1:" << GuruSHA1::sha1((byte*)"Soda Stereo\n", strlen("Soda Stereo\n")) << endl;
	cout << "sha224:" << GuruSHA2::sha224((byte*)"Soda Stereo\n", strlen("Soda Stereo\n")) << endl;
	cout << "sha256:" << GuruSHA2::sha256((byte*)"Soda Stereo\n", strlen("Soda Stereo\n")) << endl;
	cout << "sha384:" << GuruSHA4::sha384((byte*)"Soda Stereo\n", strlen("Soda Stereo\n")) << endl;
	cout << "sha512:" << GuruSHA4::sha512((byte*)"Soda Stereo\n", strlen("Soda Stereo\n")) << endl;

	return 0;
}

