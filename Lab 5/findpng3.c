#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <curl/curl.h>
#include <search.h>
#include <fcntl.h>
#include "curl_xml.h"
#include "findpng3.h"

#define CT_PNG "image/png"
#define CT_HTML "text/html"
#define CT_PNG_LEN 9
#define CT_HTML_LEN 9
#define URL_LENGTH 256
#define MAX_WAIT_MSECS 30*1000

int logging = 0;
int curlm_init( CURLM *connections, char* url );

/*work to be performed*/
int work(void* arg) {
	work_args *p_in = arg;
	ENTRY e, *ep;
	CURL *curl_handle;
	int contWork = 1;
	int still_running = 0;
	int msgs_left = 0;

	CURLM *connections = curl_multi_init();
	curl_multi_setopt( connections, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)p_in->max_connections );
	CURLMsg *msg = NULL;

	while (contWork) {

		if (*p_in->pngs_collected >= p_in->target) break;
		/*check if empty frontier first*/
		if (p_in->fhead == NULL) {
			/*temporary measure*/
			break;
		}

		int loaded_connections = 0;
		/*add up to max_connections to the multi_handle here*/
		while (p_in->fhead != NULL && loaded_connections < p_in->max_connections) {
			/*pop the next element in frontier*/
			frontier_node* popped = p_in->fhead;
		 	p_in->fhead = p_in->fhead->next;
			if (p_in->fhead == NULL) p_in->ftail = NULL;
                	/*maintain linked list consistent state and help to terminate loop when nothing left*/

			/*save value of fhead, to be popped*/
			e.key = popped->url;
			e.data = NULL;
			free(popped);

			//Search VISITED hash table
			hsearch_r(e, FIND, &ep, p_in->visited);

			/*if already in visited, move forward to next URL in frontier*/
			if (ep != NULL) {	//represents successful search
				free(e.key);
				e.key = NULL;
				continue;
			}

			/*Add popped URL to VISITED: ENTER flag enters the element since its not already there*/
			hsearch_r(e, ENTER, &ep, p_in->visited);

			/*print the URL to log file*/
			if (logging) {
				FILE *fp = fopen(p_in->logfile, "a");
				if (fp != NULL) {
					fwrite(e.key, strlen(ep->key), 1, fp);
					fwrite("\n", 1, 1, fp);
				}
				fclose(fp);
			}

			if ( curlm_init( connections, ep->key ) ) {
				return -2;
			}
			loaded_connections++;
		}

		/*now dispatch the connections*/
		curl_multi_perform( connections, &still_running );
		do {
			int numfds = 0;
			int res = curl_multi_wait( connections, NULL, 0, MAX_WAIT_MSECS, &numfds );
			if ( res != CURLM_OK ) {
				abort();
			}
			curl_multi_perform( connections, &still_running );
		} while( still_running );

		while ( ( msg = curl_multi_info_read( connections, &msgs_left ) ) ) {
			if ( msg->msg == CURLMSG_DONE ) {
				/*retrieve the data*/
				curl_handle = msg->easy_handle;

				/*int http_status = 0;
				const char* szUrl;*/
				RECV_BUF *recv_buf;

				/*curl_easy_getinfo( curl_handle, CURLINFO_RESPONSE_CODE, &http_status );
				curl_easy_getinfo( curl_handle, CURLINFO_EFFECTIVE_URL, &szUrl );
				if ( http_status == 200 ) {
					printf("200 OK for %s\n", szUrl );
				}*/

				if ( msg->data.result == CURLE_OK ) {
					curl_easy_getinfo( curl_handle, CURLINFO_PRIVATE, &recv_buf );
					process_data( curl_handle, recv_buf, arg );
				}

				curl_multi_remove_handle( connections, curl_handle );
				curl_easy_cleanup( curl_handle );
			}
		}
	}

	curl_multi_cleanup(connections);
	return 0;
}

