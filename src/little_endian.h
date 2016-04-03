#ifndef LITTLE_ENDIAN_H
#define LITTLE_ENDIAN_H

#include <fstream>
#include <cstdint>

inline uint32_t read_little_endian_32(std::ifstream& ifs) {
	char b;
	ifs.get(b);
	uint32_t val = static_cast<uint8_t>(b);
	ifs.get(b);
	val += static_cast<uint8_t>(b) << 8;
	ifs.get(b);
	val += static_cast<uint8_t>(b) << 16;
	ifs.get(b);
	val += static_cast<uint8_t>(b) << 24;

	return val;
}

inline uint16_t read_little_endian_16(std::ifstream& ifs) {
	char b;
	ifs.get(b);
	uint32_t val = static_cast<uint8_t>(b);
	ifs.get(b);
	val += static_cast<uint8_t>(b) << 8;

	return val;
}

inline uint8_t read_little_endian_8(std::ifstream& ifs) {
	char b;
	ifs.get(b);
	return static_cast<uint8_t>(b);
}

inline void write_little_endian_32(std::ofstream& ofs, uint32_t val) {
	ofs << static_cast<char>(val & 0xFF);
	ofs << static_cast<char>((val >> 8) & 0xFF);
	ofs << static_cast<char>((val >> 16) & 0xFF);
	ofs << static_cast<char>((val >> 24) & 0xFF);
}

inline void write_little_endian_16(std::ofstream& ofs, uint16_t val) {
	ofs << static_cast<char>(val & 0xFF);
	ofs << static_cast<char>((val >> 8) & 0xFF);
}

inline void write_little_endian_8(std::ofstream& ofs, uint8_t val) {
	ofs << static_cast<char>(val);
}

inline void copy_until(std::ifstream& ifs, std::ofstream& ofs, int pos) {
	while (ifs.tellg() != pos) {
		ofs << static_cast<uint8_t>(ifs.get());
	}
}

#endif
