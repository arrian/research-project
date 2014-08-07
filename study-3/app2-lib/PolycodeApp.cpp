#include <iostream>
#include <sstream>
#include <thread>

#include "PolycodeApp.h"

poly_core* poly_core_create(char* title, int width, int height, char* resources)
{
	GLFWCore* core = new GLFWCore(title, width, height);
	CoreServices::getInstance()->getResourceManager()->addArchive(resources);//"../resources/default.pak"
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

poly_scene* poly_scene_create(int width, int height)
{
	Scene* scene = new Scene(Scene::SCENE_2D);
	scene->getActiveCamera()->setOrthoSize(width, height);
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

void poly_entity_set_position(poly_entity* entity, double x, double y)
{
	reinterpret_cast<Entity*>(entity)->setPosition(x, y);
}

void poly_entity_set_yaw(poly_entity* entity, double yaw)
{
	reinterpret_cast<Entity*>(entity)->setYaw(yaw);
}

void poly_entity_add_child(poly_entity* target, poly_entity* entity)
{
	reinterpret_cast<Entity*>(target)->addChild(reinterpret_cast<Entity*>(entity));
}

void poly_entity_set_color(poly_entity* entity, double r, double g, double b, double a)
{
	reinterpret_cast<Entity*>(entity)->setColor(r, g, b, a);
}

poly_scene_image* poly_scene_image_create(char* path)
{
	return reinterpret_cast<poly_scene_image*>(new SceneImage(path));
}

void poly_scene_image_destroy(poly_scene_image* image)
{
	delete reinterpret_cast<SceneImage*>(image);
}

poly_scene_multi_label* poly_scene_multi_label_create(char* text, int size)
{
	SceneMultiLabel* input = new SceneMultiLabel(text, size, -(size * 2));
	return reinterpret_cast<poly_scene_multi_label*>(input);
}

void poly_scene_multi_label_destroy(poly_scene_multi_label* input)
{
	delete reinterpret_cast<SceneMultiLabel*>(input);
}

void poly_scene_multi_label_set_text(poly_scene_multi_label* input, char* text)
{
	reinterpret_cast<SceneMultiLabel*>(input)->setText(text);
}

poly_scene_label* poly_scene_label_create(char* text, int size)
{
	return reinterpret_cast<poly_scene_label*>(new SceneLabel(text, size));
}

void poly_scene_label_destroy(poly_scene_label* label)
{
	delete reinterpret_cast<SceneLabel*>(label);
}

void poly_scene_label_set_text(poly_scene_label* label, char* text)
{
	reinterpret_cast<SceneLabel*>(label)->setText(text);
}

poly_scene_primitive* poly_scene_primitive_create_cube(double xSize, double ySize, double zSize)
{
	return reinterpret_cast<poly_scene_primitive*>(new ScenePrimitive(0, xSize, ySize, zSize));
}

poly_scene_primitive* poly_scene_primitive_create_circle(double xSize, double ySize, int segments)
{
	return reinterpret_cast<poly_scene_primitive*>(new ScenePrimitive(8, xSize, ySize, segments));
}

void poly_scene_primitive_destroy(poly_scene_primitive* primitive)
{
	delete reinterpret_cast<ScenePrimitive*>(primitive);
}

void poly_scene_primitive_set_circle_options(poly_scene_primitive* primitive, double xSize, double ySize, int segments)
{
	reinterpret_cast<ScenePrimitive*>(primitive)->setPrimitiveOptions(8, xSize, ySize, segments);
}

