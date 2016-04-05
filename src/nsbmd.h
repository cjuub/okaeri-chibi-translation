#ifndef NSBMD_H
#define NSBMD_H

#include <vector>
#include <string>

class NSBMD {
public:
	NSBMD();
	void insert_textures(std::string& nsmbd_file, std::string& img_folder, std::string& out_folder);
	void extract_textures(std::string& nsmbd_file, std::string& out_folder);
private:
    void save(std::string& nsbmd_out, std::vector<char>& nsbmd_vector_mod); 
};

#endif
