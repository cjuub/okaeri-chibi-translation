#include "graphic_meta.h"

#include <fstream>
#include <sstream>

using namespace std;

GraphicMeta::GraphicMeta(const string& path, int nbr_oams_org_in) {
	ifstream meta(path);

	nbr_oams_org = nbr_oams_org_in;

	if (meta.good()) {
		meta_exists = true;
	} else {
		meta.close();
	}

	if (meta_exists) {
		string line;
		getline(meta, line);
		istringstream oam_info(line);

		oam_info >> x_min;
		oam_info >> y_min;
		oam_info >> x_max;
		oam_info >> y_max;

		int img_nbr = 0;
		int oam;
		int new_oam = -1;
		while (oam_info >> oam) {
			oams_change.push_back(oam);
			oam_image_map.emplace(oam, img_nbr);

			if (is_new_oam(oam)) {
				new_oam = oam;
			}
		}

		if (getline(meta, line)) {
			istringstream extend_info(line);

			is_custom_oam = true;

			int x_shift;
			extend_info >> x_shift;
			extend_info >> oam_y_shift;

			for (auto it = oams_change.begin(); it != oams_change.end(); ++it) {
				oam_x_shift[*it] = x_shift;
			}
		}

		if (getline(meta, line) && line != "") {
			istringstream extend_info(line);

			int tile;
			int custom_x;
			int custom_y;
			extend_info >> tile;

			custom_oam_tile_map.emplace(new_oam, tile);
			extend_info >> nbr_new_oam;

			// ugly hack to specify existing tile instead of new
			if (nbr_new_oam == 0) {
				is_existing_tile = true;
				nbr_new_oam++;
			}

			extend_info >> custom_x;
			custom_x_map.emplace(new_oam, custom_x);
			extend_info >> custom_y;
			custom_y_map.emplace(new_oam, custom_y);
		}

		while (getline(meta, line)) {
			istringstream extend_info(line);

			++img_nbr;

			vector<int> oams_to_shift;

			while (extend_info >> oam) {
				oams_change.push_back(oam);
				oams_to_shift.push_back(oam);
				oam_image_map.emplace(oam, img_nbr);

				if (is_new_oam(oam)) {
					new_oam = oam;
				}
			}

			getline(meta, line);
			istringstream extend_info2(line);

			int x_shift;
			extend_info2 >> x_shift;
			extend_info2 >> oam_y_shift;

			for (unsigned i = 0; i != oams_to_shift.size(); ++i) {
				oam_x_shift[oams_to_shift[i]] = x_shift;
			}

			if (getline(meta, line)) {
				istringstream extend_info2(line);

				int tile;
				int nbr_new;
				int custom_x;
				int custom_y;

				extend_info2 >> tile;
				extend_info2 >> nbr_new;

				custom_oam_tile_map.emplace(new_oam, tile);
				nbr_new_oam += nbr_new;
				extend_info2 >> custom_x;
				custom_x_map.emplace(new_oam, custom_x);
				extend_info2 >> custom_y;
				custom_y_map.emplace(new_oam, custom_y);
			}
		}
	}

	meta.close();
}

bool GraphicMeta::has_custom_oam() {
	return is_custom_oam;
}

bool GraphicMeta::has_new_oam() {
	return nbr_new_oam != 0;
}

uint16_t GraphicMeta::get_nbr_new_oam() {
	return nbr_new_oam;
}

bool GraphicMeta::exists() {
	return meta_exists;
}

bool GraphicMeta::has_oam(int oam) {
	for (auto it = oams_change.begin(); it != oams_change.end(); ++it) {
		if (*it == oam) {
			return true;
		}
	}

	return false;
}

bool GraphicMeta::is_new_oam(int oam) {
	for (auto it = oams_change.begin(); it != oams_change.end(); ++it) {
		if (*it == oam && *it >= nbr_oams_org) {
			return true;
		}
	}

	return false;
}

int GraphicMeta::get_new_oam_x(int oam) {
	return custom_x_map[oam];
}

int GraphicMeta::get_new_oam_y(int oam) {
	return custom_y_map[oam];
}

int GraphicMeta::get_oam_x_shift(int oam) {
	return oam_x_shift[oam];
}

int GraphicMeta::get_new_tile(int oam) {
	return custom_oam_tile_map[oam];
}

int GraphicMeta::get_x_min() {
	return x_min;
}

int GraphicMeta::get_y_min() {
	return y_min;
}

int GraphicMeta::get_x_max() {
	return x_max;
}

int GraphicMeta::get_y_max() {
	return y_max;
}

int GraphicMeta::get_image(int oam) {
	return oam_image_map[oam];
}

bool GraphicMeta::has_existing_tile() {
	return is_existing_tile;
}