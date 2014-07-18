#include "PolycodeApp.h"

#include <iostream>
#include <sstream>

PolycodeApp::PolycodeApp(PolycodeView *view) {

	core = new POLYCODE_CORE(view, 1024, 768, false, true, 0, 0, 90, 0, true);

	CoreServices::getInstance()->getResourceManager()->addArchive("Resources/default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	
	scene = new Scene(Scene::SCENE_2D);

	std::cout << CoreServices::getInstance()->getRenderer()->getBackingResolutionScaleX() << std::endl;

	scene->getActiveCamera()->setOrthoSize(1024, 768);

	SceneParticleEmitter *emitter = new SceneParticleEmitter(100, 2.0, 0.5);
	
    emitter->setParticleRotationSpeed(Vector3(0.0, 0.0, 100.0));
    
	emitter->useScaleCurve = true;
	emitter->scaleCurve.clearControlPoints();
	emitter->scaleCurve.addControlPoint2d(0, 0.3);
	emitter->scaleCurve.addControlPoint2d(0.5, 1);
	emitter->scaleCurve.addControlPoint2d(1, 0);
	
	emitter->useColorCurves = true;
	emitter->colorCurveR.clearControlPoints();	
	emitter->colorCurveG.clearControlPoints();	
	emitter->colorCurveB.clearControlPoints();	
	emitter->colorCurveA.clearControlPoints();	
				
	emitter->colorCurveR.addControlPoint2d(0, 0.3);
	emitter->colorCurveR.addControlPoint2d(0.1, 1);	
	emitter->colorCurveR.addControlPoint2d(0.4, 1);		
	emitter->colorCurveR.addControlPoint2d(0.5, 0);	
	emitter->colorCurveR.addControlPoint2d(1, 0);
	
	emitter->colorCurveG.addControlPoint2d(0, 0.3);
	emitter->colorCurveG.addControlPoint2d(0.1, 0.6);	
	emitter->colorCurveG.addControlPoint2d(0.4, 0.6);		
	emitter->colorCurveG.addControlPoint2d(0.5, 0);		
	emitter->colorCurveG.addControlPoint2d(1, 0.0);
	
	emitter->colorCurveB.addControlPoint2d(0, 1);
	emitter->colorCurveB.addControlPoint2d(0.1, 0);	
	emitter->colorCurveB.addControlPoint2d(1, 0);
	
	emitter->colorCurveA.addControlPoint2d(0, 0);
	emitter->colorCurveA.addControlPoint2d(0.05, 1);
	emitter->colorCurveA.addControlPoint2d(0.6, 1);
	emitter->colorCurveA.addControlPoint2d(1, 0);
    
    emitter->setPosition(0.0, -0.4, 0.0);
    
    emitter->setMaterialByName("UnlitAdditive");
    
    Texture *tex = CoreServices::getInstance()->getMaterialManager()->createTextureFromFile("Resources/particle.png");
    emitter->getLocalShaderOptions()->addTexture("diffuse", tex);


    SceneImage* bg = new SceneImage("research-project/Resources/gradient.png");
    scene->addChild(bg);

	scene->addChild(emitter);

	circles = new SceneEntity();
	lines = new SceneEntity();
	text = new SceneEntity();

	scene->addChild(lines);
	scene->addChild(circles);
	scene->addChild(text);
}

PolycodeApp::~PolycodeApp() {

	delete lines;
	delete circles;
}

bool PolycodeApp::Update() {
    return core->updateAndRender();
}

Core* PolycodeApp::getCore() {
    return core;
}

Scene* PolycodeApp::getScene() {
    return scene;
}

void PolycodeApp::clear()
{
	// std::cout << "size: " << clearables.size() << std::endl;

	for(auto clearable : clearableLabels)
	{
		text->removeChild(clearable);
		delete clearable;
	}
	clearableLabels.erase(clearableLabels.begin(), clearableLabels.end());	

	for(auto clearable : clearableLines)
	{
		lines->removeChild(clearable);
		delete clearable;
	}
	clearableLines.erase(clearableLines.begin(), clearableLines.end());	


	for(auto clearable : clearables)
	{
		circles->removeChild(clearable);
		delete clearable;
	}
	clearables.erase(clearables.begin(), clearables.end());	
	


}

polycode_scenemesh* polycode_add_text(polycode* pc, float x, float y, char* text, int size)
{
	if(!pc) 
	{
		std::cerr << "polycode was null in polycode_add_text" << std::endl;
		return nullptr;
	}

	PolycodeApp* app = reinterpret_cast<PolycodeApp*>(pc);
	SceneLabel* label = new SceneLabel(std::string(text), size);
	label->setColor(1, 1, 0.6, 1);
	label->setPosition(x, y);
	app->text->addChild(label);
	app->clearableLabels.push_back(label);
	return reinterpret_cast<polycode_scenemesh*>(label);
}

float polycode_add_code(polycode* pc, float x, float y, char* text, int size)
{
	if(!pc) 
	{
		std::cerr << "polycode was null in polycode_add_text" << std::endl;
		return y;
	}

	char delims = '\n';
	std::stringstream ss(text);
	std::string to;

	float next = y;

	PolycodeApp* app = reinterpret_cast<PolycodeApp*>(pc);

	if (text != NULL)
	{
		while(std::getline(ss, to, '\n')){
			SceneLabel* label = new SceneLabel(to, size);
			label->setColor(1, 1, 0.6, 1);
			label->setPosition(x, next);
			app->text->addChild(label);
			app->clearableLabels.push_back(label);
			next += 10.0;
		}
	}


	return next;
}

polycode_scenemesh* polycode_add_image(polycode* pc, char* path)
{
	return nullptr;
}

polycode_scenemesh* polycode_add_circle(polycode* pc, float x, float y, float w, float h, int segments, float r, float g, float b)
{
	if(!pc) 
	{
		std::cerr << "polycode was null in polycode_add_text" << std::endl;
		return nullptr;
	}

	PolycodeApp* app = reinterpret_cast<PolycodeApp*>(pc);
	ScenePrimitive* primitive = new ScenePrimitive(ScenePrimitive::TYPE_CIRCLE, w, h, segments);
	primitive->setPosition(x, y);
	primitive->setColor(r, g, b, 1);
	//primitive->colorAffectsChildren = false;
	app->circles->addChild(primitive);
	app->clearables.push_back(primitive);
	return reinterpret_cast<polycode_scenemesh*>(primitive);
}

polycode_scenemesh* polycode_add_line(polycode* pc, polycode_scenemesh* source, polycode_scenemesh* target, float width)
{
	if(!pc) 
	{
		std::cerr << "polycode was null in polycode_add_text" << std::endl;
		return nullptr;
	}

	PolycodeApp* app = reinterpret_cast<PolycodeApp*>(pc);
	
	SceneLine* line = new SceneLine(reinterpret_cast<Entity*>(source), reinterpret_cast<Entity*>(target));
	line->setLineWidth(width);
	app->clearableLines.push_back(line);
	app->lines->addChild(line);
	return reinterpret_cast<polycode_scenemesh*>(line);
}


void polycode_set_position(polycode_scenemesh* mesh, float x, float y)
{

}

void polycode_set_color(polycode_scenemesh* mesh, float r, float g, float b)
{
	reinterpret_cast<SceneMesh*>(mesh)->setColor(r, g, b, 1);
}

void polycode_set_roll(polycode_scenemesh* mesh, int degree)
{

}

void polycode_set_scale(polycode_scenemesh* mesh, float sx, float sy)
{

}

void polycode_clear_scene(polycode* pc)
{
	reinterpret_cast<PolycodeApp*>(pc)->clear();
}









