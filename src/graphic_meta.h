#ifndef GRAPHIC_META_H
#define GRAPHIC_META_H

#include <string>
#include <vector>

class GraphicMeta {
public:
	GraphicMeta(const std::string& path);
	bool has_custom_oam();
	bool has_new_oam();
	uint16_t get_nbr_new_oam();
	bool exists();
	bool has_oam(int oam);
	bool is_new_oam(int oam, int nbr_oams_org);
	int get_new_oam_x();
	int get_new_oam_y();
	int get_oam_x_shift();
	int get_new_tile();

	int get_x_min();
	int get_y_min();
	int get_x_max();
	int get_y_max();
private:
	bool meta_exists = false;

	int x_min = 0;
	int y_min = 0;
	int x_max = 0;
	int y_max = 0;

	int custom_oam_tile = 0;
	int oam_x_shift = 0;
	int oam_y_shift = 0;
	int nbr_new_oam = 0;
	int custom_x = 0;
	int custom_y = 0;

	bool is_custom_oam = false;
	
	std::vector<int> oams_change;
};

#endif