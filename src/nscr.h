#ifndef NSCR_H
#define NSCR_H

#include "ncgr.h"

#include <string>
#include <vector>

class NSCR {
public:
	NSCR();
	void insert_image(std::string& nscr_file, NCGR& ncgr, std::string& in_file, std::string& ncgr_out);
	void extract_image(std::string& in_file, NCGR& ncgr, std::string& out_file);
private:
	
};

#endif