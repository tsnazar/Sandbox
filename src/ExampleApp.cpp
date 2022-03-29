#include "Application.h"
#include "TestLayer.h"
#include "OrthoTest.h"

class ExampleApp : public Application
{
public:
	ExampleApp()
		: Application()
	{
		RegisterLayer<TestLayer>("test");
		RegisterLayer<OrthoTest>("ortho");
	}
};

int main(void)
{
	std::unique_ptr<ExampleApp> app = std::make_unique<ExampleApp>();
	app->Run();
}