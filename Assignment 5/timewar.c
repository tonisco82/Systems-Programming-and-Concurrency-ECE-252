#define _GNU_SOURCE
#define HASH_BUFFER_LENGTH 32

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>

void read_data( char* destination, unsigned int generator_seed );
void process( int* input );
int* calculate( char* input_buffer );

const int num_doctors = 13;
pthread_t doctors[13];

pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_barrier_t barrier;

int* coordinates;
int ready;

void* first_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Calling the war council of Gallifrey...\n" );

    char * input = malloc( 33 );
    read_data( input, 252 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[0] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* second_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "This is the Doctor!\n" );

    char * input = malloc( 33 );
    read_data( input, 477 );
    int* c = calculate( input );

        pthread_mutex_lock( &lock2 );
        coordinates[0] += *c;
        pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

        pthread_mutex_lock( &lock1 );
        ready++;
        pthread_mutex_unlock( &lock1 );

}

void* third_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Ready!\n" );

    char * input = malloc( 33 );
    read_data( input, 482 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[0] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* fourth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Commencing Calculations...\n" );

    char * input = malloc( 33 );
    read_data( input, 839 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[0] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* fifth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Nearly there?\n" );

    char * input = malloc( 33 );
    read_data( input, 178 );
    int* c = calculate( input );

        pthread_mutex_lock( &lock2 );
        coordinates[1] += *c;
        pthread_mutex_unlock( &lock2 );

        free(c);
	free(input);

        pthread_mutex_lock( &lock1 );
        ready++;
        pthread_mutex_unlock( &lock1 );

}

void* sixth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Just about!\n" );

    char * input = malloc( 33 );
    read_data( input, 970 );
    int* c = calculate( input );

        pthread_mutex_lock( &lock2 );
        coordinates[1] += *c;
        pthread_mutex_unlock( &lock2 );

        free(c);
	free(input);

        pthread_mutex_lock( &lock1 );
        ready++;
        pthread_mutex_unlock( &lock1 );

}

void* seventh_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Cross the boundaries that divide one universe from another...\n" );

    char * input = malloc( 33 );
    read_data( input, 235 );
    int* c = calculate( input );

        pthread_mutex_lock( &lock2 );
        coordinates[1] += *c;
        pthread_mutex_unlock( &lock2 );

        free(c);
	free(input);

        pthread_mutex_lock( &lock1 );
        ready++;
        pthread_mutex_unlock( &lock1 );

}

void* eighth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Lock on to these coordinates!\n" );

    char * input = malloc( 33 );
    read_data( input, 919 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[1] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* war_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Oh for gods' sake... Gallifrey stands!\n" );

    char * input = malloc( 33 );
    read_data( input, 93 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[2] += *c;
	pthread_mutex_unlock( &lock2 );

	free( c );
	free( input );

	int* count = malloc( sizeof( int ) );
	process( count );
	free(count);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* ninth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "And for my next trick...!\n" );

    char * input = malloc( 33 );
    read_data( input, 57 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[2] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* tenth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "Allons-y!\n" );

    char * input = malloc( 33 );
    read_data( input, 712 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[3] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* eleventh_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );
    printf( "GERONIMO!\n" );

    char * input = malloc( 33 );
    read_data( input, 272 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[3] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

void* twelfth_doctor( void* arg ) {
    pthread_barrier_wait( &barrier );

    char * input = malloc( 33 );
    read_data( input, 647 );
    int* c = calculate( input );

	pthread_mutex_lock( &lock2 );
	coordinates[3] += *c;
	pthread_mutex_unlock( &lock2 );

	free(c);
	free(input);

	pthread_mutex_lock( &lock1 );
	ready++;
	pthread_mutex_unlock( &lock1 );

}

/****** Do not change anything below this line -- Internal Functionality Only ********/
void gather_doctors();
void wait_until_done();
bool verify_coordinates();

int main( int argc, char** argv ) {
    ready = 0;
    coordinates = calloc( 4, sizeof( int ) );
    pthread_mutex_init( &lock1, NULL );
    pthread_mutex_init( &lock2, NULL );
    pthread_barrier_init( &barrier, NULL, num_doctors );
    gather_doctors();
    wait_until_done();
    int final_ready;
    pthread_mutex_lock( &lock1 );
    final_ready = ready;
    pthread_mutex_unlock( &lock1 );

    if (verify_coordinates() && final_ready == num_doctors ) {
        printf( "Gallifrey Stands!\n" );
    } else {
        printf( "Gallifrey falls...\n" );
    }

    free( coordinates );
    pthread_mutex_destroy( &lock1 );
    pthread_mutex_destroy( &lock2 );
    pthread_barrier_destroy( &barrier );
    return 0;
}

void gather_doctors( ) {
    pthread_create( &doctors[0], NULL, first_doctor, NULL );
    pthread_create( &doctors[1], NULL, second_doctor, NULL );
    pthread_create( &doctors[2], NULL, third_doctor, NULL );
    pthread_create( &doctors[3], NULL, fourth_doctor, NULL );
    pthread_create( &doctors[4], NULL, fifth_doctor, NULL );
    pthread_create( &doctors[5], NULL, sixth_doctor, NULL );
    pthread_create( &doctors[6], NULL, seventh_doctor, NULL );
    pthread_create( &doctors[7], NULL, eighth_doctor, NULL );
    pthread_create( &doctors[8], NULL, war_doctor, NULL );
    pthread_create( &doctors[9], NULL, ninth_doctor, NULL );
    pthread_create( &doctors[10], NULL, tenth_doctor, NULL );
    pthread_create( &doctors[11], NULL, eleventh_doctor, NULL );
    pthread_create( &doctors[12], NULL, twelfth_doctor, NULL );
}

void wait_until_done( ) {
    for ( int i = 0; i < num_doctors; ++i ) {
        pthread_join( doctors[i], NULL );
    }
}

/* Generate random string code based off original by Ates Goral */
void read_data(char* dest, unsigned int generator_seed) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    char* s = malloc( (HASH_BUFFER_LENGTH+1) * sizeof( char ) );
    for (int i = 0; i < HASH_BUFFER_LENGTH; ++i) {
        s[i] = alphanum[rand_r(&generator_seed) % (sizeof(alphanum) - 1)];
    }
    s[HASH_BUFFER_LENGTH] = 0;
    strcpy(dest, s);
    free( s );
}

int* calculate( char* input_buffer ) {
    int * res = malloc(sizeof(int));
    unsigned char* output_buffer = calloc( HASH_BUFFER_LENGTH , sizeof ( unsigned char ) );
    SHA256( input_buffer, HASH_BUFFER_LENGTH, output_buffer );
    pthread_t self = pthread_self( );
    for (int i = 0; i < num_doctors; ++i ) {
        if ( pthread_equal( self, doctors[i] ) ){
            *res = i;
        }
    }
    free( output_buffer );
    return res;
}

void process( int* input ) {
    for (int i = 0; i < 100000000; i++) {
        (*input)++;
    }
}

bool verify_coordinates() {
    pthread_mutex_lock( &lock2 );
    bool correct = 6 == coordinates[0] 
                && 22 == coordinates[1]
                && 17 == coordinates[2]
                && 33 == coordinates[3];
    pthread_mutex_unlock( &lock2 );
    return correct;
}
