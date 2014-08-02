#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Polycode.h>
#include "PolyGLFWCore.h"

using namespace Polycode;

class PolycodeApp {
public:
    PolycodeApp(int width, int height);
    ~PolycodeApp();

    bool update();

    Core* getCore();
    Scene* getScene();

private:
	Core* core;
	Scene* scene;
};


int main(int argc, char* argv[])
{
	PolycodeApp* app = new PolycodeApp(640, 480);
	
	while(true)
	{
		app->update();
	}

	return 0;
}



