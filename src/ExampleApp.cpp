#include "Application.h"
#include "CornellBox/CornellBox.h"
#include "PointShadows/TestPointShadows.h"
#include "ShadowMap/TestShadowMap.h"
#include "Ortho/OrthoTest.h"
#include "TestLightA/TestLightA.h"
#include "TestLightB/TestLightB.h"
#include "TestLightC/TestLightC.h"
#include "TestLightD/TestLightD.h"


class ExampleApp : public Application
{
public:
	ExampleApp()
		: Application()
	{
		RegisterLayer<TestShadowMap>("TestShadowMap");
		RegisterLayer<TestPointShadows>("TestPointShadows");
		RegisterLayer<CornellBox>("CornellBox");
		RegisterLayer<OrthoTest>("OrthoTest");
		RegisterLayer<TestLightA>("TestLightA");
		RegisterLayer<TestLightB>("TestLightB");
		RegisterLayer<TestLightC>("TestLightC");
		RegisterLayer<TestLightD>("TestLightD");
	}
};

int main(void)
{
	std::unique_ptr<ExampleApp> app = std::make_unique<ExampleApp>();
	app->GetWindow().SetVsync(false);
	app->Run();
}