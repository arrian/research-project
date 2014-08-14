
#include "list.h"

#include <vector>


list* list_create()
{
	return reinterpret_cast<list*>(new std::vector<void*>());
}

void list_destroy(list* l)
{
	delete reinterpret_cast<std::vector<void*>*>(l);
}


void list_add(list* l, void* data)
{
	reinterpret_cast<std::vector<void*>*>(l)->push_back(data);	
}

void* list_get(list* l, int index)
{
	return reinterpret_cast<std::vector<void*>*>(l)->at(index);
}

void list_remove(list* l, int index)
{
	std::vector<void*>* vec = reinterpret_cast<std::vector<void*>*>(l);
	vec->erase(vec->begin() + index);
}

void list_remove_end(list* l, int count)
{
	std::vector<void*>* vec = reinterpret_cast<std::vector<void*>*>(l);
	vec->erase(vec->end() - count, vec->end());
}


int list_size(list* l)
{
	return reinterpret_cast<std::vector<void*>*>(l)->size();
}


