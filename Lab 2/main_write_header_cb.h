#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>

typedef struct recv_buf2 {
	char *buf;		/*memory to hold copy of received data*/
	size_t size; 		/*size of valid data in buf in bytes*/
	size_t max_size;	/*max capacity of buf in bytes*/
	int seq;		/* >=0 sequence number extracted from http header*/
				/* <0 indicates an invalid seq number*/
} RECV_BUF;

size_t header_cb_curl(char *p_recv, size_t size, size_t nmemb, void *userdata);
size_t write_cb_curl3(char *p_recv, size_t size, size_t nmemb, void *p_userdata);
int recv_buf_init(RECV_BUF *ptr, size_t max_size);
int recv_buf_cleanup(RECV_BUF *ptr);
int write_file(const char *path, const void *in, size_t len);
