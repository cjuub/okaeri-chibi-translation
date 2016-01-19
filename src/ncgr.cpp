#include "ncgr.h"
#include "little_endian.h"

#include <fstream>

using namespace std;

NCGR::NCGR() {}

void NCGR::load_tiles(std::string& in_file) {
	file_name = in_file;
	ifstream ifs(in_file);
	char b;

	ifs.seekg(0x8);
	file_size = read_little_endian_32(ifs);
	ifs.seekg(0x1C);
	read_little_endian_32(ifs); // bit_depth
	read_little_endian_16(ifs); // height
	read_little_endian_16(ifs); // width
	ifs.get(b); // not_tiled
	ifs.get(b); // is_partitioned
	read_little_endian_16(ifs);
	uint32_t data_size = read_little_endian_32(ifs); // data_size
	read_little_endian_32(ifs); // data_offs

	unsigned i = 0;
	while (i != data_size) {
		i++;
		ifs.get(b);
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

	uint32_t size_diff = tile_data_mod.size() - tile_data.size();

	copy_until(ifs, ofs, 0x8);
	write_little_endian_32(ofs, read_little_endian_32(ifs) + size_diff);
	copy_until(ifs, ofs, 0x14);
	write_little_endian_32(ofs, read_little_endian_32(ifs) + size_diff);
	copy_until(ifs, ofs, 0x28);
	write_little_endian_32(ofs, read_little_endian_32(ifs) + size_diff);
	copy_until(ifs, ofs, 0x30);

	for (unsigned i = 0; i < tile_data_mod.size(); ++i) {
		write_little_endian_8(ofs, tile_data_mod[i]);
	}

	ofs.close();
	ifs.close();
}

void NCGR::copy_until(ifstream& ifs, ofstream& ofs, int pos) {
	while (ifs.tellg() != pos) {
		ofs << static_cast<uint8_t>(ifs.get());
	}
}