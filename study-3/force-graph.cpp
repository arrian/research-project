#include "force-graph.h"


ForceGraph::ForceGraph(CodeTree* code, ForceGraph* parent)
	: x(rand() % 100 + 1),
	  y(rand() % 100 + 1),
	  vx(0.0),
	  vy(0.0),
	  size(10.0),
	  //data(code),
	  tag(code->tag),
	  type(code->type),
	  code(code->code),
	  parent(parent),
	  children()
{
	for(auto codeChild : code->children)//iterate children of code
	{
		children.push_back(new ForceGraph(codeChild, this));
	}
}

ForceGraph::~ForceGraph()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

void ForceGraph::addChild(CodeTree* tree)
{
	children.push_back(new ForceGraph(tree, this));
}

void ForceGraph::update(CodeTree* code)
{
	// std::cout << "----------------started erase from the list--------------------------" << std::endl;	

	children.erase(std::remove_if(children.begin(), children.end(), 
                   [&](ForceGraph* f) { 
						bool match = false;
						for(auto codeChild : code->children)//iterate children of code
						{	
							// std::cout << "started" << std::endl;
							// std::cout << "code child" << codeChild << std::endl;
							// std::cout << "code" << codeChild->code << std::endl;
							// std::cout << "graph child" << f << std::endl;
							// std::cout << "graph" << f->code << std::endl;
							if (f->isMatch(codeChild)) 
							{
								match = true;
								break;
							}
							// std::cout << "finished" << std::endl;
						}
						// std::cout << "size" << children.size() << std::endl;
                   		return !match;
                   }), children.end());

	// std::cout << "----------------erased from the list--------------------------" << std::endl;

	for(auto codeChild : code->children)//iterate children of code
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
		if(!match) children.push_back(new ForceGraph(codeChild, this));//add new node... check for sub matches
	}

	//iterate this and code and compare
}

bool ForceGraph::isMatch(CodeTree* code)
{
	// std::cout << "MATCH" << std::endl;
	// std::cout << code->code << std::endl << "------------- vs -------------" << std::endl << this->code << std::endl;
	return code->code == this->code;//change to approximately equals
}

void ForceGraph::step(float dt)
{
	std::vector<ForceGraph*> vertices = getVertices();
	std::vector<std::pair<ForceGraph*, ForceGraph*> > edges = getEdges();

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
	std::cout << tag << std::endl;

	for(std::vector<ForceGraph*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->print(depth + 1);
	}
}

std::vector<ForceGraph*> ForceGraph::getVertices()
{
	std::vector<ForceGraph*> vertices;
	vertices.push_back(this);

	for(auto graphChild : children)
	{
		std::vector<ForceGraph*> childVertices = graphChild->getVertices();
		vertices.insert(vertices.end(), childVertices.begin(), childVertices.end());
	}

	return vertices;
}

std::vector<std::pair<ForceGraph*, ForceGraph*> > ForceGraph::getEdges()
{
	std::vector<std::pair<ForceGraph*, ForceGraph*> > edges;

	for(auto graphChild : children)
	{
		edges.push_back(std::pair<ForceGraph*, ForceGraph*>(this, graphChild));

		std::vector<std::pair<ForceGraph*, ForceGraph*> > childEdges = graphChild->getEdges();
		edges.insert(edges.end(), childEdges.begin(), childEdges.end());
	}

	return edges;
}


forcegraph* forcegraph_create(codetree* code)
{
	// std::cout << "Allocating forcegraph" << std::endl;
	return reinterpret_cast<forcegraph*>(new ForceGraph(reinterpret_cast<CodeTree*>(code)));
}

void forcegraph_destroy(forcegraph* graph)
{
	// std::cout << "Deallocating forcegraph" << std::endl;
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

void forcegraph_add_child(forcegraph* graph, codetree* code)
{
	(reinterpret_cast<ForceGraph*>(graph))->addChild(reinterpret_cast<CodeTree*>(code));
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

// codetree* forcegraph_get_code(forcegraph* graph)
// {
// 	return (reinterpret_cast<codetree*>(reinterpret_cast<ForceGraph*>(graph)->data));
//}

//int forcegraph_get_type
//const char* forcegraph_get_code
//const char* forcegraph_get_tag

void forcegraph_print(forcegraph* graph)
{
	(reinterpret_cast<ForceGraph*>(graph))->print();	
}



