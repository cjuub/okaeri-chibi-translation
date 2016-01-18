#include "ncer.h"
#include "lodepng.h"
#include "little_endian.h"

#include <fstream>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

NCER::NCER() {}

void NCER::insert_cells(std::string& ncer_file, NCGR& ncgr, std::string& bmp_folder, std::string& out_file) {
	ifstream ifs(ncer_file);

	ifs.seekg(0x18);
	uint16_t nbr_banks = read_little_endian_16(ifs);
	ifs.seekg(0x30);

	for (unsigned int i = 0; i != nbr_banks; ++i) {
		uint16_t nbr_oam = read_little_endian_16(ifs);
		nbr_oams.push_back(nbr_oam);
		read_little_endian_16(ifs);
		uint32_t oam_offset = read_little_endian_32(ifs);
		oam_offsets.push_back(oam_offset);
		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
	}

	while (ifs.tellg() != EOF) {
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
	}

	vector<uint8_t> tile_data_mod;
	tile_data_mod.resize(ncgr.get_tile_data().size());

	for (unsigned int i = 0; i != nbr_banks; ++i) {
		int width;
		int height;
		uint32_t offs = oam_offsets[i] / 2;
		unsigned img_w = 512;
		unsigned img_h = 256;
		int img_offs_x = img_w / 2;
		int img_offs_y = img_h / 2;


		ifstream check(bmp_folder + "/" + SSTR(i) + ".png");
		bool exists = false;
		if (check.good()) {
			exists = true;
		}

		check.close();

		vector<unsigned char> image;
		if (exists) {
			lodepng::decode(image, img_w, img_h, bmp_folder + "/" + SSTR(i) + ".png");
		}

		ifstream meta(bmp_folder + "/" + SSTR(i) + ".meta");
		bool meta_exists = false;
		if (meta.good()) {
			meta_exists = true;
		} else {
			meta.close();
		}

		int x_min = 0;
		int y_min = 0;
		int x_max = 511;
		int y_max = 255;

		vector<int> oams_change;
		if (meta_exists) {
			meta >> x_min;
			meta >> y_min;
			meta >> x_max;
			meta >> y_max;

			int oam;
			while (meta >> oam) {
				oams_change.push_back(oam);
			}
		}

		meta.close();

		for (int j = nbr_oams[i] - 1; j != -1; --j) {
			uint16_t obj_atr_0 = oam_data[offs + j * 3];
			uint16_t obj_atr_1 = oam_data[offs + j * 3 + 1];
			uint16_t obj_atr_2 = oam_data[offs + j * 3 + 2];

			int shape = (obj_atr_0 & 0xC000) >> 14;
			int size = (obj_atr_1 & 0xC000) >> 14;

			width = size_table[size][shape][0];
			height = size_table[size][shape][1];

			int tile_index = ((obj_atr_2 & 0x3FF) << 1);

			int16_t oam_x = obj_atr_1 & 0x1FF;
			if (oam_x & 0x100) {
				oam_x |= 0xFE00;
			}

			int8_t oam_y = obj_atr_0 & 0xFF;

			bool modify_oam = false;
			if (meta_exists) {
				for (auto it = oams_change.begin(); it != oams_change.end(); ++it) {
					if (*it == nbr_oams[i] - 1 - j) { // oams seem to be in reverse order compared to tinke?
						modify_oam = true;
					}
				}
			} else {
				modify_oam = true;
			}

			for (int y = 0; y < height; y += 8) {
				for (int x = 0; x < width; x += 8) {
					for (int tileY = 0; tileY < 8; tileY++) {
						for (int tileX = 0; tileX < 8; tileX++) {
							int tile_data_index = tile_index * 8 * 8 + x * 8 + tileX + y * width + tileY * 8;
							int img_x = img_offs_x + oam_x + tileX + x;
							int img_y = img_offs_y + oam_y + tileY + y;
							int pos = img_x * 4 + img_y * img_w * 4;

							if (exists && modify_oam && img_x >= x_min && img_x <= x_max && img_y >= y_min && img_y <= y_max) {
								tile_data_mod[tile_data_index] = image[pos];
							} else {
								tile_data_mod[tile_data_index] = ncgr.get_tile_data()[tile_data_index];
							}
						}
					}
				}
			}
		}
	}

	ncgr.save(out_file, tile_data_mod);
	ifs.close();
}

void NCER::extract_cells(std::string& in_file, NCGR& ncgr, std::string& out_folder) {
	vector<uint8_t>& tile_data = ncgr.get_tile_data();
	ifstream ifs(in_file);

	ifs.seekg(0x18);
	uint16_t nbr_banks = read_little_endian_16(ifs);
	ifs.seekg(0x30);

	for (unsigned int i = 0; i != nbr_banks; ++i) {
		uint16_t nbr_oam = read_little_endian_16(ifs);
		nbr_oams.push_back(nbr_oam);
		read_little_endian_16(ifs);
		uint32_t oam_offset = read_little_endian_32(ifs);
		oam_offsets.push_back(oam_offset);

		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
		read_little_endian_16(ifs);
	}

	while (ifs.tellg() != EOF) {
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
	}

	for (unsigned int i = 0; i != nbr_banks; ++i) {
		int width;
		int height;
		uint32_t offs = oam_offsets[i] / 2;
		int img_w = 512;
		int img_h = 256;
		int img_offs_x = img_w / 2;
		int img_offs_y = img_h / 2;

		vector<unsigned char> image(img_w * img_h * 4);
		
		int img_x = 0;
		int img_y = 0;

		for (int j = nbr_oams[i] - 1; j != -1; --j) {
			uint16_t obj_atr_0 = oam_data[offs + j*3];
			uint16_t obj_atr_1 = oam_data[offs + j*3 + 1];
			uint16_t obj_atr_2 = oam_data[offs + j*3 + 2];
			int shape = (obj_atr_0 & 0xC000) >> 14;
			int size = (obj_atr_1 & 0xC000) >> 14;
			width = size_table[size][shape][0];
			height = size_table[size][shape][1];
			int tile_index = ((obj_atr_2 & 0x3FF) << 1);
			int16_t oam_x = obj_atr_1 & 0x1FF;

			if (oam_x & 0x100) {
				oam_x |= 0xFE00;
			}

			int8_t oam_y = obj_atr_0 & 0xFF;

			for (int y = 0; y < height; y += 8) {
				for (int x = 0; x < width; x += 8) {
					for (int tileY = 0; tileY < 8; tileY++) {
						for (int tileX = 0; tileX < 8; tileX++) {
							uint8_t r = tile_data[tile_index * 8 * 8 + x * 8 + tileX + y * width + tileY * 8];
							int pos = (img_offs_x + oam_x + tileX + x) * 4 + (img_offs_y + oam_y + tileY + y) * img_w * 4;
							image[pos + 0] = r;
							image[pos + 1] = r;
							image[pos + 2] = r;
							image[pos + 3] = 255;
						}
					}
				}
			}

			img_x = (img_x + 8) % width;
			if (img_x == 0) {
				img_y += 8;
			}
		}

		lodepng::encode(out_folder + "/" + SSTR(i) + ".png", image, img_w, img_h);
	}

	ifs.close();
}
