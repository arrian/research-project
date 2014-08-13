#include <iostream>
#include <sstream>
#include <thread>
#include <sys/time.h>

#include "PolycodeApp.h"
#include "Polycode2DPhysics.h"
#include "PolyPhysicsScreenEntity.h"

/**
 * A 2D physics based attractor.
 */
struct Attractor
{
	PhysicsScene2D* scene;
	Attractor* parent;// null if root
	ScenePrimitive* entity;
	PhysicsScene2DEntity* physicsEntity;

	int type;
	int segments;

	// Transitions

	//Size
	bool finishedSizeTransition;
	double initialSize;
	double currentSize;
	double targetSize;
	unsigned long long sizeStartTime;
	
	//Color
	bool finishedColorTransition;
	Color initialColor;
	Color currentColor;
	Color targetColor;
	unsigned long long colorStartTime;

	unsigned long long transitionTime;


	Attractor(double x, double y, PhysicsScene2D* scene, Attractor* parent, double size, Color color, int type, int segments)
	{
		this->scene = scene;
		this->parent = parent;

		this->finishedSizeTransition = true;
		this->finishedColorTransition = true;

		this->type = type;
		this->segments = segments;

		this->transitionTime = 10000;

		this->entity = new ScenePrimitive(type, size, size, segments);
		entity->setPosition(x, y);
		entity->setColor(color);

		createPhysicsEntity();
	}

	void createPhysicsEntity()
	{
		this->physicsEntity = this->scene->addPhysicsChild(this->entity, PhysicsScene2DEntity::ENTITY_CIRCLE, false);
	}

	void destroyPhysicsEntity()
	{
		scene->removePhysicsChild(reinterpret_cast<Entity*>(this->physicsEntity));
	}

	void setTargetSize(double size)
	{
		this->finishedSizeTransition = false;
		this->initialSize = this->currentSize;
		this->targetSize = size;
		this->sizeStartTime = getTime();
	}

	void setTargetColor(Color color)
	{
		this->finishedColorTransition = false;
		this->initialColor = this->currentColor;
		this->targetColor = color;
		this->colorStartTime = getTime();
	}

	void update()
	{
		unsigned long long now = getTime();
		if(!finishedSizeTransition)
		{
			unsigned long long end = sizeStartTime + transitionTime;
			double newSize;

			if(now > end)
			{
				this->finishedSizeTransition = true;
				newSize = targetSize;
			}
			else newSize = quadraticEaseOut(now, sizeStartTime, sizeStartTime + transitionTime, initialSize, targetSize);

			this->currentSize = newSize;
			reinterpret_cast<ScenePrimitive*>(this->entity)->setPrimitiveOptions(type, newSize, newSize, segments);
		}
		if(!finishedColorTransition)
		{
			unsigned long long end = colorStartTime + transitionTime;
			Color newColor;

			if(now > end)
			{
				this->finishedColorTransition = true;
				newColor = targetColor;
			}
			else newColor = Color(quadraticEaseOut(now, colorStartTime, colorStartTime + transitionTime, initialColor.r, targetColor.r),
								  quadraticEaseOut(now, colorStartTime, colorStartTime + transitionTime, initialColor.g, targetColor.g),
								  quadraticEaseOut(now, colorStartTime, colorStartTime + transitionTime, initialColor.b, targetColor.b),
								  quadraticEaseOut(now, colorStartTime, colorStartTime + transitionTime, initialColor.a, targetColor.a));

			this->currentColor = newColor;
			this->entity->setColor(newColor);
		}

		if(!finishedSizeTransition || !finishedColorTransition)//recreate physics
		{
			destroyPhysicsEntity();
			createPhysicsEntity();
		}
	}

	//hacky easing
	double quadraticEaseOut(unsigned long long now, unsigned long long startTime, unsigned long long endTime, double startValue, double endValue)
	{
		double changeValue = endValue - startValue;
		double changeTime = double(endTime - startTime);
		double currentTime = double(now - startTime);

		std::cout << "changeValue: " << changeValue << " changeTime: " << changeTime << " currentTime: " << currentTime << std::endl;
		// std::cout << "in object: " << this << " quad ease was: " << result << " change time " << changeTime << " current time " << double(now - startTime) << std::endl;


		currentTime /= changeTime;


		double result = -changeValue * currentTime * (currentTime - 2.0) + startValue;

		return result;
	}

	unsigned long long getTime()
	{
		struct timeval tv;

		gettimeofday(&tv, NULL);

		unsigned long long millisecondsSinceEpoch =
		    (unsigned long long)(tv.tv_sec) * 1000 +
		    (unsigned long long)(tv.tv_usec) / 1000;
		return millisecondsSinceEpoch;
	}
};

std::vector<Attractor*> attractorsGlobal;

poly_core* poly_core_create(char* title, int width, int height, char* resources)
{
	GLFWCore* core = new GLFWCore(title, width, height);
	CoreServices::getInstance()->getResourceManager()->addArchive(resources);//"../resources/default.pak"
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	return reinterpret_cast<poly_core*>(core);
}

void poly_core_update(poly_core* core)
{
	for(int i = 0; i < attractorsGlobal.size(); i++)
	{
		Attractor* attractor = attractorsGlobal[i];
		attractor->update();
	}

	reinterpret_cast<Core*>(core)->updateAndRender();
}

void poly_core_destroy(poly_core* core)
{
	delete reinterpret_cast<Core*>(core);
}

poly_scene* poly_scene_create(int width, int height)
{
	//Scene* scene = new Scene(Scene::SCENE_2D);
	PhysicsScene2D* scene = new PhysicsScene2D(5.0, 60);//0.1,60
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
	//reinterpret_cast<Scene*>(scene)->addChild(reinterpret_cast<Entity*>(entity));
	reinterpret_cast<PhysicsScene2D*>(scene)->addPhysicsChild(reinterpret_cast<Entity*>(entity), PhysicsScene2DEntity::ENTITY_CIRCLE, false);
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

poly_attractor* poly_attractor_add(poly_scene* s, poly_attractor* a, double x, double y, double size)
{
	PhysicsScene2D* scene = reinterpret_cast<PhysicsScene2D*>(s);
	Color color(RANDOM_NUMBER, RANDOM_NUMBER * 0.5 + 0.5, 1.0, RANDOM_NUMBER);
	int type = 8;//circle
	int segments = 6;

	Attractor* attractor = new Attractor(x, y, scene, nullptr, size, color, type, segments);
	attractorsGlobal.push_back(attractor);
	return reinterpret_cast<poly_attractor*>(attractor);
}

void poly_attractor_remove(poly_scene* s, poly_attractor* a)
{
	PhysicsScene2D* scene = reinterpret_cast<PhysicsScene2D*>(s);
	Attractor* attractor = reinterpret_cast<Attractor*>(a);
	scene->removePhysicsChild(attractor->entity);
	attractorsGlobal.erase(std::remove(attractorsGlobal.begin(), attractorsGlobal.end(), attractor), attractorsGlobal.end());
	delete attractor->entity;
	delete attractor;
}

void poly_attractor_target_size(poly_attractor* a, double size)
{
	Attractor* attractor = reinterpret_cast<Attractor*>(a);
	attractor->setTargetSize(size);
}

void poly_attractor_target_color(poly_attractor* att, double r, double g, double b, double a)
{
	Attractor* attractor = reinterpret_cast<Attractor*>(att);
	attractor->setTargetColor(Color(r,g,b,a));
}

void poly_attractor_impulse(poly_attractor* a, double x, double y)
{
	Attractor* attractor = reinterpret_cast<Attractor*>(a);
}



