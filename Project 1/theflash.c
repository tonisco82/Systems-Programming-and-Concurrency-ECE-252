#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

#define CITIES_LENGTH 7
#define NUM_CITIES (CITIES_LENGTH - 1)

static const char* cities[] = { "Central City", "Starling City", "Gotham City", "Metropolis", "Coast City", "National City" };

const int distances[CITIES_LENGTH - 1][ CITIES_LENGTH - 1] = {
    {0, 793, 802, 254, 616, 918},
    {793, 0, 197, 313, 802, 500},
    {802, 197, 0, 496, 227, 198},
    {254, 313, 496, 0, 121, 110},
    {616, 802, 227, 121, 0, 127},
    {918, 500, 198, 110, 127, 0}
};

int initial_vector[CITIES_LENGTH] = { 0, 1, 2, 3, 4, 5, 0 };

typedef struct {
    int cities[CITIES_LENGTH];
    int total_dist;
} route;

void print_route ( route* r ) {
    printf ("Route: ");
    for ( int i = 0; i < CITIES_LENGTH; i++ ) {
        if ( i == CITIES_LENGTH - 1 ) {
            printf( "%s\n", cities[r->cities[i]] );
        } else {
            printf( "%s - ", cities[r->cities[i]] );
        }
    }
}

void calculate_distance( route* r ) {
    if ( r->cities[0] != 0 ) {
        printf( "Route must start with %s (but was %s)!\n", cities[0], cities[r->cities[0]]);
        exit( -1 );
    }
    if ( r->cities[6] != 0 ) {
        printf( "Route must end with %s (but was %s)!\n", cities[0], cities[r->cities[6]]);
        exit ( -2 );
    }
    int distance = 0;
    for ( int i = 1; i < CITIES_LENGTH; i++ ) {
        int to_add = distances[r->cities[i-1]][r->cities[i]];
        if ( to_add == 0 ) {
            printf( "Route cannot have a zero distance segment.\n");
            exit ( -3 );
        }
        distance += to_add;
    }
    r->total_dist = distance;
}

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void permute(route *r, int left, int right, route *best) {

	if (left == right) {

		pid_t pid;
		int childStatus;

		/*Begin fork into parent and child*/
		pid = fork();

		if (pid < 0) {
			fprintf(stderr, "Fork_Failed");
			return;
		} else if (pid == 0) {

			/*Child function*/
			calculate_distance(r);

			/*Create new file and write all information on current route for parent*/
			FILE *f = fopen("file.txt", "w+");
			fprintf(f, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", r->total_dist,
					r->cities[0], r->cities[1], r->cities[2], r->cities[3],
					r->cities[4], r->cities[5]);
			fclose(f);

			exit(0);

		} else {
			wait(&childStatus);
		}

		/*Create/initialize new route r2 to collect information from file*/
		route *r2 = malloc(sizeof(route));
		memcpy(r2->cities, initial_vector, CITIES_LENGTH * sizeof(int));
		r2->total_dist = 0;

		/*Parent reads the file into r2*/
		FILE *f2 = fopen("file.txt", "r");
		if ((fscanf(f2, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &r2->total_dist,
								&r2->cities[0], &r2->cities[1], &r2->cities[2], &r2->cities[3],
								&r2->cities[4], &r2->cities[5])) != 7) {
			fprintf(stderr, "Scan_Failed");
			return;
		}
		fclose(f2);

		/*Comparing output to the best so far*/
		if (r2->total_dist < best->total_dist) {
			memcpy(best, r2, sizeof(route));
		}

		free(r2);

		return;
	}

	for (int i = left; i <= right; i++) {
		swap(&r->cities[left], &r->cities[i]);
		permute(r, left + 1, right, best);
		swap(&r->cities[left], &r->cities[i]);
	}
}

int main(int argc, char **argv) {

	route *candidate = malloc(sizeof(route));
	memcpy(candidate->cities, initial_vector, CITIES_LENGTH * sizeof(int));
	candidate->total_dist = 0;

	route *best = malloc(sizeof(route));
	memset(best, 0, sizeof(route));
	best->total_dist = 999999;

	permute(candidate, 1, 5, best);

	print_route(best);
	printf("Distance: %d\n", best->total_dist);

	free(candidate);
	free(best);

	return 0;
}
