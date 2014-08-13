#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Polycode.h>
#include "PolyGLFWCore.h"
#include "PolySceneMultiLabel.h"
#include "PolyEntity.h"

using namespace Polycode;

extern "C"
{
	struct poly_core;
	struct poly_entity;
	struct poly_scene;
	struct poly_scene_image;
	struct poly_scene_mesh;
	struct poly_scene_primitive;
	struct poly_scene_label;
	struct poly_scene_multi_label;

	poly_core* poly_core_create(char* title, int width, int height, char* resources);
	void poly_core_update(poly_core* core);
	void poly_core_destroy(poly_core* core);

	poly_scene* poly_scene_create(int width, int height);
	void poly_scene_destroy(poly_scene* scene);
	void poly_scene_add_child(poly_scene* scene, poly_entity* entity);

	void poly_entity_set_position(poly_entity* entity, double x, double y);
	void poly_entity_set_yaw(poly_entity* entity, double yaw);
	void poly_entity_add_child(poly_entity* target, poly_entity* entity);
	void poly_entity_set_color(poly_entity* entity, double r, double g, double b, double a);

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
	
	//Attractors - requires a physics enabled scene

	struct poly_attractor;
	poly_attractor* poly_attractor_add(poly_scene* scene, poly_attractor* attractor, double x, double y, double size);
	void poly_attractor_remove(poly_attractor* attractor);
	// void poly_attractor_pulse_size(double next, double final);
	// void poly_attractor_pulse_color(double nr, double ng, double nb, double na, double fr, double fg, double fb, double fa);
	void poly_attractor_target_size(poly_attractor* a, double size);
	void poly_attractor_target_color(poly_attractor* att, double r, double g, double b, double a);
	void poly_attractor_impulse(poly_attractor* a, double x, double y);
}

int main(int argc, char* argv[])
{
	int width = 640;
	int height = 480;

	poly_core* core = poly_core_create("Test Window", width, height, "../resources/default.pak");
	poly_scene* scene = poly_scene_create(width, height);

// poly_attractor_add(scene, nullptr, 100.0, 100.0, 100.0);

	// poly_scene_multi_label* label = poly_scene_multi_label_create("test text\nnext line here\nanother line\n\na line after an empty line", 20);

	//poly_scene_add_child(scene, (poly_entity*) poly_scene_label_create("test text", 30));
	//poly_scene_add_child(scene, (poly_entity*) poly_scene_image_create("../resources/gradient.png"));
	//poly_scene_add_child(scene, (poly_entity*) label);

	//poly_scene_add_child(scene, (poly_entity*) poly_scene_primitive_create_cube(30.0, 30.0, 20.0));

	int i = 0;
	std::vector<poly_attractor*> attractors;
	int iterations = 0;
	int maxSize = 20.0;
	int maxEntities = 100;

	poly_attractor* a = poly_attractor_add(scene, nullptr, RANDOM_NUMBER * maxSize, RANDOM_NUMBER * maxSize,  RANDOM_NUMBER * maxSize);	
	poly_attractor_target_size(a, 100.0);
	poly_attractor_target_color(a, 0.0, 1.0, 0.0, 1.0);

	while(true)
	{
		std::cout << iterations << std::endl;
		poly_core_update(core);

		if(i < maxEntities && iterations % 20 == 0) 
		{
			attractors.push_back(poly_attractor_add(scene, nullptr, RANDOM_NUMBER * width, RANDOM_NUMBER * height,  RANDOM_NUMBER * maxSize + 20.0));	
			i++;
		}

		if(attractors.size() > 0 && iterations % 50 == 0)
		{
			poly_attractor_target_size(attractors[int(RANDOM_NUMBER * double(attractors.size() - 1))], 200.0);
		} 
		
		usleep(10000);
		iterations++;
	}

	return 0;
}


