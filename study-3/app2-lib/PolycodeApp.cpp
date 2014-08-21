#include <iostream>
#include <sstream>
#include <thread>
#include <sys/time.h>

#include "PolycodeApp.h"
#include "Polycode2DPhysics.h"
#include "PolyPhysicsScreenEntity.h"

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
	//Scene* scene = new Scene(Scene::SCENE_2D);
	PhysicsScene2D* scene = new PhysicsScene2D(5.0, 20);//0.1,60
	scene->setGravity(Vector2(0.0, 0.0));

	// Setup physics bounds
	// double thickness = 10.0;
	// ScenePrimitive* top = new ScenePrimitive(0, width, thickness, 1.0);
	// top->setPosition(0.0, - height / 2.0);
	// ScenePrimitive* bottom = new ScenePrimitive(0, width, thickness, 1.0);
	// bottom->setPosition(0.0, height / 2.0);
	// ScenePrimitive* left = new ScenePrimitive(0, thickness, height, 1.0);
	// left->setPosition(- width / 2.0, 0.0);
	// ScenePrimitive* right = new ScenePrimitive(0, thickness, height, 1.0);
	// right->setPosition(width / 2.0, 0.0);

	// scene->addPhysicsChild(top, PhysicsScene2DEntity::ENTITY_RECT, true);
	// scene->addPhysicsChild(bottom, PhysicsScene2DEntity::ENTITY_RECT, true);
	// scene->addPhysicsChild(left, PhysicsScene2DEntity::ENTITY_RECT, true);
	// scene->addPhysicsChild(right, PhysicsScene2DEntity::ENTITY_RECT, true);

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
	//reinterpret_cast<PhysicsScene2D*>(scene)->addPhysicsChild(reinterpret_cast<Entity*>(entity), PhysicsScene2DEntity::ENTITY_CIRCLE, false);
}

void poly_scene_physics_set_gravity(poly_scene* s, double gx, double gy)
{
	PhysicsScene2D* scene = reinterpret_cast<PhysicsScene2D*>(s);
	scene->setGravity(Vector2(gx, gy));
}

poly_physics_entity* poly_scene_physics_add_child(poly_scene* s, poly_entity* e, bool is_static)
{
	PhysicsScene2D* scene = reinterpret_cast<PhysicsScene2D*>(s);
	return reinterpret_cast<poly_physics_entity*>(scene->addPhysicsChild(reinterpret_cast<Entity*>(e), PhysicsScene2DEntity::ENTITY_CIRCLE, is_static));
}

void poly_scene_physics_remove_child(poly_scene* s, poly_entity* e)
{
	PhysicsScene2D* scene = reinterpret_cast<PhysicsScene2D*>(s);
	scene->removePhysicsChild(reinterpret_cast<Entity*>(e));	
}

void poly_physics_entity_apply_force(poly_physics_entity* e, double x, double y)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->applyForce(Vector2(x, y));
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

double poly_entity_get_x(poly_entity* entity)
{
	return reinterpret_cast<Entity*>(entity)->getPosition2D().x;
}

double poly_entity_get_y(poly_entity* entity)
{
	return reinterpret_cast<Entity*>(entity)->getPosition2D().y;
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
	std::cout << "entered primitive destroy with " << primitive << std::endl;
	delete reinterpret_cast<ScenePrimitive*>(primitive);
	std::cout << "exited primitive destroy" << std::endl;
}

void poly_scene_primitive_set_circle_options(poly_scene_primitive* primitive, double xSize, double ySize, int segments)
{
	reinterpret_cast<ScenePrimitive*>(primitive)->setPrimitiveOptions(8, xSize, ySize, segments);
}

