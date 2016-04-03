#include "nsbmd.h"
#include "little_endian.h"
#include "lodepng.h"

#include <fstream>
#include <vector>
#include <algorithm>

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

	ifs.seekg(pos + texture_info_offs + size_3d_info_section);
	read_little_endian_16(ifs);
	uint16_t params = read_little_endian_16(ifs);
	
	// is this really correct?
	unsigned height = read_little_endian_8(ifs) << 3;
	read_little_endian_8(ifs);
	unsigned width = read_little_endian_8(ifs) << 3;

	// printf("%d %d\n", width, height);

	vector<unsigned char> img;
	for (unsigned y = 0; y != height; ++y) {
		for (unsigned x = 0; x != width; ++x) {
			unsigned char color = nsbmd_vector[x + y * width] & 0x1F;
			unsigned char alpha = nsbmd_vector[x + y * width] >> 5;

			color = (color + 1) * 3;
			alpha = ((alpha * 4) + (alpha / 2)) * 8;
			
			img.push_back(color);
			img.push_back(color);
			img.push_back(color);
			img.push_back(alpha);
		}
	}

	lodepng::encode(out_folder, img, width, height);

	ifs.close();
}
