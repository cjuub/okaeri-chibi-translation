#ifndef NCGR_H
#define NCGR_H

#include <string>
#include <vector>

class NCGR {
public:
	NCGR();
	void load_tiles(std::string& in_file);
	std::vector<uint8_t>& get_tile_data();
	void set_tile_data(int index, uint8_t val);
	void save(std::string& out_file, std::vector<uint8_t>& tile_data_mod);
private:
	void copy_until(std::ifstream& ifs, std::ofstream& ofs, int pos);
	std::string file_name;
	std::vector<uint8_t> tile_data;

	uint32_t file_size;
};

#endif