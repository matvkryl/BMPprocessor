#include <iostream>

#include "BMPreader.h"

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "Error: File_name required\n";
		return 1;
	}
	try {
		BMPreader picture(argv[1]);
		picture.displayBMP();
	}
	catch (std::runtime_error& e) {
		std::cout << "Runtime_error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
