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
#include <pthread.h>
#include <semaphore.h>
#include <search.h>
#include <fcntl.h>
#include "curl_xml.h"
#include "findpng2.h"

#define CT_PNG "image/png"
#define CT_HTML "text/html"
#define CT_PNG_LEN 9
#define CT_HTML_LEN 9
#define URL_LENGTH 256

int logging = 0;

/*work to be performed by threads*/
void* work(void* arg) {
	thread_args *p_in = arg;
	ENTRY e, *ep;
	CURL *curl_handle;
	CURLcode res;
	/*will use a boolean flag for the while loop*/
	int contWork = 1;

	while (contWork) {

		pthread_mutex_lock(p_in->mut_pngs);
		if (*p_in->pngs_collected >= p_in->target) {
			pthread_mutex_unlock(p_in->mut_pngs);
			break;
		}
		pthread_mutex_unlock(p_in->mut_pngs);

		pthread_mutex_lock(p_in->mut_frontier);

		/*check if empty frontier first*/
		if (p_in->fhead == NULL) {
			(*(p_in->blocked_threads))++;
			if (*p_in->blocked_threads < p_in->num_threads) {
				/*wait and unblock frontier*/
				pthread_cond_wait(p_in->sig_frontier, p_in->mut_frontier);
			}
			/*now if the last thread to be blocked and nothing left in frontier*/
			if (*p_in->blocked_threads >= p_in->num_threads && p_in->fhead == NULL) {
				pthread_cond_broadcast(p_in->sig_frontier);
				pthread_mutex_unlock(p_in->mut_frontier);
				break;
			}
			/*decrement blocked threads counter when leaving this area*/
			(*(p_in->blocked_threads))--;

		}

		if (p_in->fhead == NULL) {
			/*this is an error case, try again*/
			pthread_mutex_unlock(p_in->mut_frontier);
			continue;
		}
		/*pop the next element in frontier*/
		frontier_node* popped = p_in->fhead;
		p_in->fhead = p_in->fhead->next;
		/*maintain linked list consistent state and help to terminate loop when nothing left*/
		if (p_in->fhead == NULL) p_in->ftail = NULL;

		pthread_mutex_unlock(p_in->mut_frontier);

		/*save value of phead, to be popped*/
                e.key = popped->url;
                e.data = NULL;

		//Search VISITED hash table
		pthread_rwlock_wrlock(p_in->rw_hash);
		hsearch_r(e, FIND, &ep, p_in->visited);

		/*if already in visited, move forward to next URL in frontier*/
                if (ep != NULL) {       //represents successful search
			pthread_rwlock_unlock(p_in->rw_hash);
                        free(e.key);
                        e.key = NULL;
                        continue;
                }
		/*Add popped URL to VISITED: hsearch with ENTER flag enters the element since its not already there*/
		hsearch_r(e, ENTER, &ep, p_in->visited);
		pthread_rwlock_unlock(p_in->rw_hash);

		/*print the URL to log file*/
		if (logging) {
			pthread_mutex_lock(p_in->mut_log);

			FILE *fp = fopen(p_in->logfile, "a");
			if (fp != NULL) {
				fwrite(ep->key, strlen(ep->key), 1, fp);
				fwrite("\n", 1, 1, fp);
			}
			fclose(fp);
			pthread_mutex_unlock(p_in->mut_log);
		}

		/*CURL the popped URL*/
        	RECV_BUF recv_buf;
		curl_handle = easy_handle_init(&recv_buf, ep->key);

		if (curl_handle == NULL) {
			abort();
		}
		res = curl_easy_perform(curl_handle);
		if (res != CURLE_OK) {
			//printf("curl_easy_perform() failed: %s \n", curl_easy_strerror(res));
			cleanup(curl_handle, &recv_buf);
			/*keep trying*/
			continue;
		}
		/*data processing handled externally (process_data => html/png)*/
		process_data(curl_handle, &recv_buf, arg);

		cleanup(curl_handle, &recv_buf);

		pthread_mutex_lock(p_in->mut_pngs);
                if (*p_in->pngs_collected >= p_in->target) {

			pthread_mutex_unlock(p_in->mut_pngs);
                        break;
                }
                pthread_mutex_unlock(p_in->mut_pngs);

	}

	/*Once while finished exit other thread, since PNG limit reached*/
	pthread_mutex_lock(p_in->mut_frontier);
	*p_in->blocked_threads = p_in->num_threads;
	pthread_cond_broadcast(p_in->sig_frontier);
	pthread_mutex_unlock(p_in->mut_frontier);

	return NULL;
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
	int no_threads = 1;
	int num_urls = 50;
	char logfile[64];
	memset(logfile, 0, 64);
	int c;
	/*argv[argc - 1] is the seed URL*/

	while ((c = getopt(argc, argv, "t:m:v:")) != -1) {
		switch(c) {
		case 't':
			no_threads = strtoul(optarg, NULL, 10);
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
	fhead->url = malloc(URL_LENGTH * sizeof(char));
	memset(fhead->url, 0, URL_LENGTH * sizeof(char));
	/*set the head of the frontier to be the seed URL*/
	//memcpy(fhead->url, argv[argc-1], strlen(argv[argc-1]) * sizeof(char));
	strcpy(fhead->url, argv[argc-1]);
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
	int blocked_threads = 0;

	/*Concurrency Controls*/
	pthread_cond_t sig_frontier;
	pthread_mutex_t mut_frontier;
	pthread_mutex_t mut_pngs;
	pthread_rwlock_t rw_hash;
	pthread_mutex_t mut_log;
	pthread_mutex_t mut_xml;
	if (pthread_cond_init(&sig_frontier, NULL) ||
		pthread_mutex_init(&mut_frontier, NULL) ||
		pthread_mutex_init(&mut_pngs, NULL) ||
		pthread_rwlock_init(&rw_hash, NULL) ||
		pthread_mutex_init(&mut_log, NULL) ||
		pthread_mutex_init(&mut_xml, NULL)) {
			perror("concurrency controls");
			abort();
	}

	/*Multi-thread arguments*/
	thread_args *p_in = malloc(sizeof(thread_args));
	p_in->fhead = fhead;
	p_in->ftail = ftail;
	p_in->phead = phead;
	p_in->visited = visited;
	p_in->pngs_collected = &pngs_collected;
	p_in->blocked_threads = &blocked_threads;
	p_in->target = num_urls;
	p_in->num_threads = no_threads;
	p_in->logfile = logfile;
	p_in->sig_frontier = &sig_frontier;
	p_in->mut_frontier = &mut_frontier;
	p_in->mut_pngs = &mut_pngs;
	p_in->rw_hash = &rw_hash;
	p_in->mut_log = &mut_log;
	p_in->mut_xml = &mut_xml;

	/*curl init*/
	curl_global_init(CURL_GLOBAL_DEFAULT);
	xmlInitParser();

	/*thread init*/
	pthread_t* threads = malloc(no_threads * sizeof(pthread_t));
	/*remove old logfile.txt if it exists*/
	remove(logfile);

	/*thread create*/
	for (int i = 0; i < no_threads; ++i) {
	        pthread_create(threads + i, NULL, work, p_in);
	}
	for (int i = 0; i < no_threads; ++i) {
		pthread_join(threads[i], NULL);
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
	xmlCleanupParser();

	/*destroy frontier linked list*/

	frontier_node* fstepper = p_in->fhead;
	while (fstepper != NULL) {
		frontier_node* tmp = fstepper;
		fstepper = fstepper->next;
		if (tmp->url != NULL) free(tmp->url);
		free(tmp);
	}

	/*destroy png linked list*/

	png_node* pstepper = p_in->phead;
	while (pstepper != NULL) {
		png_node* tmp = pstepper;
		pstepper = pstepper->next;
		if (tmp->url != NULL) free(tmp->url);
		free(tmp);
	}

	/*cleanup concurrency controls*/
	pthread_cond_destroy(&sig_frontier);
	pthread_mutex_destroy(&mut_frontier);
	pthread_mutex_destroy(&mut_pngs);
	pthread_rwlock_destroy(&rw_hash);
	pthread_mutex_destroy(&mut_log);

	/*cleanup thread*/
	free(p_in);
	free(threads);

	/*clean up visited hash table*/
	hdestroy_r(visited);
	free(visited);

	return 0;
}
