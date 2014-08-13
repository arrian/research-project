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

		this->transitionTime = 1000;

		this->currentSize = size;

		this->entity = new ScenePrimitive(type, size, size, segments);
		entity->setPosition(x, y);
		entity->setColor(color);

		createEntity();
	}

	void createEntity()
	{
		this->physicsEntity = this->scene->addPhysicsChild(this->entity, PhysicsScene2DEntity::ENTITY_CIRCLE, false);		
	}

	void destroyEntity()
	{
		scene->removePhysicsChild(this->entity);
		this->physicsEntity = nullptr;
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

			if(now >= end)
			{
				this->finishedSizeTransition = true;
				newSize = targetSize;
			}
			else newSize = quadraticEaseOut(int(now - sizeStartTime), initialSize, targetSize - initialSize, transitionTime);
			std::cout << newSize << std::endl;
			this->currentSize = newSize;
			reinterpret_cast<ScenePrimitive*>(this->entity)->setPrimitiveOptions(type, newSize, newSize, segments);
		}
		if(!finishedColorTransition)
		{
			unsigned long long end = colorStartTime + transitionTime;
			Color newColor;

			if(now >= end)
			{
				this->finishedColorTransition = true;
				newColor = targetColor;
			}
			else newColor = Color(quadraticEaseOut(now - colorStartTime, initialColor.r, targetColor.r - initialColor.r, transitionTime),
								  quadraticEaseOut(now - colorStartTime, initialColor.g, targetColor.g - initialColor.g, transitionTime),
								  quadraticEaseOut(now - colorStartTime, initialColor.b, targetColor.b - initialColor.b, transitionTime),
								  quadraticEaseOut(now - colorStartTime, initialColor.a, targetColor.a - initialColor.a, transitionTime));

			this->currentColor = newColor;
			this->entity->setColor(newColor);
		}

		if(!finishedSizeTransition || !finishedColorTransition)//recreate physics
		{
			destroyEntity();
			createEntity();
		}

		this->physicsEntity->applyForce(attract());
	}

	Vector2 attract() 
	{
		double G = 1000.0;
		
		Vector2 pos = Vector2(0.0,0.0);    
		Vector2 moverPos = this->entity->getPosition2D();
		
		Vector2 force = pos - moverPos;
		double distance = force.length();

		distance = std::max(1.0, std::min(distance, 10.0));
		force.Normalize();
		
		double strength = (G * 1 * 10.0) / (distance * distance); 
		return force * strength;
	}

	double quadraticEaseOut(int current, double startValue, double changeValue, int duration)
	{
		//if(duration == 0) return startValue;
		double td = double(current) / double(duration);
		double result = -changeValue * td * (td - 2.0) + startValue;

		if(result > 1000.0) 
		{
			std::cout << "current: " << current << " startValue: " << startValue << " changeValue: " << changeValue << " duration: " << duration << std::endl;
			throw std::runtime_error("quadratic ease out got a strange result");
		}

		// if(result > 1000.0)
		// {
		// 	std::cout << "quadtratic ease out result was too high" << std::endl;
		// 	return 1.0;
		// }
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
	Color color(RANDOM_NUMBER, RANDOM_NUMBER * 0.5 + 0.5, 1.0, 1.0);
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



