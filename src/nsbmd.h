#ifndef NSBMD_H
#define NSBMD_H

#include <string>

class NSBMD {
public:
	NSBMD();
	void extract_textures(std::string& nsmbd_file, std::string& out_folder);
private:
};

#endif
