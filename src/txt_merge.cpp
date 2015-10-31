#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    string org(argv[1]);
    string trans(argv[2]);
    string out(argv[3]);

    ifstream txt_org(org);
    ifstream txt_trans(trans);

    ofstream txt_out(out);

    string line_org;
    string line_trans;
    while (getline(txt_org, line_org)) {
    	getline(txt_trans, line_trans);
    	if (line_org != "" && line_trans == "") {
    		txt_out << line_org << "\n";
    	} else {
    		txt_out << line_trans << "\n";
    	}
    }

    txt_org.close();
    txt_trans.close();
    txt_out.close();
}