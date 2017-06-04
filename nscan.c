#include <stdio.h>
#include <ctype.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>

struct timeval tempo1, tempo2;
long elapsed_mtime;
long elapsed_seconds;
long elapsed_useconds;
int addr_byte_0;
int addr_byte_1;
int scanned;
int alive;
int inactive;

const char* getname(const char *ip) 
{
	struct in_addr addr;
	struct hostent *he;

	inet_aton(ip, &addr);
	if (he = gethostbyaddr(&addr, sizeof(addr), AF_INET)) {
		++alive;
		return he->h_name;
	}
	else {
		++inactive;
		return "None";
	}
}

int scanner(int s, int y) 
{
	int i;
    char addr[200];

    for (i = 0; i < 255; ++i) 
    {
        sprintf(addr, "%d.%d.%d.%d", addr_byte_0, addr_byte_1, s, i);
		const char *name = getname(addr);
		++scanned;
		printf("[\033[1;34m•\033[00m] Subnet: %d\r", s);

		if (name != "None" && i < 255) 
		{
			if (y == 0)
			{
				printf("[\033[1;32m•\033[00m] %s is at %s\n", name, addr);
			} 
			else 
			{
				printf("[\033[1;32m•\033[00m] %s\n", addr);
			}
		}
    }

	return 0;
}

void usage()
{
	printf("Usage: nscan <address-range> [--subnet] [--subnet-range]\n\n");
	printf("Author: Dylan Halls <dylan.halls123@gmail.com>\n\n");
	printf("- Nscan will perform a silent scan of the network to find active nodes\n");
	printf("- Address range should be formed with last two bytes stared e.g. 192.168.*.* or 10.0.*.*\n\n");
	printf("--subnet  -  subnet wished to be scanned\n");
	printf("--subnet-range  -  the range of subnets wished to be scanned e.g. 78-98\n");
	printf("--ip-only  -  this will only display the ip when found\n");
	printf("--help  -  display this message and exit\n");
	exit(1);
}

void end()
{
	gettimeofday(&tempo2, NULL);
	elapsed_seconds = tempo2.tv_sec - tempo1.tv_sec; 
	elapsed_useconds = tempo2.tv_usec - tempo1.tv_usec;
	elapsed_mtime = ((elapsed_seconds) * 1000 + elapsed_useconds/1000.0) + 0.5;

	printf("\n--- Nscan statistics ---\n");
	printf("%d nodes scanned, %d alive nodes, %d inactive nodes\n", scanned, alive, inactive);
	printf("time scanning %ldms\n", elapsed_mtime);
}

int main(int argc, char *argv[]) 
{

	printf("\033[1;3m%s\033[00m\n", "Nscan v1\n");
	gettimeofday(&tempo1, NULL);
    
    char addr[20];
    char *token;
	char *splitone = ".";
	const char *secarg;
	int max;
	int min;
	int e;
	int y;

	if (argc == 1) {
		usage();
	}

	if (argc == 2) {
		secarg = "AAAAAAAAAAAAAAAAA";
	} else {
		secarg = argv[2];
	}

	if (strncmp(argv[1], "--help", 6) == 0) {
		usage();
	}

	for (int i = 0; i < argc; ++i) {
		if (strncmp(argv[i], "--ip-only", 9)) {
			y = 0;
		} else {
			y = 1;
		}
	}

	token = strtok(argv[1], splitone);
	addr_byte_0 = atol(token);

	token = strtok(NULL, splitone);
	addr_byte_1 = atol(token);

	const char *subarg = "--subnet";
	char subrangearg[20] = "--subnet-range";
	char *hypher = "-";

	if (strcmp(subarg, secarg) == 0)
	{
			if (atoi(argv[3])-1 == -1)
			{
				printf("[\033[1;31m•\033[00m] %s%s\n", "Invalid subnet: ", argv[3]);
				exit(-1);
			}
			scanner(atoi(argv[3]), y);
			end();
			exit(1);
	}
	else if (strcmp(subrangearg, secarg) == 0)
	{
		char *n1 = strtok(argv[3], hypher);
		char *n2 = strtok(NULL, hypher);

		if (atoi(n1)-1 == -1)
			{
				printf("[\033[1;31m•\033[00m] %s%s\n", "Invalid subnet: ", n1);
				exit(-1);
			}

		if (atoi(n2)-1 == -1)
			{
				printf("[\033[1;31m•\033[00m] %s%s\n", "Invalid subnet: ", n2);
				exit(-1);
			}

		int num1 = atoi(n1);
		int num2 = atoi(n2);

		if (num1 > num2)
		{
			max = num1+1;
			min = num2;
		}
		else if (num2 > num1)
		{
			max = num2+1;
			min = num1;
		}

		if (max >= 256) {
			printf("[\033[1;31m•\033[00m] Invalid range: %d-%d\n", min, max);
			exit(-1);
		}

		for (e = min; e < max; ++e)
		{
			scanner(e, y);
		}
		end();
		exit(1);
	}

	printf("[\033[1;34m•\033[00m] %s\n", "Starting Scan");
	printf("[\033[1;33m•\033[00m] %s\n", "Scanning 255 subnets (this might take a while)");
	for (int i = 0; i < 255; ++i) {
		scanner(i, y);
	}
	end();
}