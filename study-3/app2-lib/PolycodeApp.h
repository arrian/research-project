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

	poly_scene_multi_label* label = poly_scene_multi_label_create("test text\nnext line here\nanother line\n\na line after an empty line", 20);

	//poly_scene_add_child(scene, (poly_entity*) poly_scene_label_create("test text", 30));
	//poly_scene_add_child(scene, (poly_entity*) poly_scene_image_create("../resources/gradient.png"));
	poly_scene_add_child(scene, (poly_entity*) label);

	//poly_scene_add_child(scene, (poly_entity*) poly_scene_primitive_create_cube(30.0, 30.0, 20.0));

	while(true)
	{
		poly_core_update(core);
		poly_scene_multi_label_set_text(label, "1 fa flsdjklsadf ;sjlkadf asljdf jklasdf  here\n2 multiple linenew textasdkfljasd;lfdks fa flsdjklsadf ;sjlkadf asljdf jklasdf jklasdf s\n3 new tnew textasdkfljasd;lfdks fa flsdjklsadf ;sjlkadf asljdf jklasdf new textasdkfljasd;lfdks fa here\n4 multiple lines\n5 new text here\n6 multiple lines\n7 new text here\n8 multiple lines\n9 new text here\n10 multiple lines\n");
	}

	return 0;
}


