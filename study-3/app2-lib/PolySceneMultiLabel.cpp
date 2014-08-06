#include "PolySceneMultiLabel.h"

#include <iostream>

using namespace Polycode;

SceneMultiLabel::SceneMultiLabel(String text, double size, double spacing) : Entity()
{
	// std::cout << "entered create scene multi label" << std::endl;
	this->size = size;
	this->spacing = spacing;
	setText(text);
	// std::cout << "exited create scene multi label" << std::endl;
}

SceneMultiLabel::~SceneMultiLabel()
{
	// std::cout << "entered destroy scene multi label" << std::endl;
	if(!ownsChildren) {
        clearLabels();
    }
    // std::cout << "exited destroy scene multi label" << std::endl;
}


void SceneMultiLabel::setText(String text)
{
	// std::cout << "entered set scene multi label text" << std::endl;
    clearLabels();
    
    std::vector<String> lines = text.split("\n");
    
    Number lineSize = spacing;
    Number yPos = 0.0;
    for(int i=0; i < lines.size(); i++) {
        if(lines[i] == "") {
            yPos += lineSize + spacing;
        } else {
            SceneLabel *label = new SceneLabel(lines[i], size, "sans");
			
			//Label config
			label->setBlendingMode(Renderer::BLEND_MODE_NORMAL);
			label->positionAtBaseline = true;
			label->setAnchorPoint(-1.0, -1.0, 0.0);
			label->snapToPixels = true;

			//Label position
            lineSize = label->getHeight();
            label->setPositionY(yPos);
            yPos += label->getHeight() + spacing;
            std::cout << "adding label "  << label->getText().getSTLString() << std::endl;
            addChild(label);
            labels.push_back(label);
        }
    }
    // std::cout << "exited set scene multi label text" << std::endl;
}

String SceneMultiLabel::getText()
{
	// std::cout << "entered get scene multi label text" << std::endl;
	String text;
    for(int i=0; i < labels.size(); i++) {
        if(i != 0) {
            text += "\n";
        }
        text += labels[i]->getText();
    }
    // std::cout << "exited get scene multi label text" << std::endl;
    return text;
}

double SceneMultiLabel::getHeight()
{
	return 0.0;
}

double SceneMultiLabel::getWidth()
{
	return 0.0;
}

void SceneMultiLabel::clearLabels()
{
	// std::cout << "entered scene multi label clear labels" << std::endl;
    for(int i=0; i < labels.size(); i++) {
        std::cout << "deleted label "  << labels[i]->getText().getSTLString() << std::endl;
        removeChild(labels[i]);
        delete labels[i];
    }
    labels.clear();
    // std::cout << "exited scene multi label clear labels" << std::endl;
}

