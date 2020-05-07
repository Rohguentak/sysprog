#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void){
	int fd = open("/dev/gasdev",O_RDONLY);
	int status = 1;
	if(fd<0){
		printf("error to open\n");
	}
	while(1){
		read(fd,&status,sizeof(int));
		if(status == 0){
			printf("gas level is high\n");
		}
		else{
			printf("gas level is low\n");
		}
		sleep(1);
	}
	return 0;
}
