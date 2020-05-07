#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'H'

#define IOCTL_LED_ON	_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_LED_OFF	_IO( IOCTL_MAGIC_NUMBER, 1)
#define IOCTL_GET	_IO( IOCTL_MAGIC_NUMBER, 2)

int main(int argc, char **argv){
	int fd = open("/dev/led_dev", O_RDWR);

	if(argc !=2){
		printf("usage : led_app [0|1]\n");
		exit(-1);
	}

	if(!strcmp(argv[1], "1")){
		printf("Turning on LED\n");
		ioctl(fd, IOCTL_LED_ON,0);
	}
	else if(!strcmp(argv[1],"0")){
		printf("Turning off LED\n");
		ioctl(fd,IOCTL_LED_OFF,0);
	}
	else if(!strcmp(argv[1],"g")){
		printf("get : %d\n",ioctl(fd,IOCTL_GET,0));
	}
	else{
		printf("usage : led_app [0|1]\n");
	}
	close(fd);

	return 0;
}
