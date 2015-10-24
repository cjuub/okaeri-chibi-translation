#include "bmg.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Incorrect nbr of args." << endl;
		exit(0);
	}

	BMG bmg;
	string in_file(argv[2]);
	string out_file(argv[3]);

	string option(argv[1]);

	if (option == "-e") {
		bmg.extract_text(in_file, out_file);
	} else if (option == "-i") {
		bmg.insert_text(in_file, out_file);
	}
}