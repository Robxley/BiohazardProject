#include "BioApp.h"

#include <iostream>

using namespace bhd;

int main() 
{
	BioApp app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

	return 0;
}