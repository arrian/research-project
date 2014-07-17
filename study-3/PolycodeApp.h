#include <Polycode.h>
#include "PolycodeView.h"

using namespace Polycode;

class PolycodeApp {
public:
    PolycodeApp(PolycodeView *view);
    ~PolycodeApp();
    bool Update();

    Core* getCore();
    Scene* getScene();

    void clear();
    
	std::vector<ScenePrimitive*> clearables;
	std::vector<SceneLine*> clearableLines;
	std::vector<SceneLabel*> clearableLabels;

	SceneEntity* circles;
	SceneEntity* lines;
	SceneEntity* text;

private:
	Core* core;
	Scene* scene;


};


extern "C"
{
	struct polycode;
	struct polycode_scenemesh;

	struct polycode_scenemesh* polycode_add_text(polycode* pc, float x, float y, char* text, int size);
	struct polycode_scenemesh* polycode_add_image(polycode* pc, char* path);
	struct polycode_scenemesh* polycode_add_circle(polycode* pc, float x, float y, float w, float h, int segments, float r, float g, float b);
	struct polycode_scenemesh* polycode_add_line(polycode* pc, polycode_scenemesh* source, polycode_scenemesh* target, float width);
	float polycode_add_code(polycode* pc, float x, float y, char* text, int size);

	void polycode_set_position(polycode_scenemesh* mesh, float x, float y);
	void polycode_set_color(polycode_scenemesh* mesh, float r, float g, float b);
	void polycode_set_roll(polycode_scenemesh* mesh, int degree);
	void polycode_set_scale(polycode_scenemesh* mesh, float sx, float sy);

	void polycode_clear_scene(polycode* pc);
}