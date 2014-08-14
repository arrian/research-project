#pragma once

extern "C"
{
	struct list;

	list* list_create();
	void list_destroy(list* l);

	void list_add(list* l, void* data);
	void* list_get(list* l, int index);
	void list_remove(list* l, int index);
	void list_remove_end(list* l, int count);

	int list_size(list* l);
}