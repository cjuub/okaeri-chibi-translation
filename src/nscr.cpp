#include "nscr.h"
#include "lodepng.h"
#include "little_endian.h"

#include <fstream>

using namespace std;

NSCR::NSCR() {}

void NSCR::insert_image(std::string& nscr_file, NCGR& ncgr, std::string& in_file, std::string& ncgr_out) {
	ifstream ifs(nscr_file);

	ifs.seekg(0x14);
	read_little_endian_32(ifs); // section_size
	uint16_t width = read_little_endian_16(ifs);
	read_little_endian_16(ifs); // height
	read_little_endian_16(ifs); // internal_screen_size
	read_little_endian_16(ifs); // bg_type
	uint32_t data_size = read_little_endian_32(ifs);

	vector<uint8_t> tile_data_mod;
	tile_data_mod.resize(ncgr.get_tile_data().size());

	vector<unsigned char> image;
	unsigned tmp, tmp2;
	lodepng::decode(image, tmp, tmp2, in_file);

	int img_x = 0;
	int img_y = 0;

	if (data_size == 0) {
		return;
	}

	for (uint32_t i = 0; i != data_size / 2; ++i) {
		uint16_t data = read_little_endian_16(ifs);

		int tile_index = data & 0x3FF;
		bool h_flip = data & 0x400;
		bool v_flip = data & 0x800;
		// int palette_index = data & 0xF000;

		if (h_flip && v_flip) {
			for (int tileY = 7; tileY != -1; --tileY) {
				for (int tileX = 7; tileX != -1; --tileX) {
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					tile_data_mod[tile_index * 8 * 8  + 7 - tileX + (7 - tileY) * 8] = image[pos];
				}
			}
		} else if (h_flip) {
			for (int tileY = 0; tileY < 8; tileY++) {
				for (int tileX = 7; tileX != -1; --tileX) {
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					tile_data_mod[tile_index * 8 * 8  + 7 - tileX + tileY * 8] = image[pos];
				}
			}
		} else if (v_flip){
			for (int tileY = 7; tileY != -1; --tileY) {
				for (int tileX = 0; tileX < 8; tileX++) {
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					tile_data_mod[tile_index * 8 * 8  + tileX + (7 - tileY) * 8] = image[pos];
				}
			}			
		} else {
			for (int tileY = 0; tileY < 8; tileY++) {
				for (int tileX = 0; tileX < 8; tileX++) {
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					tile_data_mod[tile_index * 8 * 8  + tileX + tileY * 8] = image[pos];
				}
			}
		}

		img_x = (img_x + 8) % width;
		if (img_x == 0) {
			img_y += 8;
		}
	}

	ncgr.save(ncgr_out, tile_data_mod);
	
	ifs.close();
}

void NSCR::extract_image(std::string& in_file, NCGR& ncgr, std::string& out_file) {
	ifstream ifs(in_file);

	ifs.seekg(0x14);
	read_little_endian_32(ifs); // section_size
	uint16_t width = read_little_endian_16(ifs);
	uint16_t height = read_little_endian_16(ifs);
	read_little_endian_16(ifs); // internal_screen_size
	read_little_endian_16(ifs); // bg_type
	uint32_t data_size = read_little_endian_32(ifs);

	vector<unsigned char> image(width * height * 4);

	int img_x = 0;
	int img_y = 0;

	for (uint32_t i = 0; i != data_size / 2; ++i) {
		uint16_t data = read_little_endian_16(ifs);

		int tile_index = data & 0x3FF;
		bool h_flip = data & 0x400;
		bool v_flip = data & 0x800;
		// int palette_index = data & 0xF000;

		if (h_flip && v_flip) {
			for (int tileY = 7; tileY != -1; --tileY) {
				for (int tileX = 7; tileX != -1; --tileX) {
					uint8_t r = ncgr.get_tile_data()[tile_index * 8 * 8  + 7 - tileX + (7 - tileY) * 8];
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					image[pos + 0] = r;
					image[pos + 1] = r;
					image[pos + 2] = r;
					image[pos + 3] = 255;

				}
			}
		} else if (h_flip) {
			for (int tileY = 0; tileY < 8; tileY++) {
				for (int tileX = 7; tileX != -1; --tileX) {
					uint8_t r = ncgr.get_tile_data()[tile_index * 8 * 8  + 7 - tileX + tileY * 8];
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					image[pos + 0] = r;
					image[pos + 1] = r;
					image[pos + 2] = r;
					image[pos + 3] = 255;

				}
			}
		} else if (v_flip){
			for (int tileY = 7; tileY != -1; --tileY) {
				for (int tileX = 0; tileX < 8; tileX++) {
					uint8_t r = ncgr.get_tile_data()[tile_index * 8 * 8  + tileX + (7 - tileY) * 8];
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					image[pos + 0] = r;
					image[pos + 1] = r;
					image[pos + 2] = r;
					image[pos + 3] = 255;

				}
			}			
		} else {
			for (int tileY = 0; tileY < 8; tileY++) {
				for (int tileX = 0; tileX < 8; tileX++) {
					uint8_t r = ncgr.get_tile_data()[tile_index * 8 * 8  + tileX + tileY * 8];
					int pos = (img_x + tileX) * 4 + (img_y + tileY) * width * 4;
					image[pos + 0] = r;
					image[pos + 1] = r;
					image[pos + 2] = r;
					image[pos + 3] = 255;

				}
			}			
		}

		img_x = (img_x + 8) % width;
		if (img_x == 0) {
			img_y += 8;
		}
	}

	lodepng::encode(out_file, image, width, height);
}
