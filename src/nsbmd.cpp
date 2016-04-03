#include "nsbmd.h"

#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std; 

NSBMD::NSBMD() {}

void NSBMD::extract_textures(string& nsbmd_file, string& out_folder) {
	ifstream ifs(nsbmd_file, ios::binary);
	vector<char> nsbmd_vector((istreambuf_iterator<char>(ifs)), 
				istreambuf_iterator<char>());

	vector<char> search_string{'T', 'E', 'X', '0'};
	unsigned pos = (search(nsbmd_vector.begin(), nsbmd_vector.end(),
			search_string.begin(), search_string.end())) - nsbmd_vector.begin(); 

	printf("%x", pos);
}
