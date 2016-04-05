#include "nsbmd.h"
#include "little_endian.h"
#include "lodepng.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


using namespace std; 

NSBMD::NSBMD() {}

void NSBMD::insert_textures(string& nsbmd_file, string& img_folder, string& nsbmd_out) {
	ifstream ifs(nsbmd_file, ios::binary);
	if (!ifs) {
		printf("Could not open file: %s\n", nsbmd_file.c_str());
		exit(1);
	}
	
	vector<char> nsbmd_vector((istreambuf_iterator<char>(ifs)), 
				istreambuf_iterator<char>());

	vector<char> search_string{'T', 'E', 'X', '0'};
	unsigned pos = (search(nsbmd_vector.begin(), nsbmd_vector.end(),
			search_string.begin(), search_string.end())) - nsbmd_vector.begin(); 

	ifs.seekg(0);

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

    vector<char> nsbmd_vector_mod(nsbmd_vector);

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

		ifstream check(img_folder + "/" + SSTR(i) + ".png");
		if (check.good()) {
			lodepng::decode(img, width, height, img_folder + "/" + SSTR(i) + ".png");
            check.close();
        } else {
            check.close();
 
            if (format == 3) {
                texture_data_offs += width * height / 2;
            } else {
                texture_data_offs += width * height;
            }
            
            continue;
        }

        unsigned x2 = 0;
		for (unsigned y = 0; y != height / 2; ++y) {
			for (unsigned x = 0; x != width; ++x) {
				unsigned color_index = (x + x2 + y * width) * 4;
                unsigned texture_data_index = pos + texture_data_offs + x + y * width;
				unsigned char color;
				unsigned char alpha;

                if (texture_data_index == 58528) {
                    printf("hej\n");
                }

				if (format == 1) {
					color = img[color_index] >> 3;
					alpha = img[color_index + 1];

                    nsbmd_vector_mod[texture_data_index] = alpha + color;
				} else if (format == 3) {
					alpha = 255;

					color = img[color_index] >> 4;
                    nsbmd_vector_mod[texture_data_index] = color;
					color = img[color_index + 4];
                    nsbmd_vector_mod[texture_data_index] += color;

                    ++x2;
				}
			}
		}

		if (format == 3) {
			texture_data_offs += width * height / 2;
		} else {
			texture_data_offs += width * height;
		}
    }

    save(nsbmd_out, nsbmd_vector_mod);
}

void NSBMD::extract_textures(string& nsbmd_file, string& out_folder) {
	ifstream ifs(nsbmd_file, ios::binary);
	if (!ifs) {
		printf("Could not open file: %s\n", nsbmd_file.c_str());
		exit(1);
	}
	
	vector<char> nsbmd_vector((istreambuf_iterator<char>(ifs)), 
				istreambuf_iterator<char>());

	vector<char> search_string{'T', 'E', 'X', '0'};
	unsigned pos = (search(nsbmd_vector.begin(), nsbmd_vector.end(),
			search_string.begin(), search_string.end())) - nsbmd_vector.begin(); 

	ifs.seekg(0);

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
				unsigned alpha;

				if (format == 1) {
					color = nsbmd_vector[color_index] & 0x1F;
					alpha = nsbmd_vector[color_index] & 0xE0;

					// shift 3 to make the "fake" grayscale colors stronger
					color = color << 3;
				
					img.push_back(color);
					img.push_back(alpha); // hacky, store alpha as green make editing easier
					img.push_back(color);
					img.push_back(0xFF);

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

void NSBMD::save(string& nsbmd_out, vector<char>& nsbmd_vector_mod) {
    ofstream ofs(nsbmd_out, ios::binary);

    ostream_iterator<char> output_iterator(ofs);
    copy(nsbmd_vector_mod.begin(), nsbmd_vector_mod.end(), output_iterator);

    ofs.close();
}
