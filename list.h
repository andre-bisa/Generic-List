/********************************************************
 **  Authors: Andrea Bisacchi, andrea.bisacchi5@studio.unibo.it
 **           Carlo Caini (DTNperf_3 project supervisor), carlo.caini@unibo.it
 **
 **
 **  Copyright (c) 2013, Alma Mater Studiorum, University of Bologna
 **  All rights reserved.
 ********************************************************/

/*
 * list.h
 */

#ifndef DTNPERF_SRC_AL_BP_UTILITY_LIST_H_
#define DTNPERF_SRC_AL_BP_UTILITY_LIST_H_

#include <features.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/file.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct Node
{
	void* data;
	size_t data_size;

	struct Node* next;
} Node;

typedef Node* List;

#define empty_list NULL

List list_duplicate(List list);

void list_destroy(List* list);

void list_insert_index(List* list, void* data, size_t data_size, int index);

void list_push_front(List* list, void* data, size_t data_size);

void list_push_back(List* list, void* data, size_t data_size);

void list_append(List* list, void* data, size_t data_size);

void* list_get_value_index(List list, int index, size_t* data_size);

void* list_get_pointer_index(List list, int index, size_t* data_size);

int list_length(List list);

void* list_remove_index_get_pointer(List* list, int index, size_t* data_size);

void* list_pop_front(List* list, size_t* data_size);

void* list_pop_back(List* list, size_t* data_size);

void list_remove_index(List* list, int index);

void list_remove_first(List* list);

void list_remove_last(List* list);

bool list_remove_data(List* list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t));

int list_find(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t));

void* list_get_pointer_data(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t));

void* list_get_value_data(List list, void* data_to_search, size_t data_to_search_size, int (*compare)(void*,size_t,void*,size_t));


/***************************************************
 *               DEFAULT FUNCTIONS                 *
 ***************************************************/

int default_compare(void* data1, size_t data1_size, void* data2, size_t data2_size);

#endif /* DTNPERF_SRC_AL_BP_UTILITY_LIST_H_ */
