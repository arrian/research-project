#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Polycode.h>
#include "PolyExtemporeCore.h"
#include "PolySceneMultiLabel.h"
#include "PolyEntity.h"

using namespace Polycode;

extern "C"
{
	struct poly_core;
	struct poly_entity;
	struct poly_physics_entity;
	struct poly_scene;
	struct poly_scene_image;
	struct poly_scene_mesh;
	struct poly_scene_primitive;
	struct poly_scene_label;
	struct poly_scene_multi_label;

	poly_core* poly_core_create(char* title, float width, float height, bool fullscreen, char* resources);
	void poly_core_update(poly_core* core);
	void poly_core_destroy(poly_core* core);

	poly_scene* poly_scene_create(float width, float height);
	void poly_scene_destroy(poly_scene* scene);
	void poly_scene_add_child(poly_scene* scene, poly_entity* entity);
	void poly_scene_set_ortho_size(poly_scene* scene, int width, int height);

	void poly_scene_physics_set_gravity(poly_scene* scene, double gx, double gy);
	poly_physics_entity* poly_scene_physics_add_child(poly_scene* scene, poly_entity* entity, bool is_static);
	void poly_scene_physics_remove_child(poly_scene* scene, poly_entity* entity);

	void poly_physics_entity_apply_force(poly_physics_entity* entity, double x, double y);
	void poly_physics_entity_apply_torque(poly_physics_entity* entity, double torque);
	void poly_physics_entity_apply_impulse(poly_physics_entity* entity, double x, double y);
	void poly_physics_entity_set_velocity(poly_physics_entity* entity, double x, double y);
	double poly_physics_entity_get_velocity_x(poly_physics_entity* entity);
	double poly_physics_entity_get_velocity_y(poly_physics_entity* entity);
	void poly_physics_entity_set_density(poly_physics_entity* entity, double density);
	double poly_physics_entity_get_density(poly_physics_entity* entity);
	void poly_physics_entity_set_friction(poly_physics_entity* entity, double friction);
	double poly_physics_entity_get_friction(poly_physics_entity* entity);
	void poly_physics_entity_set_collision_category(poly_physics_entity* entity, int category);
	void poly_physics_entity_set_collision_mask(poly_physics_entity* entity, int mask);
	void poly_physics_entity_set_collision_group_index(poly_physics_entity* entity, int group);

	void poly_entity_set_position(poly_entity* entity, double x, double y);
	void poly_entity_set_yaw(poly_entity* entity, double yaw);
	void poly_entity_add_child(poly_entity* target, poly_entity* entity);
	void poly_entity_set_color(poly_entity* entity, double r, double g, double b, double a);
	double poly_entity_get_x(poly_entity* entity);
	double poly_entity_get_y(poly_entity* entity);

	poly_scene_image* poly_scene_image_create(char* path);
	void poly_scene_image_destroy(poly_scene_image* image);

	poly_scene_multi_label* poly_scene_multi_label_create(char* text, int size);
	void poly_scene_multi_label_destroy(poly_scene_multi_label* input);
	void poly_scene_multi_label_set_text(poly_scene_multi_label* label, char* text);

	poly_scene_label* poly_scene_label_create(char* text, int size);
	void poly_scene_label_destroy(poly_scene_label* label);
	void poly_scene_label_set_text(poly_scene_label* label, char* text);
	
	poly_scene_primitive* poly_scene_primitive_create_cube(double xSize, double ySize, double zSize);
	poly_scene_primitive* poly_scene_primitive_create_circle(double xSize, double ySize, int segments);
	void poly_scene_primitive_destroy(poly_scene_primitive* primitive);
	void poly_scene_primitive_set_circle_options(poly_scene_primitive* primitive, double xSize, double ySize, int segments);

	void poly_scene_mesh_set_material(poly_scene_mesh* mesh, char* materialName);
}

int main(int argc, char* argv[])
{
	float width = 640.0;
	float height = 480.0;

	char windowName[] = "Test Window";
	char resourcePath[] = "../resources/default.pak";

	poly_core* core = poly_core_create(windowName, width, height, false, resourcePath);
	poly_scene* scene = poly_scene_create(width, height);

	int i = 0;
	int iterations = 0;
	int maxSize = 20.0;
	int maxEntities = 100;
	bool updateAttractors = true;

	while(true)
	{
		poly_core_update(core);
		
		usleep(100);
		iterations++;

		if(iterations > 100) break;
	}

	return 0;
}


