/********************************************************
 **  Authors: Andrea Bisacchi, andrea.bisacchi5@studio.unibo.it
 **           Carlo Caini, carlo.caini@unibo.it
 **
 **
 **  Copyright (c) 2013, Alma Mater Studiorum, University of Bologna
 **  All rights reserved.
 ********************************************************/

/*
 * list.c
 */

#include "list.h"
#include <stdlib.h>
#include <string.h>

/*
 * Returns a duplicate of list.
 */
List list_duplicate(List list)
{
	List new_list = empty_list;
	while (list != empty_list)
	{
		list_push_back(&new_list, list->data, list->data_size);
		list = list->next;
	}
	return new_list;
}

/*
 * Destroy the List and dealloc all nodes
 */
void list_destroy(List* list)
{
	List next;
	if (list == NULL) return;
	if (*list == empty_list) return;
	do {
		if ((*list)->data != NULL)
			free((*list)->data);
		next = (*list)->next;
		free((*list));
		*list = next;
	} while (next != empty_list);
}

/*
 * Inserts in the list a COPY of the element passed in the position index.
 * Returns the pointer to the new data allocated
 */
void* list_insert_index(List* list, void* data, size_t data_size, int index)
{
	Node* new_node;
	if (index <= 0) return NULL;
	if (list == NULL) return NULL;
	if (data == NULL) return NULL;
	if (*list == empty_list && index != 1) return NULL;

	if (index == 1)
	{ // have to insert in front
		new_node = (Node*) malloc(sizeof(Node));
		new_node->next = *list;
		*list = new_node;
	}
	else
	{ // have to insert in the middle (or in the end)
		List current = *list;
		while (index > 2 && current->next != empty_list) // will stop in the previous position of the insertion
		{
			current = current->next;
			index--;
		}
		if (index == 2)
		{ // insert in the middle (or in the end)
			new_node = (Node*) malloc(sizeof(Node));
			new_node->next = current->next;
			current->next = new_node;
		}
		else
		{ // list is not long enough
			return NULL;
		}
	}
	new_node->data_size = data_size;
	new_node->data = malloc(data_size);
	memcpy(new_node->data, data, data_size); //copy the data

	return new_node->data;
}

/*
 * Inserts in the list a COPY of the element passed.
 * Returns the pointer to the new data allocated
 */
inline void* list_push_front(List* list, void* data, size_t data_size)
{
	return list_insert_index(list, data, data_size, 1);
}

/*
 * Inserts in the list a COPY of the element passed. The element will be added in ordered way according to compare function.
 * Returns the pointer to the new data allocated
 */
void* list_push_ordered(List *list, void* data, size_t data_size, int (*compare)(void*,size_t,void*,size_t))
{
	int index = 1;
	List cursor;

	if (compare == NULL) compare = &default_compare;
	if (list == NULL) return NULL;
	cursor = *list;

	if (*list == empty_list) // List is empty -> need to create an element
	{
		return list_push_front(list, data, data_size);
	}

	while ( cursor != empty_list && compare(data, data_size, cursor->data, cursor->data_size) > 0 )
	{
		index++;
		cursor = cursor->next;
	}

	return list_insert_index(list, data, data_size, index);
}

/*
 * Inserts in the end of the list a COPY of the element passed.
 * Returns the pointer to the new data allocated
 */
inline void* list_push_back(List* list, void* data, size_t data_size)
{
	return (list == NULL ? NULL : list_insert_index(list, data, data_size, list_length(*list) + 1));
}

/*
 * Inserts in the end of the list a COPY of the element passed.
 * Returns the pointer to the new data allocated
 */
inline void* list_append(List* list, void* data, size_t data_size)
{
	return list_push_back(list, data, data_size);
}

/*
 * Returns a COPY of the element in position index according to list_length value.
 * You should FREE the resource!
 */
void* list_get_value_index(List list, int index, size_t* data_size)
{
	void* ret_val;
	if (index <= 0) return NULL;
	while (index > 1 && list->next != empty_list)
	{
		list = list->next;
		index--;
	}
	if (index != 1) return NULL; // list too short
	if (data_size != NULL)
		*data_size = list->data_size;
	ret_val = malloc(list->data_size);
	memcpy(ret_val, list->data, list->data_size);
	return ret_val;
}

/*
 * Returns the pointer and the data_size of the element in position index according to list_length value.
 */
void* list_get_pointer_index(List list, int index, size_t* data_size)
{
	if (index <= 0) return NULL;
	while (index > 1 && list->next != empty_list)
	{
		list = list->next;
		index--;
	}
	if (index != 1) return NULL; // list too short
	if (data_size != NULL)
		*data_size = list->data_size;
	return list->data;
}

/*
 * Returns the length of the list. Starts from 1.
 */
int list_length(List list)
{
	int counter = 1;
	if (list == empty_list) return 0;
	while (list->next != empty_list)
	{
		list = list->next;
		counter++;
	}
	return counter;
}

/*
 * Removes and returns the pointer and the data_size of the element in position index according to list_length value.
 * You should FREE the resource!
 */
