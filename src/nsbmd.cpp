#include "nsbmd.h"
#include "little_endian.h"
#include "lodepng.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


using namespace std; 

NSBMD::NSBMD() {}

void NSBMD::extract_textures(string& nsbmd_file, string& out_folder) {
	ifstream ifs2(nsbmd_file, ios::binary);
	if (!ifs2) {
		printf("Could not open file: %s\n", nsbmd_file.c_str());
		exit(1);
	}
	
	vector<char> nsbmd_vector((istreambuf_iterator<char>(ifs2)), 
				istreambuf_iterator<char>());

	vector<char> search_string{'T', 'E', 'X', '0'};
	unsigned pos = (search(nsbmd_vector.begin(), nsbmd_vector.end(),
			search_string.begin(), search_string.end())) - nsbmd_vector.begin(); 

	ifs2.close();

	// being lazy here, only have little endian read functions for streams
	ifstream ifs(nsbmd_file, ios::binary);

	ifs.seekg(pos + 0xC);
	unsigned texture_data_size = read_little_endian_16(ifs) << 3;
	uint16_t texture_info_offs = read_little_endian_16(ifs);
	ifs.seekg(pos + 0x14);
	uint32_t texture_data_offs = read_little_endian_32(ifs);
	ifs.seekg(pos + 0x1C);
	unsigned compressed_texture_data_size = read_little_endian_16(ifs) << 3;

	if (compressed_texture_data_size != 0) {
		printf("No support for compressed textures\n");
		exit(1);
	}

	ifs.seekg(pos + texture_info_offs + 1);
	uint8_t nbr_objects = read_little_endian_8(ifs);
	uint16_t size_3d_info_section = read_little_endian_16(ifs);

	// skip to info blocks
	ifs.seekg(static_cast<unsigned>(ifs.tellg()) + 12 + 4 * nbr_objects);

	for (int i = 0; i != nbr_objects; ++i) {
		read_little_endian_16(ifs);

		uint16_t params = read_little_endian_16(ifs);

		unsigned width = 8 << ((params >> 4) & 0x7);
		unsigned height = 8 << ((params >> 7) & 0x7);
		unsigned format = (params >> 10) & 0x7;

		read_little_endian_8(ifs);
		read_little_endian_8(ifs);
		read_little_endian_8(ifs);
		read_little_endian_8(ifs);
		
		vector<unsigned char> img;
		for (unsigned y = 0; y != height; ++y) {
			for (unsigned x = 0; x != width; ++x) {
				unsigned color_index = pos + texture_data_offs + x + y * width;
				unsigned char color;
				unsigned char alpha;

				if (format == 1) {
					color = nsbmd_vector[color_index] & 0x1F;
					alpha = nsbmd_vector[color_index] >> 5;
					alpha = ((alpha * 4) + (alpha / 2)) * 8;

					// shift 3 to make the "fake" grayscale colors stronger
					color = color << 3;
				
					img.push_back(color);
					img.push_back(color);
					img.push_back(color);
					img.push_back(alpha);

				} else if (format == 3) {
					alpha = 255;

					color = nsbmd_vector[color_index] & 0xF;
					// shift 4 to make the "fake" grayscale colors stronger
					color = color << 4;
					img.push_back(color);
					img.push_back(color);
					img.push_back(color);
					img.push_back(alpha);

					color = (nsbmd_vector[color_index] & 0xF0) >> 4;
					// shift 4 to make the "fake" grayscale colors stronger
					color = color << 4;
					img.push_back(color);
					img.push_back(color);
					img.push_back(color);
					img.push_back(alpha);
				}
			}
		}

		if (format == 3) {
			texture_data_offs += width * height / 2;
		} else {
			texture_data_offs += width * height;
		}

		string img_out(out_folder + "/" + SSTR(i) + ".png"); 
		lodepng::encode(img_out, img, width, height);
	}

	ifs.close();
}
