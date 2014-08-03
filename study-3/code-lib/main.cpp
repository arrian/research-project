#include "event-queue.h"

int main(int argc, char* argv[])
{
  eventqueue* q = eventqueue_create();
  event* e = event_create(100, "test address", "test type", NULL);
  eventqueue_push(q, e);
  event* e2 = eventqueue_pop(q);
  printf("%d\n", event_get_time(e2));
  return 0;
}