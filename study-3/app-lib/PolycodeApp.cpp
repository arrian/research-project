#include <iostream>
#include <sstream>
#include <thread>
#include <sys/time.h>

#include "PolycodeApp.h"
#include "Polycode2DPhysics.h"
#include "PolyPhysicsScreenEntity.h"
#include "PolySceneLine.h"

poly_core* poly_core_create(char* title, float width, float height, bool fullscreen, char* resources)
{
	ExtemporeCore* core = new ExtemporeCore(title, (int) width, (int) height, fullscreen);
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

poly_scene* poly_scene_create(float width, float height)
{
	PhysicsScene2D* scene = new PhysicsScene2D(5.0, 20);//0.1,60
	scene->setGravity(Vector2(0.0, 0.0));
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

void poly_scene_set_ortho_size(poly_scene* scene, int width, int height)
{
	reinterpret_cast<PhysicsScene2D*>(scene)->getActiveCamera()->setOrthoSize(width, height);
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

void poly_physics_entity_apply_torque(poly_physics_entity* e, double torque)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->applyTorque(torque);
}

void poly_physics_entity_apply_impulse(poly_physics_entity* e, double x, double y)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->applyImpulse(x, y);
}

void poly_physics_entity_set_velocity(poly_physics_entity* e, double x, double y)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setVelocity(x, y);
}

double poly_physics_entity_get_velocity_x(poly_physics_entity* e)
{
	return reinterpret_cast<PhysicsScene2DEntity*>(e)->body->GetLinearVelocity().x;
}

double poly_physics_entity_get_velocity_y(poly_physics_entity* e)
{
	return reinterpret_cast<PhysicsScene2DEntity*>(e)->body->GetLinearVelocity().y;
}

void poly_physics_entity_set_density(poly_physics_entity* e, double density)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setDensity(density);
}

double poly_physics_entity_get_density(poly_physics_entity* e)
{
	return reinterpret_cast<PhysicsScene2DEntity*>(e)->getDensity();
}

void poly_physics_entity_set_friction(poly_physics_entity* e, double friction)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setFriction(friction);
}

double poly_physics_entity_get_friction(poly_physics_entity* e)
{
	return reinterpret_cast<PhysicsScene2DEntity*>(e)->getFriction();
}

void poly_physics_entity_set_collision_category(poly_physics_entity* e, int category)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setCollisionCategory(category);
}

void poly_physics_entity_set_collision_mask(poly_physics_entity* e, int mask)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setCollisionMask(mask);
}

void poly_physics_entity_set_collision_group_index(poly_physics_entity* e, int groupIndex)
{
	reinterpret_cast<PhysicsScene2DEntity*>(e)->setCollisionGroupIndex(groupIndex);
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
	std::cout << "primitive destroy with " << primitive << std::endl;
	delete reinterpret_cast<ScenePrimitive*>(primitive);
}

void poly_scene_primitive_set_circle_options(poly_scene_primitive* primitive, double xSize, double ySize, int segments)
{
	reinterpret_cast<ScenePrimitive*>(primitive)->setPrimitiveOptions(8, xSize, ySize, segments);
}

poly_scene_line* poly_scene_line_create(poly_entity* e1, poly_entity* e2)
{
	return reinterpret_cast<poly_scene_line*>(new SceneLine(reinterpret_cast<Entity*>(e1), reinterpret_cast<Entity*>(e2)));
}

void poly_scene_line_destroy(poly_scene_line* line)
{
	delete reinterpret_cast<SceneLine*>(line);
}

void poly_scene_mesh_set_material(poly_scene_mesh* mesh, char* materialName)
{
	reinterpret_cast<SceneMesh*>(mesh)->setMaterialByName(materialName);
}

