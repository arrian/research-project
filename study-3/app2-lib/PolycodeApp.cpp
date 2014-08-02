#include <iostream>
#include <sstream>
#include <thread>

#include "PolycodeApp.h"

PolycodeApp::PolycodeApp(int width, int height) {

	std::cout << "creating polycode core" << std::endl;
	core = new GLFWCore(width, height);

	std::cout << "adding archive and dir resources" << std::endl;
	CoreServices::getInstance()->getResourceManager()->addArchive("../resources/default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	
	std::cout << "creating polycode scene" << std::endl;
	scene = new Scene(Scene::SCENE_2D);

	// std::cout << CoreServices::getInstance()->getRenderer()->getBackingResolutionScaleX() << std::endl;

	std::cout << "setting camera size" << std::endl;
	scene->getActiveCamera()->setOrthoSize(width, height);

	// std::cout << "creating initial emitter" << std::endl;
	// SceneParticleEmitter *emitter = new SceneParticleEmitter(100, 2.0, 0.5);
	
 //    emitter->setParticleRotationSpeed(Vector3(0.0, 0.0, 100.0));
    
	// emitter->useScaleCurve = true;
	// emitter->scaleCurve.clearControlPoints();
	// emitter->scaleCurve.addControlPoint2d(0, 0.3);
	// emitter->scaleCurve.addControlPoint2d(0.5, 1);
	// emitter->scaleCurve.addControlPoint2d(1, 0);
	
	// emitter->useColorCurves = true;
	// emitter->colorCurveR.clearControlPoints();	
	// emitter->colorCurveG.clearControlPoints();	
	// emitter->colorCurveB.clearControlPoints();	
	// emitter->colorCurveA.clearControlPoints();	
				
	// emitter->colorCurveR.addControlPoint2d(0, 0.3);
	// emitter->colorCurveR.addControlPoint2d(0.1, 1);	
	// emitter->colorCurveR.addControlPoint2d(0.4, 1);		
	// emitter->colorCurveR.addControlPoint2d(0.5, 0);	
	// emitter->colorCurveR.addControlPoint2d(1, 0);
	
	// emitter->colorCurveG.addControlPoint2d(0, 0.3);
	// emitter->colorCurveG.addControlPoint2d(0.1, 0.6);	
	// emitter->colorCurveG.addControlPoint2d(0.4, 0.6);		
	// emitter->colorCurveG.addControlPoint2d(0.5, 0);		
	// emitter->colorCurveG.addControlPoint2d(1, 0.0);
	
	// emitter->colorCurveB.addControlPoint2d(0, 1);
	// emitter->colorCurveB.addControlPoint2d(0.1, 0);	
	// emitter->colorCurveB.addControlPoint2d(1, 0);
	
	// emitter->colorCurveA.addControlPoint2d(0, 0);
	// emitter->colorCurveA.addControlPoint2d(0.05, 1);
	// emitter->colorCurveA.addControlPoint2d(0.6, 1);
	// emitter->colorCurveA.addControlPoint2d(1, 0);
    
 //    emitter->setPosition(0.0, -0.4, 0.0);
    
 //    emitter->setMaterialByName("UnlitAdditive");
    
 //    Texture *tex = CoreServices::getInstance()->getMaterialManager()->createTextureFromFile("../resources/particle.png");
 //    emitter->getLocalShaderOptions()->addTexture("diffuse", tex);

    SceneImage* bg = new SceneImage("../resources/gradient.png");
    scene->addChild(bg);

	// scene->addChild(emitter);
}

PolycodeApp::~PolycodeApp() {

}

bool PolycodeApp::update() {
	// std::cout << "started update and render" << std::endl;
    bool result = core->updateAndRender();
    // std::cout << "finished update and render" << std::endl;
    return result;
}

Core* PolycodeApp::getCore() {
    return core;
}

Scene* PolycodeApp::getScene() {
    return scene;
}






