#include "ncgr.h"
#include "ncer.h"
#include "nscr.h"

#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	NCGR ncgr;
	string ncgr_file(argv[2]);

	NCER ncer;
	NSCR nscr;
	string ncer_file(argv[3]);

	ncgr.load_tiles(ncgr_file);
	string option(argv[1]);
	string bmp_folder(argv[4]);

	if (option == "-e") {
		ncer.extract_cells(ncer_file, ncgr, bmp_folder);
	} else if (option == "-e2") {
		nscr.extract_image(ncer_file, ncgr, bmp_folder);
	} else if (option == "-i") {
		string ncgr_out(argv[5]);
		string ncer_out(argv[6]);
		ncer.insert_cells(ncer_file, ncgr, bmp_folder, ncgr_out, ncer_out);
	} else if (option == "-i2") {
		string ncgr_out(argv[5]);
		nscr.insert_image(ncer_file, ncgr, bmp_folder, ncgr_out);
	}
}
