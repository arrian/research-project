#include "event-queue.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
  eventqueue* q = eventqueue_create();

  int n = 123;
  char* c = (char*) malloc(20);
  sprintf(c, "%d%s", n, "test");
  event* e = event_create(100, "test address", ",is", c);

  eventqueue_push(q, e);
  event* e2 = eventqueue_pop(q);
  printf("%d\n", event_get_time(e2));
  return 0;
}