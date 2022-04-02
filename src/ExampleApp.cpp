#include "Application.h"
#include "TestLayer.h"
#include "OrthoTest.h"
#include "TestCube.h"
#include "TestLightA.h"
#include "TestLightB.h"
#include "TestLightC.h"
#include "TestLightD.h"

class ExampleApp : public Application
{
public:
	ExampleApp()
		: Application()
	{
		//RegisterLayer<TestLayer>("test");
		RegisterLayer<OrthoTest>("ortho");
		RegisterLayer<TestCube>("cube");
		RegisterLayer<TestLightA>("LightA");
		RegisterLayer<TestLightB>("LightB");
		RegisterLayer<TestLightC>("LightC");
		RegisterLayer<TestLightD>("LightD");
	}
};

int main(void)
{
	std::unique_ptr<ExampleApp> app = std::make_unique<ExampleApp>();
	app->Run();
}