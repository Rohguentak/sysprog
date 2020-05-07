#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'M'

#define IOCTL_BUZZER_ON		_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_BUZZER_OFF	_IO( IOCTL_MAGIC_NUMBER, 1)

int main(int argc, char **argv){
	int fd = open("/dev/buzzer_dev", O_RDWR);

	if(argc !=2){
		printf("usage : buzzer_app [0|1]\n");
		exit(-1);
	}

	if(!strcmp(argv[1], "1")){
		printf("Turning on BUZZER\n");
		for(int i=0;i<500;i++){
			ioctl(fd, IOCTL_BUZZER_ON,0);
			usleep(2500);
			ioctl(fd, IOCTL_BUZZER_OFF,0);
			usleep(2500);
		}
		usleep(2500);
	}
	else if(!strcmp(argv[1],"0")){
		printf("Turning off BUZER\n");
		ioctl(fd,IOCTL_BUZZER_OFF,0);
	}
	else{
		printf("usage : buzzer_app [0|1]\n");
	}
	close(fd);

	return 0;
}