int main(int argc, char** argv) {

	if (argc < 2) {
		return -1;
	}

	double times[2];
	struct timeval tv;

	/*record time before program execution*/
	if (gettimeofday(&tv, NULL) != 0) {
		perror("gettimeofday");
		abort();
	}
	times[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	/*arguments*/
	int max_connections = 1;
	int num_urls = 50;
	char logfile[64];
	memset(logfile, 0, 64);
	int c;
	/*argv[argc - 1] is the seed URL*/

	while ((c = getopt(argc, argv, "t:m:v:")) != -1) {
		switch(c) {
		case 't':
			max_connections = strtoul(optarg, NULL, 10);
			break;
		case 'm':
			num_urls = strtoul(optarg, NULL, 10);
			break;
		case 'v':
			logging = 1;
			sprintf(logfile, optarg, sizeof(optarg));
			break;
		default:
			break;
		}
	}

	/*init URL frontier*/
	frontier_node* fhead = malloc(sizeof(frontier_node));
	fhead->url = calloc(1, URL_LENGTH * sizeof(char));
	/*set the head of the frontier to be the seed URL*/
	strcpy(fhead->url, argv[argc - 1]);
	fhead->next = NULL;
	frontier_node* ftail = fhead;
	/*init glib hash table for visited URLS*/
	struct hsearch_data *visited = calloc(1, sizeof(struct hsearch_data));
	if (hcreate_r(40 * num_urls, visited) == 0) {
		return -2;
	}
	/*init PNG result list*/
	png_node* phead = NULL;
	int pngs_collected = 0;

	work_args *p_in = malloc(sizeof(work_args));
	p_in->fhead = fhead;
	p_in->ftail = ftail;
	p_in->phead = phead;
	p_in->visited = visited;
	p_in->pngs_collected = &pngs_collected;
	p_in->target = num_urls;
	p_in->max_connections = max_connections;
	p_in->logfile = logfile;

	/*curl init*/
	curl_global_init(CURL_GLOBAL_ALL);
	xmlInitParser();
	/*remove old logfile if it exists*/
	remove(logfile);

	/*WORK IS DONE HERE*/
	if (work(p_in) != 0) {
		/*cleanup if failed*/
		free(p_in);
		hdestroy_r(visited);
		free(visited);
		return -1;
	}

	/*Print PNG URLs to png.urls.txt, this will create an empty file even if nothing to print*/
	FILE* fp_pngs;
	fp_pngs = fopen("png_urls.txt", "w");
	png_node* png_stepper = p_in->phead;
	while (png_stepper != NULL) {
		fprintf(fp_pngs, "%s\n", png_stepper->url);
		png_stepper = png_stepper->next;
	}
	fclose(fp_pngs);

	/*record time after program execution is finished*/
	if (gettimeofday(&tv, NULL) != 0) {
		perror("gettimeofday");
		abort();
	}
	times[1] = (tv.tv_sec) + tv.tv_usec/1000000.;
	printf("findpng2 execution time: %.6lf seconds\n", times[1] - times[0]);

	/*CLEANUP*/
	curl_global_cleanup();

	/*destroy frontier linked list*/
	frontier_node* fstepper = p_in->fhead;
	while (fstepper != NULL) {
		frontier_node* tmp = fstepper;
		fstepper = fstepper->next;
		free(tmp->url);
		free(tmp);
	}

	/*destroy png linked list*/
	png_node* pstepper = p_in->phead;
	while (pstepper != NULL) {
		png_node* tmp = pstepper;
		pstepper = pstepper->next;
		free(tmp->url);
		free(tmp);
	}

	/*cleanup work arguments*/
	free(p_in);

	/*clean up visited hash table*/
	hdestroy_r(visited);
	free(visited);

	return 0;
}

int curlm_init( CURLM* connections, char* url ) {
	/*this memory should be freed afterward*/
	RECV_BUF *recv_buf = malloc( sizeof( RECV_BUF ) );
	CURL *curl_handle = easy_handle_init( recv_buf, url );
	if ( curl_handle == NULL ) {
//		free(recv_buf);
		return -1;
	}
	curl_easy_setopt( curl_handle, CURLOPT_PRIVATE, recv_buf );
	curl_multi_add_handle( connections, curl_handle );
//	free(recv_buf);
	return 0;
}
