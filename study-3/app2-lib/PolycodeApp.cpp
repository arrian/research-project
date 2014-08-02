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

//////////////////////////////////////////////////////

poly_core* poly_core_create(int width, int height)
{
	GLFWCore* core = new GLFWCore(width, height);
	CoreServices::getInstance()->getResourceManager()->addArchive("../resources/default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	return reinterpret_cast<poly_core*>(core);
}

void poly_core_update(poly_core* core)
{
	reinterpret_cast<Core*>(core)->updateAndRender();
}

void poly_core_destroy(poly_core* core)
{
	delete reinterpret_cast<Core*>(core);
}

poly_scene* poly_scene_create()
{
	Scene* scene = new Scene(Scene::SCENE_2D);
	//scene->getActiveCamera()->setOrthoSize(width, height);
	return reinterpret_cast<poly_scene*>(scene);
}

void poly_scene_destroy(poly_scene* scene)
{
	delete reinterpret_cast<Scene*>(scene);
}

void poly_scene_add_child(poly_scene* scene, poly_entity* entity)
{
	reinterpret_cast<Scene*>(scene)->addChild(reinterpret_cast<Entity*>(entity));
}

poly_scene_image* poly_scene_image_create(char* path)
{
	return reinterpret_cast<poly_scene_image*>(new SceneImage(path));
}

void poly_scene_image_destroy(poly_scene_image* image)
{
	delete reinterpret_cast<SceneImage*>(image);
}





