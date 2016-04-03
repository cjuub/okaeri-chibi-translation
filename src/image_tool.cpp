#include "ncgr.h"
#include "ncer.h"
#include "nscr.h"
#include "nsbmd.h"

#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	string option(argv[1]);

	if (option == "-e") {
		NCGR ncgr;
		NCER ncer;
		
		string ncgr_file(argv[2]);
		string ncer_file(argv[3]);
		string out_folder(argv[4]);

		ncgr.load_tiles(ncgr_file);

		ncer.extract_cells(ncer_file, ncgr, out_folder);
	} else if (option == "-e2") {
		NCGR ncgr;
		NCER ncer;
		NSCR nscr;
		
		string ncgr_file(argv[2]);
		string ncer_file(argv[3]);
		string out_folder(argv[4]);

		ncgr.load_tiles(ncgr_file);

		nscr.extract_image(ncer_file, ncgr, out_folder);
	} else if (option == "-e3") {
		NSBMD nsbmd;

		string nsbmd_file(argv[2]);
		string out_folder(argv[3]);

		nsbmd.extract_textures(nsbmd_file, out_folder);	
	} else if (option == "-i") {
		NCGR ncgr;
		NCER ncer;
		
		string ncgr_file(argv[2]);
		string ncer_file(argv[3]);
		string bmp_folder(argv[4]);
		string ncgr_out(argv[5]);
		string ncer_out(argv[6]);

		ncgr.load_tiles(ncgr_file);

		ncer.insert_cells(ncer_file, ncgr, bmp_folder, ncgr_out, ncer_out);
	} else if (option == "-i2") {
		NCGR ncgr;
		NCER ncer;
		NSCR nscr;

		string ncgr_file(argv[2]);
		string ncer_file(argv[3]);
		string bmp_folder(argv[4]);
		string ncgr_out(argv[5]);
		string nscr_out(argv[6]);

		ncgr.load_tiles(ncgr_file);

		nscr.insert_image(ncer_file, ncgr, bmp_folder, ncgr_out, nscr_out);
	}
}
