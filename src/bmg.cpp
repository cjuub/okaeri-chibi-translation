#include "bmg.h"
#include "little_endian.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstdint>

using namespace std;

BMG::BMG() {}

void BMG::extract_text(string& in_file, string& out_file) {
	ifstream ifs(in_file);
	if (!ifs) {
		cout << "Could not open BMG file." << endl;
	}

	ifs.seekg(8);
	read_little_endian_32(ifs);
	uint32_t nbr_sections = read_little_endian_32(ifs);

	if (nbr_sections != 2) {
		cout << "Not two sections, exiting..." << endl;
		exit(0);
	}

	// Section 1: INF1
	ifs.seekg(0x24);
	uint32_t section_size = read_little_endian_32(ifs);
	uint16_t section_nbr_msgs = read_little_endian_16(ifs);
	ifs.seekg(0x30);

	for (unsigned int i = 0; i != section_nbr_msgs; ++i) {
		read_little_endian_32(ifs);
	}

	// Section 2: DAT1
	int offset = section_size;
	ifs.seekg(offset + 0x24);
	read_little_endian_32(ifs);

	ofstream ofs(out_file);

	int nbr = 0;
	offset = 0;

	for (uint16_t i = 0, j = 0; j != static_cast<uint16_t>(section_nbr_msgs + 1); ++i) {
		char b1 = ifs.get();
		if (b1 == '\0') {
			++j;
			ofs << "\n";
			offset = i + 1;
			nbr++;
		} else if (b1 == 0x0A) {
			ofs << "<N>";
		} else if (b1 == 0x1A) {
			uint8_t b1 = ifs.get();
			uint8_t id = ifs.get();
			i += 2;
			int size = 0;

			ofs << "<" << hex << setfill('0') << setw(2);
			ofs << hex << setfill('0') << setw(2) << 0x1A;
			ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(b1);
			ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(id);
			switch (b1) {
			case 0x06: 
				if (id == 0xFF) {
					size = 3;
				} else {
					size = 3;
				}

				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				break;
			case 0x07: size = 4; 
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				break;
			case 0x0A: size = 7; 
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				ofs << hex << setfill('0') << setw(2) << static_cast<unsigned>(ifs.get());
				break;
			}

			ofs << ">";
			i += size;
		} else {
			ofs << b1;
		}
	}

	ofs.close();
	ifs.close();
}

void BMG::insert_text(string& in_file, string& out_file) {
	ifstream ifs(in_file);

	vector<uint32_t> pointers;
	vector<char> bytes;

	char b;
	int nbr = 0;

	for (int i = 0; (b = ifs.get()) != EOF; ++i) {
		int extra = -1;
		switch (b) {
		case '<':
			while ((b = ifs.get()) != '>') {
				switch (b) {
				case 'N':
					bytes.push_back('\n');
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
					string val = "";
					val.push_back(b);
					val.push_back(ifs.get());
					++extra;
					bytes.push_back(stoul(val, nullptr, 16));
					break;
				}
			}

			if (extra > 0) {
				i += extra;
			}

			break;
		case '\n':
			pointers.push_back(i + 1);
			++nbr;
			bytes.push_back('\0');
			break;
		default:
			bytes.push_back(b);
		}
	}

	int header_size = 0x30;
	int ptr_size = 4 * pointers.size();
	int dat1_header_size = 8;
	int strings_size = bytes.size();
	int ptr_align = 0;
	for (int i = 0; (ptr_size + i) % 16 != 0; ++i) {
		++ptr_align;
	}

	int text_align = 0;
	for (int i = 0; (dat1_header_size + strings_size + i) % 16 != 0; ++i) {
		++text_align;
	}

	ofstream ofs(out_file);

	ofs << "MESGbmg1";
	int size = header_size + ptr_size + dat1_header_size + strings_size + ptr_align + text_align;

	write_little_endian_32(ofs, size);
	write_little_endian_32(ofs, 2); // nbr of sections
	write_little_endian_32(ofs, 3); // unknown
	write_little_endian_32(ofs, 0); // filler
	write_little_endian_32(ofs, 0);	// filler
	write_little_endian_32(ofs, 0);	// filler

	ofs << "INF1";
	write_little_endian_32(ofs, 0x10 + ptr_size + ptr_align);
	write_little_endian_16(ofs, nbr);
	write_little_endian_16(ofs, 4);
	write_little_endian_32(ofs, 3); // unknown

	for (auto it = pointers.begin(); it != pointers.end(); ++it) {
		write_little_endian_32(ofs, *it);
	}

	for (int i = 0; (ptr_size + i) % 16 != 0; ++i) {
		ofs << static_cast<char>(0);
	}

	ofs << "DAT1";
	write_little_endian_32(ofs, dat1_header_size + strings_size + text_align);

	for (auto it = bytes.begin(); it != bytes.end(); ++it) {
		ofs << *it;
	}

	for (int i = 0; (dat1_header_size + strings_size + i) % 16 != 0; ++i) {
		ofs << static_cast<char>(0);
	}

	ofs.close();
	ifs.close();
}
