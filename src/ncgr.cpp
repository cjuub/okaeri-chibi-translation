#include "ncgr.h"
#include "little_endian.h"

#include <fstream>

using namespace std;

NCGR::NCGR() {}

void NCGR::load_tiles(std::string& in_file) {
	file_name = in_file;
	ifstream ifs(in_file);
	char b;

	ifs.seekg(0x1C);
	read_little_endian_32(ifs); // bit_depth
	read_little_endian_16(ifs); // height
	read_little_endian_16(ifs); // width
	ifs.get(b); // not_tiled
	ifs.get(b); // is_partitioned
	read_little_endian_16(ifs);
	read_little_endian_32(ifs); // data_size
	read_little_endian_32(ifs); // data_offs

	int i = 0;
	while (ifs.get(b)) {
		i++;
		tile_data.push_back(static_cast<uint8_t>(b));
	}

	ifs.close();
}

std::vector<uint8_t>& NCGR::get_tile_data() {
	return tile_data;
}

void NCGR::set_tile_data(int index, uint8_t val) {
	tile_data[index] = val;
}

void NCGR::save(std::string& out_file, std::vector<uint8_t>& tile_data_mod) {
	ifstream ifs(file_name);
	ofstream ofs(out_file);

	while (ifs.tellg() != 0x30) {
		ofs << static_cast<uint8_t>(ifs.get());
	}

	for (unsigned int i = 0; i < tile_data_mod.size(); ++i) {
		ofs << static_cast<uint8_t>(tile_data_mod[i]);
	}

	ofs.close();
	ifs.close();
}
