#include "event-queue.h"

eventqueue* eventqueue_create()
{
	return reinterpret_cast<eventqueue*>(new Queue<Event*>());
}

void eventqueue_destroy(eventqueue* eq)
{
	delete reinterpret_cast<Queue<Event*>*>(eq);
}

event* event_create(int time, char* address, char* type, void* data)
{
	Event* e = new Event;
	e->time = time;
	e->address = address;
	e->type = type;
	e->data = data;
	return reinterpret_cast<event*>(e);
}

void event_destroy(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
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
	printf("got size %d in c method\n", queue->size());
	return queue->size();
}

int event_get_time(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->time;
}

char* event_get_address(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->address;
}

char* event_get_type(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->type;
}

void* event_get_data(event* e)
{
	Event* ev = reinterpret_cast<Event*>(e);
	return ev->data;
}

