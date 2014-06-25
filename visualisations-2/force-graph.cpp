#include "force-graph.h"


ForceGraph::ForceGraph(CodeTree* code, ForceGraph* parent)
	: x(rand() % 100 + 1),
	  y(rand() % 100 + 1),
	  vx(0.0),
	  vy(0.0),
	  size(10.0),
	  data(code),
	  parent(parent),
	  children()
{
	for(std::vector<CodeTree*>::iterator it = code->children.begin(); it != code->children.end(); ++it)//iterate children of code
	{
		children.push_back(new ForceGraph(*it, this));
	}
}

ForceGraph::~ForceGraph()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

void ForceGraph::update(CodeTree* code)
{
	//iterate this and code and compare
}

float squared_distance(float p1, float p2, float q1, float q2)
{
	return fabs((p1 - q1) * (p1 - q1) + (p2 - q2) * (p2 - q2));
}

void ForceGraph::step(float dt)
{
	std::vector<ForceGraph*> vertices = getVertices();
	std::vector<std::pair<ForceGraph*, ForceGraph*> > edges = getEdges();



	float multiplier = 1.0f;
	float area = 10000.0f;//width * height
    float maxDisplace = sqrt(multiplier * area) / 10.0f;
    float k = sqrt((multiplier * area) / (1.0f + vertices.size()));

    for(auto v : vertices)
    {
    	v->tx = 0.0;
    	v->ty = 0.0;
    }

	for(auto v1 : vertices)
	{
		for(auto v2 : vertices)
		{
			if(v1 == v2) continue;
			float xd = v1->x - v2->x;
			float yd = v1->y - v2->y;
			float dist = sqrt(xd * xd + yd * yd);

			if(dist > 0)
			{
				float replusive = k * k / dist;
				v1->tx += xd / dist * replusive;
				v1->ty += yd / dist * replusive;
			}
		}
	}

	for(auto e : edges)
	{
		ForceGraph* source = e.first;
		ForceGraph* target = e.second;

		float xd = source->x - target->x;
		float yd = source->y - target->y;
		float dist = sqrt(xd * xd + yd * yd);
		
		float attractive = dist * dist / k;

		if(dist > 0)
		{
			source->tx -= xd / dist * attractive;
			source->ty -= yd / dist * attractive;
			target->tx += xd / dist * attractive;
			target->ty += yd / dist * attractive;
		}
	}

	for(auto v : vertices)
	{
		float dist = sqrt(v->tx * v->tx + v->ty * v->ty);
		if(dist > 0)
		{
			std::cout << v->x << " " << v->y << std::endl;
			v->x += (v->tx / dist);
			v->y += (v->ty / dist);
		}
	}
}

void ForceGraph::print(int depth)
{
	for(int i = 0; i < depth; i++) std::cout << "--";
	std::cout << data->tag << std::endl;

	for(std::vector<ForceGraph*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->print(depth + 1);
	}
}

std::vector<ForceGraph*> ForceGraph::getVertices()
{
	std::vector<ForceGraph*> vertices;
	vertices.push_back(this);

	for(std::vector<ForceGraph*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		std::vector<ForceGraph*> childVertices = (*it)->getVertices();
		vertices.insert(vertices.end(), childVertices.begin(), childVertices.end());
	}

	return vertices;
}

std::vector<std::pair<ForceGraph*, ForceGraph*> > ForceGraph::getEdges()
{
	std::vector<std::pair<ForceGraph*, ForceGraph*> > edges;

	for(std::vector<ForceGraph*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		edges.push_back(std::pair<ForceGraph*, ForceGraph*>(this, *it));

		std::vector<std::pair<ForceGraph*, ForceGraph*> > childEdges = (*it)->getEdges();
		edges.insert(edges.end(), childEdges.begin(), childEdges.end());
	}

	return edges;
}




forcegraph* forcegraph_create(codetree* code)
{
	return reinterpret_cast<forcegraph*>(new ForceGraph(reinterpret_cast<CodeTree*>(code)));
}

void forcegraph_destroy(forcegraph* graph)
{
	delete reinterpret_cast<ForceGraph*>(graph);
}

void forcegraph_update(forcegraph* graph, codetree* update)
{
	(reinterpret_cast<ForceGraph*>(graph))->update(reinterpret_cast<CodeTree*>(update));
}

void forcegraph_step(forcegraph* graph, float dt)
{
	(reinterpret_cast<ForceGraph*>(graph))->step(dt);
}

float forcegraph_get_x(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->x;
}

float forcegraph_get_y(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->y;
}

float forcegraph_get_size(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->size;
}

int forcegraph_get_child_count(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->children.size();
}

forcegraph* forcegraph_get_child(forcegraph* graph, int index)
{
	return reinterpret_cast<forcegraph*>((reinterpret_cast<ForceGraph*>(graph))->children.at(index));
}

codetree* forcegraph_get_code(forcegraph* graph)
{
	return (reinterpret_cast<codetree*>(reinterpret_cast<ForceGraph*>(graph)->data));
}

void forcegraph_print(forcegraph* graph)
{
	(reinterpret_cast<ForceGraph*>(graph))->print();	
}



