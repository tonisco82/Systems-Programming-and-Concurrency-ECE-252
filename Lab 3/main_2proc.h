#pragma once

typedef struct recv_buf_flat {
	char *buf;
	size_t size;
	size_t max_size;
	int seq;
} RECV_BUF;

size_t header_cb_curl(char *p_recv, size_t size, size_t nmemb, void *userdata);
size_t write_cb_curl(char *p_recv, size_t size, size_t nmemb, void *p_userdata);
int shm_recv_buf_init(RECV_BUF *ptr, size_t nbytes);
int shm_recv_buf_cleanup(RECV_BUF *ptr);
int write_file(const char *path, const void *in, size_t len);
int sizeof_shm_recv_buf(size_t nbytes);
