#ifndef NCER_H
#define NCER_H

#include "ncgr.h"

#include <string>
#include <vector>

class NCER {
public:
	NCER();
	void insert_cells(std::string& ncer_file, NCGR& ncgr, std::string& bmp_folder, std::string& out_file, std::string& ncer_out);
	void extract_cells(std::string& in_file, NCGR& ncgr, std::string& out_folder);
	void write_bmp(std::vector<std::vector<uint8_t>>& img_data, int width, int height, std::string& id);
	void save(std::string& ncer_out, std::vector<uint16_t>& oam_data_mod);
private:
	const int TILE_INDEX_MULTIPLIER = 2;

	std::string file_name;
	uint16_t nbr_banks;
	std::vector<uint16_t> nbr_oams;
	std::vector<uint32_t> oam_offsets;
	std::vector<uint16_t> oam_data;

	int size_table[4][3][2] = {{{8, 8}, {16, 8}, {8, 16}},
								{{16, 16}, {32, 8}, {8, 32}},
								{{32, 32}, {32, 16}, {16, 32}},
								{{64, 64}, {64, 32}, {32, 64}}};

	int oam_start_addr;
};

#endif