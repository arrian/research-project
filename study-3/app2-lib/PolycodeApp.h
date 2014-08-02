#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Polycode.h>
#include "PolyGLFWCore.h"

using namespace Polycode;

class PolycodeApp {
public:
    PolycodeApp(int width, int height);
    ~PolycodeApp();

    bool update();

    Core* getCore();
    Scene* getScene();

private:
	Core* core;
	Scene* scene;
};


int main(int argc, char* argv[])
{
	PolycodeApp* app = new PolycodeApp(640, 480);
	
	while(true)
	{
		app->update();
	}

	return 0;
}


extern "C"
{

	struct poly_core;
	struct poly_entity;
	struct poly_scene;
	struct poly_scene_image;
	struct poly_scene_mesh;
	struct poly_scene_label;
	struct poly_ui_input;

	poly_core* poly_core_create(int width, int height);
	void poly_core_update(poly_core* core);
	void poly_core_destroy(poly_core* core);

	poly_scene* poly_scene_create();
	void poly_scene_destroy(poly_scene* scene);
	void poly_scene_add_child(poly_scene* scene, poly_entity* entity);

	poly_scene_image* poly_scene_image_create(char* path);
	void poly_scene_image_destroy(poly_scene_image* image);

// ;;scene mesh
// (bind-alias poly_scene_mesh i8)
// (bind-lib polycode poly_scene_mesh_create [poly_scene_mesh*]*)
// (bind-lib polycode poly_scene_mesh_destroy [void,poly_scene_mesh*]*)

// ;;scene label
// (bind-alias poly_scene_label i8)
// (bind-lib polycode poly_scene_label_create [poly_scene_label*,i8*]*)
// (bind-lib polycode poly_scene_label_destroy [void,poly_scene_label*]*)

// ;;scene label
// (bind-alias poly_ui_input i8)
// (bind-lib polycode poly_ui_input_create [poly_ui_input*,i8*]*)
// (bind-lib polycode poly_ui_input_destroy [void,poly_ui_input*]*)
}




