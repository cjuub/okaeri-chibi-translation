#ifndef BMG_H
#define BMG_H

#include <string>

class BMG {
public:
	BMG();
	void extract_text(std::string& in_file, std::string& out_file);
	void insert_text(std::string& in_file, std::string& out_file);
private:
	
};

#endif