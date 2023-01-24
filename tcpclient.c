#include "common.h"
//localhost: 127.0.0.1

int main(int argc, char **argv){
	int sockfd, n;
	int sendbytes;
	SA_IN servaddr;
	char sendline[BUFSIZE];
	char recvline[BUFSIZE];

	if (argc != 2){
		err_n_die("usage: %s <server address>", argv[0]);
	}
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		err_n_die("Error while creating the socket!");
	}
	

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
		err_n_die("inet_pton error for %s ", argv[1]);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
		err_n_die("Connect failed!");
	}

	sprintf(sendline, "/home/emil/testfiles/test.txt\n");
	sendbytes = strlen(sendline);

	if (write(sockfd, sendline, sendbytes) != sendbytes){
		err_n_die("Write error");
	}

	clock_t begin = clock();

	memset(recvline, 0, BUFSIZE);
	printf("\n");
	while ( (n = read(sockfd, recvline, BUFSIZE-1)) > 0){
		printf("%s\n", recvline);
		memset(recvline, 0, BUFSIZE);
	}
	if (n < 0){
		err_n_die("Read error");
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Fetch time: %lf \n", time_spent);

	exit(0);
}
