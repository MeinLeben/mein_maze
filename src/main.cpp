#include "pch.h"

#include "application.h"

int32_t main(int32_t argc, char* argv[]) {
	std::unique_ptr<Application> application = std::make_unique<Application>(argc, argv);
	return application->Run();
}
