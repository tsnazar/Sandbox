#include "Application.h"
#include "TestLayer.h"

class ExampleApp : public Application
{
public:
	ExampleApp()
		: Application()
	{
		RegisterLayer<TestLayer>("test");
	}
};

int main(void)
{
	std::unique_ptr<ExampleApp> app = std::make_unique<ExampleApp>();
	app->Run();
}