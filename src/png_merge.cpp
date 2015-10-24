#include "lodepng.h"
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    string org(argv[1]);
    string trans(argv[2]);
    string out(argv[3]);

    vector<unsigned char> image_org;
    unsigned w1;
    unsigned h1;
    lodepng::decode(image_org, w1, h1, org);

    vector<unsigned char> image_trans;
    unsigned w2;
    unsigned h2;
    lodepng::decode(image_trans, w2, h2, trans);

    for (unsigned y = 0; y != h1; ++y) {
        for (unsigned x = 0; x != w1; ++x) {
            if (image_trans[4 * w1 * y + 4 * x + 3] != 0) {
                image_org[4 * w1 * y + 4 * x + 0] = image_trans[4 * w1 * y + 4 * x + 0];
                image_org[4 * w1 * y + 4 * x + 1] = image_trans[4 * w1 * y + 4 * x + 1];
                image_org[4 * w1 * y + 4 * x + 2] = image_trans[4 * w1 * y + 4 * x + 2];
                image_org[4 * w1 * y + 4 * x + 3] = image_trans[4 * w1 * y + 4 * x + 3];    
            }
        }
    }

    lodepng::encode(out, image_org, w1, h1);
}