void* list_remove_index_get_pointer(List* list, int index, size_t* data_size)
{
	List current;
	List previous = NULL;
	void* temp;
	if (list == NULL) return NULL;
	if (*list == empty_list) return NULL;
	if (index <= 0) return NULL;
	current = *list;
	while (index > 1 && current->next != empty_list)
	{
		previous = current;
		current = current->next;
		index--;
	}
	if (index != 1) return NULL; // list too short
	if (previous != NULL)
	{ // not first element
		previous->next = current->next;
	}
	else
	{ // first element
		*list = (*list)->next;
	}
	temp = current->data;
	if (data_size != NULL)
		*data_size = current->data_size;
	free(current);
	return temp;
}

/*
 * Removes and returns the pointer and the data_size of the first element.
 * You should FREE the resource!
 */
inline void* list_pop_front(List* list, size_t* data_size)
{
	return list_remove_index_get_pointer(list, 1, data_size);
}

/*
 * Removes and returns the pointer and the data_size of the last element.
 * You should FREE the resource!
 */
inline void* list_pop_back(List* list, size_t* data_size)
{
	return (list == NULL ? NULL : list_remove_index_get_pointer(list, list_length(*list), data_size));
}

/*
 * Removes from the list the element in position index according to list_length value.
 */
void list_remove_index(List* list, int index)
{
	void* temp;
	temp = list_remove_index_get_pointer(list, index, NULL);
	free(temp);
}

/*
 * Removes from the list the first element.
 */
inline void list_remove_first(List* list)
{
	list_remove_index(list, 1);
}

/*
 * Removes from the list the last element.
 */
inline void list_remove_last(List* list)
{
	list_remove_index(list, list_length(*list));
}

/*
 * Removes from the list the element equals to the passed element. The way it will compare the elements is obteined by compare param.
 * Compare is according to the standard compare functions. compare(a,b) --> Negative:a<b, Zero:a=b, Positive: a>b
 * If compare param not passed (NULL) it will use default_compare.
 * OUTPUT: true if removed, false if not.
 */
bool list_remove_data(List* list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t))
{
	int index;
	if (list == NULL) return false;
	index = list_find(*list, data_to_search, data_to_search_size, compare);
	if (index <= 0)
	{
		return false;
	}
	else
	{
		list_remove_index(list, index);
		return true;
	}
}

/*
 * Removes from the list the elements which the function isToRemove is true is obteined by compare param.
 * OUTPUT: TRUE if removed at least one element, FALSE if not.
 */
bool list_remove_if(List* list, bool (*isToRemove)(void*,size_t)) {
	bool result = false;
	List current;
	List next;
	int indexToRemove;

	if (list == NULL) return false;
	if (*list == empty_list) return false;

	current = *list;
	indexToRemove = 1;

	while (current != empty_list) {
		next = current->next;
		if (isToRemove(current->data, current->data_size)) { // Remove
			list_remove_index(list, indexToRemove);
			result = true;
		} else {
			indexToRemove++;
		}
		current = next;
	}

	return result;
}

/*
 * Executes the function (param function) for each list element.
 * DO NOT FREE the pointer passed to your function.
 */
void list_for_each(List list, void (*function)(void*,size_t)) {
	if (function == NULL) return;
	for (; list != empty_list; list = list->next)
	{
		function(list->data, list->data_size);
	}
}

/*
 * Returns the index of the element passed. The way it will compare the elements is obteined by compare param.
 * Compare is according to the standard compare functions. compare(a,b) --> Negative:a<b, Zero:a=b, Positive: a>b
 * If compare param not passed (NULL) it will use default_compare.
 */
int list_find(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t))
{
	int index = 1;
	if (data_to_search == NULL) return -1;
	if (data_to_search_size < 0) return -1;
	if (compare == NULL) compare = &default_compare;
	while(list != empty_list)
	{
		if (compare(data_to_search, data_to_search_size, list->data, list->data_size) == 0)
			return index;
		list = list->next;
		index++;
	}
	return -1;
}

/*
 * Returns the pointer to the first data which compare returns 0.
 */
void* list_get_pointer_data(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t))
{
	int index = -1;

	if (data_to_search == NULL) return NULL;
	if (data_to_search_size < 0) return NULL;
	if (compare == NULL) compare = &default_compare;

	index = list_find(list, data_to_search, data_to_search_size, compare);

	if (index >= 1)
	{ // found
		return list_get_pointer_index(list, index, NULL);
	}
	else
	{ // not found
		return NULL;
	}
}

/*
 * Returns a pointer to a copy to the first data which compare returns 0.
 * You should FREE the resource!
 */
void* list_get_value_data(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t))
{
	int index = -1;

	if (data_to_search == NULL) return NULL;
	if (data_to_search_size < 0) return NULL;
	if (compare == NULL) compare = &default_compare;

	index = list_find(list, data_to_search, data_to_search_size, compare);

	if (index >= 1)
	{ // found
		return list_get_value_index(list, index, NULL);
	}
	else
	{ // not found
		return NULL;
	}
}

/***************************************************
 *               DEFAULT FUNCTIONS                 *
 ***************************************************/

/*
 * Compare the two data.
 * If data_size are equals 	--> compare bit per bit according to memcmp
 * If not 					--> returns (data1_size - data2_size)
 */
int default_compare(void* data1, size_t data1_size, void* data2, size_t data2_size)
{
	if (data1 == NULL) return -1;
	if (data2 == NULL) return 1;
	if (data1==data2 && data1_size==data2_size) return 0;
	return (data1_size != data2_size) ? (data1_size - data2_size) : memcmp(data1, data2, data1_size);
}

/***************************************************
 *               PRIVATE FUNCTIONS                 *
 ***************************************************/

