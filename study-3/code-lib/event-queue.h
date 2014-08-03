#pragma once

#include <string>
#include "queue.h"


struct Event
{
  int time;
  std::string address;
  std::string type;
  char* data;
};

extern "C" 
{
  struct eventqueue;
  struct event;

  eventqueue* eventqueue_create();
  void eventqueue_destroy(eventqueue* eq);

  event* event_create(int time, char* address, char* type, const char* data);
  void event_destroy(event* e);

  void eventqueue_push(eventqueue* eq, event* e);
  event* eventqueue_pop(eventqueue* eq);
  int eventqueue_size(eventqueue* eq);

  int event_get_time(event* e);
  const char* event_get_address(event* e);
  const char* event_get_type(event* e);
  char* event_get_data(event* e);
}

