#ifndef GRAPHIC_META_H
#define GRAPHIC_META_H

#include <string>
#include <vector>
#include <unordered_map>

#define OAM_LIMIT 15

class GraphicMeta {
public:
	GraphicMeta(const std::string& path, int nbr_oams_org_in);
	bool has_custom_oam();
	bool has_new_oam();
	uint16_t get_nbr_new_oam();
	bool exists();
	bool has_oam(int oam);
	bool is_new_oam(int oam);
	int get_new_oam_x(int oam);
	int get_new_oam_y(int oam);
	int get_oam_x_shift(int oam);
	int get_new_tile(int oam);

	int get_x_min();
	int get_y_min();
	int get_x_max();
	int get_y_max();

	// bool is_shift_only(int oam);
	int get_image(int oam);
private:
	bool meta_exists = false;

	int nbr_oams_org;

	int x_min = 0;
	int y_min = 0;
	int x_max = 0;
	int y_max = 0;

	std::unordered_map<int, int> custom_oam_tile_map;
	int oam_x_shift[OAM_LIMIT] = {0};
	int oam_y_shift = 0;
	int nbr_new_oam = 0;
	std::unordered_map<int, int> custom_x_map;
	std::unordered_map<int, int> custom_y_map;

	bool is_custom_oam = false;
	
	std::vector<int> oams_change;
	std::vector<int> oams_shift_only;

	std::unordered_map<int, int> oam_image_map;
};

#endif