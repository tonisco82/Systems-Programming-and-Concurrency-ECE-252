
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>
#include <getopt.h>
#include "catpng.h"
#include "main_write_header_cb.h"

#define IMG_URL "http://ece252-1.uwaterloo.ca:2520/image?img="
#define ECE252_HEADER "X-Ece252-Fragment: "
#define BUF_SIZE 1048576
#define BUF_INC 524288

/*Thread arguments*/
struct thread_args
{
	int* retrieved;
	int* num_retrieved;
	char* url;
};

/*function passed to pthread to grab a segment*/
void *get_segment(void *arg) {
	struct thread_args *p_in = arg;
	/*setup CURL*/
	CURL *curl_handle;
	CURLcode res;
	RECV_BUF recv_buf;
	recv_buf_init(&recv_buf, BUF_SIZE);

	curl_handle = curl_easy_init();
	if (curl_handle == NULL) {
		recv_buf_cleanup(&recv_buf);
		curl_easy_cleanup(curl_handle);
		return NULL;
	}
	curl_easy_setopt(curl_handle, CURLOPT_URL, p_in->url);
	/*callback function to process received data*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_cb_curl3);
	/*set recv buffer*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&recv_buf);
	/*callback to process received header data*/
	curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_cb_curl);
	/*set recv buffer*/
	curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void*)&recv_buf);

	/*some servers may require a user-agent field*/
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	size_t size = 0;
	while (*(p_in->num_retrieved) < 50) {
		res = curl_easy_perform(curl_handle);
		if (res != CURLE_OK) {
			recv_buf_cleanup(&recv_buf);
			curl_easy_cleanup(curl_handle);
			return NULL;
		}
		if (!p_in->retrieved[recv_buf.seq]) {
			p_in->retrieved[recv_buf.seq] = 1;
			*(p_in->num_retrieved) = *(p_in->num_retrieved) + 1;

			char fname[20];
			sprintf(fname, "./output_%d.png", recv_buf.seq);

			write_file(fname, recv_buf.buf + size, recv_buf.size - size);
		}
		size = recv_buf.size;
	}

	recv_buf_cleanup(&recv_buf);
	curl_easy_cleanup(curl_handle);

	return NULL;
}

int main(int argc, char** argv) {
	int c;
	int no_threads = 1;
	int img_no = 1;

	/*check arguments*/
	while ((c = getopt(argc, argv, "t:n:")) != -1) {
		switch(c) {
		case 't':
			no_threads = strtoul(optarg, NULL, 10);
			break;
		case 'n':
			img_no = strtoul(optarg, NULL, 10);
			break;
		default:
			break;
		}
	}

        curl_global_init(CURL_GLOBAL_DEFAULT);

	/*map of retrieved images*/
	int retrieved[50];
	memset(retrieved, 0, 50*sizeof(int));
	int num_retrieved = 0;
        char url[256];
	sprintf(url, "%s%d", IMG_URL, img_no);

	/*setup threads*/
	pthread_t *p_tids = malloc(sizeof(pthread_t) * no_threads);
	struct thread_args *p_in = malloc(sizeof(struct thread_args));
	p_in->retrieved = retrieved;
	p_in->num_retrieved = &num_retrieved;
	p_in->url = url;

	for(int i = 0; i < no_threads; i++) {
		pthread_create(p_tids + i, NULL, get_segment, p_in);
	}

	for(int i = 0; i < no_threads; i++) {
		pthread_join(p_tids[i], NULL);
	}

	char* filenames[51];
	filenames[0] = malloc(20*sizeof(char));
	sprintf(filenames[0], "first");
	for(int i = 1; i < 51; i++) {
		filenames[i] = malloc(20*sizeof(char));
		sprintf(filenames[i], "output_%d.png", i - 1);
	}

	int catres = catpng(51, filenames);
	if(catres != 0) {
		for (int i = 0; i < 51; ++i) {
			remove(filenames[i]);
			free(filenames[i]);
		}
		free(p_in);
		free(p_tids);
		curl_global_cleanup();
		return -1;
	}

        for(int i = 0; i < 51; i++) {
		remove(filenames[i]);
		free(filenames[i]);
	}
	free(p_in);
	free(p_tids);

	curl_global_cleanup();

	return 0;
}

/*ALGORITHM:
	Retrieve all segments using cURL
	Output them to files
	Concatenate them using catpng method
*/

/*Threads Behaviour:
	Parameters: retrieved, num_retrieved, url
	no return value
	while num_retrieved < 50 grab more stuff
*/
