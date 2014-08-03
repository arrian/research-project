#pragma once

#include "queue.h"


struct Event
{
  int time;
  char* address;
  char* type;
  void* data;
};

extern "C" 
{
  struct eventqueue;
  struct event;

  eventqueue* eventqueue_create();
  void eventqueue_destroy(eventqueue* eq);

  event* event_create(int time, char* address, char* type, void* data);
  void event_destroy(event* e);

  void eventqueue_push(eventqueue* eq, event* e);
  event* eventqueue_pop(eventqueue* eq);
  int eventqueue_size(eventqueue* eq);

  int event_get_time(event* e);
  char* event_get_address(event* e);
  char* event_get_type(event* e);
  void* event_get_data(event* e);
}

