#include "Core/AppEngine.h"

#include "Engine/Core/EngineLocator.h"

int main()
{
	using namespace MyEngine;

	std::shared_ptr<Engine> pApp = std::shared_ptr<Engine>(new Application());
	EngineLocator::Set(pApp);

	pApp->Run(true);

	pApp->Shutdown();

	return 0;
}