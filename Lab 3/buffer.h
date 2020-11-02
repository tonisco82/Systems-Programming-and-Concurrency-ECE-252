#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "main_2proc.h"

typedef struct Buffer
{
	RECV_BUF* queue;
	int size;
	int max_size;
	int front;
	int rear;
} Buffer;

void Buffer_init(Buffer* b, int max_size, int recv_buf_size);
int Buffer_add(Buffer* b, RECV_BUF* node, int recv_buf_size);
int Buffer_pop(Buffer* b, RECV_BUF* node, int recv_buf_size);
void Buffer_clean(Buffer *b);
int sizeof_Buffer(int buffer_size, size_t recv_buf_size);



