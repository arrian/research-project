#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Polycode.h>
#include "PolyGLFWCore.h"

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
	struct poly_ui_input;

	poly_core* poly_core_create(char* title, int width, int height, char* resources);
	void poly_core_update(poly_core* core);
	void poly_core_destroy(poly_core* core);

	poly_scene* poly_scene_create(int width, int height);
	void poly_scene_destroy(poly_scene* scene);
	void poly_scene_add_child(poly_scene* scene, poly_entity* entity);

	poly_scene_image* poly_scene_image_create(char* path);
	void poly_scene_image_destroy(poly_scene_image* image);

	poly_ui_input* poly_ui_input_create(char* text);
	void poly_ui_input_destroy(poly_ui_input* input);

	poly_scene_label* poly_scene_label_create(char* text, int size);
	void poly_scene_label_destroy(poly_scene_label* label);
	void poly_scene_label_set_text(poly_scene_label* label, char* text);
	
	poly_scene_primitive* poly_scene_primitive_create_cube(float xSize, float ySize, float zSize);
	poly_scene_primitive* poly_scene_primitive_create_circle(float xSize, float ySize, float segments);
	void poly_scene_primitive_destroy(poly_scene_primitive* primitive);
	
// ;;scene mesh
// (bind-alias poly_scene_mesh i8)
// (bind-lib polycode poly_scene_mesh_create [poly_scene_mesh*]*)
// (bind-lib polycode poly_scene_mesh_destroy [void,poly_scene_mesh*]*)

}

int main(int argc, char* argv[])
{
	int width = 640;
	int height = 480;

	poly_core* core = poly_core_create("Test Window", width, height, "../resources/default.pak");
	poly_scene* scene = poly_scene_create(width, height);

	poly_scene_add_child(scene, (poly_entity*) poly_scene_label_create("test text", 30));
	//poly_scene_add_child(scene, (poly_entity*) poly_scene_image_create("../resources/gradient.png"));
	poly_scene_add_child(scene, (poly_entity*) poly_ui_input_create("test text\nnext line here"));

	while(true)
	{
		poly_core_update(core);
	}

	return 0;
}


