#pragma once

#include <Polycode.h>

using namespace Polycode;

class PolyBind
{
public:
	PolyBind(int width, int height);
	~PolyBind();

	void update();

	void addResourceDirectory(std::string dir);

	SceneMesh* addText(std::string text, int size);
	SceneMesh* addImage(std::string path);
	SceneMesh* addCircle(float x, float y, float r, float g, float b);
	SceneMesh* addLine(SceneMesh* source, SceneMesh* target);

	void setPosition(SceneMesh* mesh, float x, float y);
	void setColor(SceneMesh* mesh, float r, float g, float b);
	void setRoll(SceneMesh* mesh, int degree);
	void setScale(SceneMesh* mesh, float xs, float ys);

private:
	Code* core;

};

