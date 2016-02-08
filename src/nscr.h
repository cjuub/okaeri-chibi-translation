#ifndef NSCR_H
#define NSCR_H

#include "ncgr.h"

#include <string>
#include <vector>

typedef struct meta_data meta_data; 

class NSCR {
public:
	NSCR();
	void insert_image(std::string& nscr_file, NCGR& ncgr, std::string& in_file, std::string& ncgr_out, std::string& nscr_out);
	void extract_image(std::string& in_file, NCGR& ncgr, std::string& out_file);
	void save(const std::string& nscr_in, const std::string& nscr_out, std::vector<uint16_t>& data_mod);
private:
};

#endif