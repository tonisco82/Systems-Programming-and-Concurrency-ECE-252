#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT "2520"
#define PLANS_FILE "deathstarplans.dat"

typedef struct {
	char *data;
	int length;
} buffer;

extern int errno;

/* This function loads the file of the Death Star
 plans so that they can be transmitted to the
 awaiting Rebel Fleet. It takes no arguments, but
 returns a buffer structure with the data. It is the
 responsibility of the caller to deallocate the
 data element inside that structure.
 */
buffer load_plans();

int sendall(int socket, char *buf, int *len);

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Usage: %s IP-Address\n", argv[0]);
		return -1;
	}
	printf("Planning to connect to %s.\n", argv[1]);

	buffer buf = load_plans();

	struct addrinfo hints;
	struct addrinfo *res;

	/*Start of Code*/

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(argv[1], PORT, &hints, &res);
	if (result != 0) {
		printf("getaddrinfo (errno = %s)\n", strerror(errno));
		return -1;
	}

	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == -1) {
                printf("socket (errno = %s)\n", strerror(errno));
                return -1;
        }

	int status = connect(sockfd, res->ai_addr, res->ai_addrlen);
	if (status != 0) {
                printf("connect (errno = %s)\n", strerror(errno));
		return -1;
	}

	int testsend  = sendall(sockfd, buf.data, &buf.length);
        if (testsend != 0) {
                printf("sendall (errno = %s)\n", strerror(errno));
                return -1;
        }

	buffer message;
	memset(&message, 0, sizeof(message));
	message.data = malloc(65);

	int numbytes = recv(sockfd, message.data, 65, 0);
	if (numbytes == -1) {
                printf("recv: error (errno = %s)\n", strerror(errno));
		return -1;
	}
	else if (numbytes == 0) {
		printf("recv: closed (errno = %s)\n", strerror(errno));
		return -1;
	}

	printf("%s\n", message.data);

	free(message.data);
	free(buf.data);
	close(sockfd);
	freeaddrinfo(res);

	/*End of Code*/

	return 0;
}

buffer load_plans() {
	struct stat st;
	stat( PLANS_FILE, &st);
	ssize_t filesize = st.st_size;
	char *plansdata = malloc(filesize);
	int fd = open( PLANS_FILE, O_RDONLY);
	memset(plansdata, 0, filesize);
	read(fd, plansdata, filesize);
	close(fd);

	buffer buf;
	buf.data = plansdata;
	buf.length = filesize;

	return buf;
}

int sendall(int socket, char *buf, int *len) {
	int total = 0;
	int bytesleft = *len;
	int n;

	while(total < *len) {
		n = send(socket, buf + total, bytesleft, 0);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
	*len = total;
	return n == -1 ? -1 : 0;
}

/* REFERENCES: ECE252 Lecture Slides 7-8 were referenced for code,
 specifically for functions getaddrinfo(), socket(), connect(),
 send(), and recv(). The implementation of these functions would
 be the same in any instance, and variable names were also kept
 the same as they were informative in the writing of this code.
*/