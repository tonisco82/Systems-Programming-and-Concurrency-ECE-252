/*Custom Implementation of a fixed size queue to store recieve data from a CURL callback*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include "buffer.h"
#include "main_2proc.h"

void Buffer_init(Buffer* b, int max_size, int recv_buf_size) {
	/*address space should be allocated by user*/
	if (b == NULL) {
		return;
	}
	b->size = 0;
	b->max_size = max_size;
	b->front = -1;
	b->rear = -1;
	b->queue = (RECV_BUF*)((char*) b + sizeof(Buffer));
	/*init all queue slots*/
	for (int i = 0; i < max_size; ++i) {
		RECV_BUF* ptr = (RECV_BUF*) ((char*)b->queue + i * sizeof_shm_recv_buf(recv_buf_size));
		shm_recv_buf_init(ptr, recv_buf_size);
	}
}

int Buffer_add(Buffer* b, RECV_BUF* node, int recv_buf_size) {
	if (b == NULL || node == NULL) {
		return 1;
	}
	/*Buffer full?*/
	if (b->size >= b->max_size) {
		return 2;
	}
	else if (b->front == -1) {
		b->front = 0;
		b->rear = 0;
		/*copy everything except the pointer, which is stored at the start, cast to appropiate types*/
		memcpy((char*)b->queue + sizeof(char*),
			(char*)node + sizeof(char*),
			sizeof_shm_recv_buf(recv_buf_size) - sizeof(char*));
	}
	else if (b->rear == b->max_size - 1 && b->front != 0) {
		b->rear = 0;

		memcpy((char*)b->queue + sizeof(char*),
			(char*)node + sizeof(char*),
			sizeof_shm_recv_buf(recv_buf_size) - sizeof(char*));
	}
	else {
		b->rear++;

		memcpy((char*)b->queue + b->rear * sizeof_shm_recv_buf(recv_buf_size) + sizeof(char*),
			(char*)node + sizeof(char*),
			sizeof_shm_recv_buf(recv_buf_size) - sizeof(char*));
	}
	b->size++;
	return 0;
}

/*2nd argument is the node where data is to be copied*/
int Buffer_pop(Buffer* b, RECV_BUF* node, int recv_buf_size) {
	if (b == NULL) {
		return 1;
	}
	if (b->size == 0) {
	 	return 2;
	}

	memcpy((char*)node + sizeof(char*),
		(char*)b->queue + b->front * sizeof_shm_recv_buf(recv_buf_size) + sizeof(char*),
		sizeof_shm_recv_buf(recv_buf_size) - sizeof(char*));

	if (b->front == b->rear) {
		b->front = -1;
		b->rear = -1;
	} else if (b->front == b->max_size - 1) {
		b->front = 0;
	} else {
		b->front++;
	}
	b->size--;
	return 0;
}

void Buffer_clean(Buffer *b) {
	if (b->size == 0) return;
	for (int i = 0; i < b->max_size; ++i) {
		shm_recv_buf_cleanup(&b->queue[i]);
	}
	b->front = -1;
	b->rear = -1;
	b->size = 0;
}

int sizeof_Buffer(int buffer_size, size_t recv_buf_size) {
	return (sizeof(Buffer) + sizeof(char) * buffer_size * sizeof_shm_recv_buf(recv_buf_size));
}

/*
int main() {
	Buffer* myBuf = malloc(sizeof(Buffer));
	Buffer_init(myBuf, 5);
	printf("Buffer initial size: %d\n", myBuf->size);
	printf("Buffer max size: %d\n", myBuf->max_size);
	RECV_BUF* testBuf = malloc(sizeof(RECV_BUF));
	Buffer_add(myBuf, testBuf);
	printf("Buffer size: %d\n", myBuf->size);
	Buffer_pop(myBuf);
	printf("Buffer size: %d\n", myBuf->size);
	Buffer_clean(myBuf);
	free(myBuf);
	return 0;
}
*/

