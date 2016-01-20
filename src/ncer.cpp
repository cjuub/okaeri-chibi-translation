#include "ncer.h"
#include "lodepng.h"
#include "little_endian.h"

#include <fstream>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

NCER::NCER() {}

void NCER::insert_cells(std::string& ncer_file, NCGR& ncgr, std::string& bmp_folder, std::string& ncgr_out, std::string& ncer_out) {
	file_name = ncer_file;

	ifstream ifs(ncer_file);

	ifs.seekg(0x18);
	nbr_banks = read_little_endian_16(ifs);
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

	oam_start_addr = ifs.tellg();

	while (ifs.tellg() != EOF) {
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
		oam_data.push_back(read_little_endian_16(ifs));
	}

	vector<uint8_t> tile_data_mod;
	tile_data_mod.resize(ncgr.get_tile_data().size());

	vector<uint16_t> oam_data_mod;
	oam_data_mod.resize(oam_data.size());

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

		int custom_oam_tile;
		int oam_x_shift;
		int oam_y_shift;

		bool has_custom_oam = false;
		vector<int> oams_change;
		if (meta_exists) {
			string line;
			getline(meta, line);
			istringstream oam_info(line);

			oam_info >> x_min;
			oam_info >> y_min;
			oam_info >> x_max;
			oam_info >> y_max;

			int oam;
			while (oam_info >> oam) {
				oams_change.push_back(oam);
			}

			if (getline(meta, line)) {
				istringstream extend_info(line);
				has_custom_oam = true;
				extend_info >> custom_oam_tile;
				extend_info >> oam_x_shift;
				extend_info >> oam_y_shift;
			}
		}

		meta.close();

		for (int j = nbr_oams[i] - 1; j != -1; --j) {
			uint16_t obj_atr_0 = oam_data[offs + j * 3];
			uint16_t obj_atr_1 = oam_data[offs + j * 3 + 1];
			uint16_t obj_atr_2 = oam_data[offs + j * 3 + 2];

			oam_data_mod[offs + j * 3] = obj_atr_0;
			oam_data_mod[offs + j * 3 + 1] = obj_atr_1;
			oam_data_mod[offs + j * 3 + 2] = obj_atr_2;

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

				if (has_custom_oam && modify_oam) {
					obj_atr_0 |= 1 << 14;
					obj_atr_0 &= ~(1 << 15);

					obj_atr_1 &= ~(1 << 14);
					obj_atr_1 |= 1 << 15;

					// 1 tile index includes 2 consecutive tiles
					int CUSTOM_SIZE = 8 / 2; // 32x16 = 8 tiles
					tile_index = (ncgr.get_tile_data().size()  / (8 * 8 * 2)) + custom_oam_tile * CUSTOM_SIZE;
					obj_atr_2 &= 0xFC00;
					obj_atr_2 += tile_index;


					oam_x += static_cast<int16_t>(oam_x_shift);
					oam_y += static_cast<int16_t>(oam_y_shift);

					obj_atr_1 &= 0xFE00;
					obj_atr_1 += static_cast<uint16_t>(oam_x & 0x01FF);

					oam_data_mod[offs + j * 3] = obj_atr_0;
					oam_data_mod[offs + j * 3 + 1] = obj_atr_1;
					oam_data_mod[offs + j * 3 + 2] = obj_atr_2;

					width = 32;
					height = 16;
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

							if (exists && modify_oam && has_custom_oam) {
								img_x = x_min + tileX + x;
								img_y = y_min + tileY + y;
								pos = img_x * 4 + img_y * img_w * 4;

								tile_data_mod.push_back(image[pos]);
							} else if (exists && modify_oam && img_x >= x_min && img_x <= x_max && img_y >= y_min && img_y <= y_max) {
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

	save(ncer_out, oam_data_mod);
	ncgr.save(ncgr_out, tile_data_mod);
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

void NCER::save(string& ncer_out, vector<uint16_t>& oam_data_mod) {
	ifstream ifs(file_name);
	ofstream ofs(ncer_out);

	copy_until(ifs, ofs, oam_start_addr);

	for (unsigned i = 0; i < oam_data_mod.size(); ++i) {
		write_little_endian_16(ofs, oam_data_mod[i]);
	}

	copy_until(ifs, ofs, EOF);

	ofs.close();
	ifs.close();
}
