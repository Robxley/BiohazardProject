


#include "Logger.hpp"

#include "BioApp.hpp"
#include <iostream>

using namespace bhd;

class LOGGER_INITER
{
	public:
	LOGGER_INITER(){ BHD_LOG_INSTANCE(nullptr); }
};

static LOGGER_INITER loginiter;

int main() 
{
	try {
		VulkanInstance::info();
		BioApp app;
		app.run();
	}
	catch (const std::runtime_error& e) {
		BHD_LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;

}