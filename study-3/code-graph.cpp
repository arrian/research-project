#include "code-graph.h"

#include <ctime>


CodeGraph::CodeGraph(CodeTree* code, CodeGraph* parent)
	: x(rand() % 100 + 1),
	  y(rand() % 100 + 1),
	  size(code->children.size()),//10.0),
	  vx(0.0),
	  vy(0.0),
	  timeAdded(std::time(0)),
	  timeActivated(0),
	  active(false),
	  //data(code),
	  parent(parent),
	  children(),
	  tag(code->tag),
	  type(code->type),
	  code(code->code)	  
{
	//if(type == ROOT)
	//{
		size = 1.0;
		for(auto codeChild : code->children)
		{
			children.push_back(new CodeGraph(codeChild, this));
		}
	//}
}

CodeGraph::~CodeGraph()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

void CodeGraph::addChild(CodeTree* tree)
{
	children.push_back(new CodeGraph(tree, this));
}

void CodeGraph::update(CodeTree* code)
{
	children.erase(std::remove_if(children.begin(), children.end(), 
                   [&](CodeGraph* f) { 
						bool match = false;
						
						for(auto codeChild : code->children)
						{	
							if (f->isMatch(codeChild)) 
							{
								match = true;
								break;
							}
						}

                   		return !match;
                   }), children.end());

	for(auto codeChild : code->children)
	{
		bool match = false;
		for(auto graphChild : children)
		{
			if(graphChild->isMatch(codeChild))
			{
				match = true;
				break;
			}
		}
		if(!match) children.push_back(new CodeGraph(codeChild, this));

	}
}

bool CodeGraph::isMatch(CodeTree* code)
{
	// std::cout << "MATCH" << std::endl;
	// std::cout << code->code << std::endl << "------------- vs -------------" << std::endl << this->code << std::endl;
	return code->code == this->code;//change to approximately equals
}

void CodeGraph::step(float dt)
{
	std::vector<CodeGraph*> vertices = getVertices();
	std::vector<std::pair<CodeGraph*, CodeGraph*> > edges = getEdges();

	float multiplier = 1.0f;//1.0f
	float area = 7000.0f;//width * height
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
		CodeGraph* source = e.first;
		CodeGraph* target = e.second;

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
		if(dist > 0.0)
		{
			// std::cout << v->x << " " << v->y << std::endl;
			v->x += (v->tx / dist);
			v->y += (v->ty / dist);
		}
	}
}

void CodeGraph::print(int depth)
{
	int nextDepth = depth + 1;
	if(tag == "statement")
	{
		nextDepth = depth;
	}
	else
	{
		for(int i = 0; i < depth; i++) std::cout << "--";
		std::cout << tag << std::endl;
	}
	
	for(std::vector<CodeGraph*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->print(nextDepth);
	}
}

std::vector<CodeGraph*> CodeGraph::getVertices()
{
	std::vector<CodeGraph*> vertices;
	vertices.push_back(this);

	for(auto graphChild : children)
	{
		std::vector<CodeGraph*> childVertices = graphChild->getVertices();
		vertices.insert(vertices.end(), childVertices.begin(), childVertices.end());
	}

	return vertices;
}

std::vector<std::pair<CodeGraph*, CodeGraph*> > CodeGraph::getEdges()
{
	std::vector<std::pair<CodeGraph*, CodeGraph*> > edges;

	for(auto graphChild : children)
	{
		edges.push_back(std::pair<CodeGraph*, CodeGraph*>(this, graphChild));

		std::vector<std::pair<CodeGraph*, CodeGraph*> > childEdges = graphChild->getEdges();
		edges.insert(edges.end(), childEdges.begin(), childEdges.end());
	}

	return edges;
}


codegraph* codegraph_create(codetree* code)
{
	// std::cout << "Allocating codegraph" << std::endl;
	return reinterpret_cast<codegraph*>(new CodeGraph(reinterpret_cast<CodeTree*>(code)));
}

void codegraph_destroy(codegraph* graph)
{
	// std::cout << "Deallocating codegraph" << std::endl;
	delete reinterpret_cast<CodeGraph*>(graph);
}

void codegraph_update(codegraph* graph, codetree* update)
{
	(reinterpret_cast<CodeGraph*>(graph))->update(reinterpret_cast<CodeTree*>(update));
}

void codegraph_step(codegraph* graph, float dt)
{
	(reinterpret_cast<CodeGraph*>(graph))->step(dt);
}

void codegraph_add_child(codegraph* graph, codetree* code)
{
	(reinterpret_cast<CodeGraph*>(graph))->addChild(reinterpret_cast<CodeTree*>(code));
}

float codegraph_get_x(codegraph* graph)
{
	return (reinterpret_cast<CodeGraph*>(graph))->x;
}

float codegraph_get_y(codegraph* graph)
{
	return (reinterpret_cast<CodeGraph*>(graph))->y;
}

float codegraph_get_size(codegraph* graph)
{
	return (reinterpret_cast<CodeGraph*>(graph))->size;
}

int codegraph_get_child_count(codegraph* graph)
{
	return (reinterpret_cast<CodeGraph*>(graph))->children.size();
}

codegraph* codegraph_get_child(codegraph* graph, int index)
{
	return reinterpret_cast<codegraph*>((reinterpret_cast<CodeGraph*>(graph))->children.at(index));
}

int codegraph_get_type(codegraph* graph)
{
	return codeTypeToInteger(reinterpret_cast<CodeGraph*>(graph)->type);
}

const char* codegraph_get_tag(codegraph* graph)
{
	return reinterpret_cast<CodeGraph*>(graph)->tag.c_str();
}

const char* codegraph_get_code(codegraph* graph)
{
	return reinterpret_cast<CodeGraph*>(graph)->code.c_str();
}

int codegraph_get_time_added(codegraph* graph)
{
	return reinterpret_cast<CodeGraph*>(graph)->timeAdded;
}

int codegraph_get_time_activated(codegraph* graph)
{
	return reinterpret_cast<CodeGraph*>(graph)->timeActivated;
}

int codegraph_is_active(codegraph* graph)
{
	return reinterpret_cast<CodeGraph*>(graph)->active;
}

void codegraph_print(codegraph* graph)
{
	(reinterpret_cast<CodeGraph*>(graph))->print();	
}



