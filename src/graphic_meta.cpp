#include "graphic_meta.h"

#include <fstream>
#include <sstream>

using namespace std;

GraphicMeta::GraphicMeta(const string& path) {
	ifstream meta(path);

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
		while (oam_info >> oam) {
			oams_change.push_back(oam);
			oam_image_map.emplace(oam, img_nbr);
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

		if (getline(meta, line)) {
			istringstream extend_info(line);

			extend_info >> custom_oam_tile;
			extend_info >> nbr_new_oam;
			extend_info >> custom_x;
			extend_info >> custom_y;
		}

		while (getline(meta, line)) {
			istringstream extend_info(line);

			++img_nbr;

			while (extend_info >> oam) {
				oams_change.push_back(oam);
				oams_shift_only.push_back(oam);
				oam_image_map.emplace(oam, img_nbr);
			}

			getline(meta, line);
			istringstream extend_info2(line);

			int x_shift;
			extend_info2 >> x_shift;
			extend_info2 >> oam_y_shift;

			for (unsigned i = 0; i != oams_shift_only.size(); ++i) {
				oam_x_shift[oams_shift_only[i]] = x_shift;
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

bool GraphicMeta::is_new_oam(int oam, int nbr_oams_org) {
	for (auto it = oams_change.begin(); it != oams_change.end(); ++it) {
		if (*it == oam && *it >= nbr_oams_org) {
			return true;
		}
	}

	return false;
}

int GraphicMeta::get_new_oam_x() {
	return custom_x;
}

int GraphicMeta::get_new_oam_y() {
	return custom_y;
}

int GraphicMeta::get_oam_x_shift(int oam) {
	return oam_x_shift[oam];
}

int GraphicMeta::get_new_tile() {
	return custom_oam_tile;
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

bool GraphicMeta::is_shift_only(int oam) {
	for (auto it = oams_shift_only.begin(); it != oams_shift_only.end(); ++it) {
		if (*it == oam) {
			return true;
		}
	}

	return false;
}

int GraphicMeta::get_image(int oam) {
	return oam_image_map[oam];
}
