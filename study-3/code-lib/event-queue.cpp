#include "event-queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

eventqueue* eventqueue_create()
{
	return reinterpret_cast<eventqueue*>(new Queue<Event*>());
}

void eventqueue_destroy(eventqueue* eq)
{
	delete reinterpret_cast<Queue<Event*>*>(eq);
}

event* event_create(int time, char* address, char* type, const char* data)
{
	int bytes = 0;
	for(int i = 1; i < strlen(type); i++)
	{
		// printf("data %s with length %zu\n", &data[bytes], strlen(&data[bytes]));
		if(type[i] == 'i') bytes += sizeof(int);
		else if(type[i] == 's') bytes += strlen(&data[bytes]) + 1;
	}

	// printf("allocated %d\n", bytes);

	Event* e = new Event;
	e->time = time;
	e->address = std::string(address);
	e->type = std::string(type);

	e->data = (char*) malloc(bytes + 1);
	memcpy(e->data, data, bytes);

	return reinterpret_cast<event*>(e);
}

void event_destroy(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	free(ev->data);
	delete ev;
}


void eventqueue_push(eventqueue* eq, event* e)
{
	Queue<Event*>* queue = reinterpret_cast<Queue<Event*>*>(eq);
	queue->push(reinterpret_cast<Event*>(e));
}

event* eventqueue_pop(eventqueue* eq)
{
	Queue<Event*>* queue = reinterpret_cast<Queue<Event*>*>(eq);
	return reinterpret_cast<event*>(queue->pop());
}

int eventqueue_size(eventqueue* eq)
{
	Queue<Event*>* queue = reinterpret_cast<Queue<Event*>*>(eq);
	// printf("got size %d in c method\n", queue->size());
	return queue->size();
}

int event_get_time(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->time;
}

const char* event_get_address(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->address.c_str();
}

const char* event_get_type(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->type.c_str();
}

char* event_get_data(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->data;
}

