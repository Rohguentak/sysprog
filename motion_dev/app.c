#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(void){
	int motion_fd = open("/dev/motion_dev",O_RDONLY);
	if(motion_fd<0){
		printf("open error\n");
		return 1;
	}
	int motion_state = 2;
	while(1){

		read(motion_fd,&motion_state,sizeof(int));
		if(motion_state == 0){
			printf("stop\n");
		}
		else if(motion_state ==1){
			printf("moving\n");
		}
		sleep(0.85);
	}
	return 0;
}
