#pragma once

#include <stdio.h>
#include <stdlib.h>

/*nodes in frontier linked list*/
typedef struct dummy1
{
	char* url;
	struct dummy1* next;
} frontier_node;

/*PNG Node*/
typedef struct dummy2
{
	char* url;
	struct dummy2* next;
} png_node;

typedef struct dummmy3
{
	frontier_node* fhead;
	frontier_node* ftail;
	png_node* phead;
	struct hsearch_data* visited;
	int* pngs_collected;
	int target;
	int max_connections;
	char* logfile;
} work_args;
