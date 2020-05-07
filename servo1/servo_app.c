#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'K'

#define IOCTL_SERVO_ON		_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_SERVO_OFF		_IO( IOCTL_MAGIC_NUMBER, 1)


int servo_rotate(int fd, int angle);

int main(int argc, char **argv){
	int fd = open("/dev/servo1_dev", O_RDWR);

	if(argc !=2){
		printf("usage : servo1_app [0|1]\n");
		exit(-1);
	}
	
	servo_rotate(fd,atoi(argv[1]));
	
	close(fd);

	return 0;
}


int servo_rotate(int fd, int angle){
	
        double	high_period_ms = 0;

	int low_period_us = 0;

	if(angle >0 && angle<=90){
		high_period_ms = 1.5 + 1*(angle/90.0);
		printf("high_period = %f\n",high_period_ms);
	}
	else if(angle == 0){
		high_period_ms = 1.5;
		printf("high_period = %f\n",high_period_ms);
	}
	else if(angle >=-90 && angle < 0){
		high_period_ms = 1.5 - 1*((-angle)/90.0);
		printf("high_period = %f\n",high_period_ms);
	}
	else{// cannot support this angle
		return -1;
	}

	low_period_us = 20000 - (int)(high_period_ms*1000);

	for(int i=0;i<10;i++){
		ioctl(fd, IOCTL_SERVO_ON, 0);
		usleep(20000 - low_period_us);
		ioctl(fd, IOCTL_SERVO_OFF, 0);
		usleep(low_period_us);
	}

	return 0;
}

