#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(void){
	int mag_fd = open("/dev/mag_dev",O_RDONLY);
	if(mag_fd<0){
		printf("open error\n");
		return 1;
	}
	int mag_state = 2;
	while(1){

		read(mag_fd,&mag_state,sizeof(int));
		if(mag_state == 0){
			printf("door is close\n");
		}
		else{
			printf("door is open\n");
		}
		sleep(1);
	}
	return 0;
}
