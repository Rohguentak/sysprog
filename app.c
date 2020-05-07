#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_SERVO1_MAGIC_NUMBER 'K'

#define IOCTL_SERVO1_ON		_IO( IOCTL_SERVO1_MAGIC_NUMBER, 0)
#define IOCTL_SERVO1_OFF	_IO( IOCTL_SERVO1_MAGIC_NUMBER, 1)


#define IOCTL_SERVO2_MAGIC_NUMBER 'L'

#define IOCTL_SERVO2_ON		_IO( IOCTL_SERVO2_MAGIC_NUMBER, 0)
#define IOCTL_SERVO2_OFF	_IO( IOCTL_SERVO2_MAGIC_NUMBER, 1)

#define IOCTL_BUZZER_MAGIC_NUMBER 'M'

#define IOCTL_BUZZER_ON		_IO( IOCTL_BUZZER_MAGIC_NUMBER, 0)
#define IOCTL_BUZZER_OFF	_IO( IOCTL_BUZZER_MAGIC_NUMBER, 1)

int state=0;
int a=0;
int count=0;

int servo_rotate(int fd, int angle,int servoid);

int buzzer_on(int fd, int second);

int inf1(int fd);
int inf2(int fd);
int fire_det(int fd);
int gas_det(int fd);
int mag_det(int fd);
int motion_det(int fd);

int main(int argc, char **argv){


	
	int servo1 = open("/dev/servo1_dev", O_RDWR);
	int servo2 = open("/dev/servo2_dev", O_RDWR);
	int buzzer = open("/dev/buzzer_dev", O_RDWR);
	int fire_fd = open("/dev/fire_dev", O_RDWR);
	int dist_fd = open("/dev/inf_dev", O_RDWR);
	
	
	int gas_fd = open("/dev/gasdev",O_RDONLY);
	int motion_fd = open("/dev/motion_dev",O_RDONLY);
	int mag_fd = open("/dev/mag_dev",O_RDONLY);
	
	int in,out;
	/*while(1){
		if(gas_det(gas_fd) == 0){
			printf("gas level is high\n");
		}
		if(mag_det(mag_fd) == 0){
			printf("door is close\n");
		}
		if(motion_det(motion_fd)==1){
			printf("moving\n");
		}else{
			printf("stop\n");
		}
		sleep(1);
	}*/

	if(argc <2){
		printf("usage : servo_app [0|1]\n");
		exit(-1);
	}

	if(!strcmp(argv[1],"s1")){
		servo_rotate(servo1,atoi(argv[2]),1);
	}
	else if(!strcmp(argv[1],"s2")){
		servo_rotate(servo2,atoi(argv[2]),2);
	}
	else if(!strcmp(argv[1],"buz")){
		buzzer_on(buzzer,atoi(argv[2]));
	}
	else if(!strcmp(argv[1],"inf")){
		while(1)
		{
			in=inf1(dist_fd); out=inf2(dist_fd); printf("in=%d, out=%d\t",in,out);
			a=(in>360)*2 + (out>360);
			if(state==0){if(a==1) state=1; else if(a==2) state=2;
			}
			else if(state==1){if(a==2) state=3; else if(a==0) state=0;
			}
			else if(state==2){if(a==1) state=4; else if(a==0) state=0;
			}
			else if(state==3){if(a==1) state=1; else if(a==0){ state=0; count++;}
			}
			else if(state==4){if(a==2) state=2; else if(a==0){ state=0; count--;}
			}
			printf("count=%d\n",count);
		}
	}
	else if(!strcmp(argv[1],"fire")){
		for(int k=0; k<20; k++) printf("%d\n",fire_det(fire_fd));
	}
	else if(!strcmp(argv[1],"mot")){
		while(1){
			printf("motion = %d\n",motion_det(motion_fd));
		}
	}
	else if(!strcmp(argv[1],"mag")){
		while(1){
			printf("mag = %d\n",mag_det(mag_fd));
		}
	}
	close(servo1);
	close(servo2);
	close(fire_fd);
	close(dist_fd);
	
	return 0;
}


int servo_rotate(int fd, int angle, int servoid){
	
        double	high_period_ms = 0;

	int low_period_us = 0;

	if(angle >0 && angle<=90){
		high_period_ms = 1.5 + 1*(angle/90.0);
		//printf("high_period = %f\n",high_period_ms);
	}
	else if(angle == 0){
		high_period_ms = 1.5;
		//printf("high_period = %f\n",high_period_ms);
	}
	else if(angle >=-90 && angle < 0){
		high_period_ms = 1.5 - 1*((-angle)/90.0);
		//printf("high_period = %f\n",high_period_ms);
	}
	else{// cannot support this angle
		return -1;
	}

	low_period_us = 20000 - (int)(high_period_ms*1000);

	if(servoid == 1){
		printf("s1\n");
		for(int i=0;i<10;i++){
			ioctl(fd, IOCTL_SERVO1_ON, 0);
			usleep(20000 - low_period_us);
			ioctl(fd, IOCTL_SERVO1_OFF, 0);
			usleep(low_period_us);
		}
	}
	else if( servoid == 2){
		printf("s2\n");
		for(int i=0;i<10;i++){
			ioctl(fd, IOCTL_SERVO2_ON, 0);
			usleep(20000 - low_period_us);
			ioctl(fd, IOCTL_SERVO2_OFF, 0);
			usleep(low_period_us);
		}
	}
	else {
		printf("error\n");
	}

	return 0;
}

int buzzer_on(int fd, int second){
	int i=0;

	double delay = ((double)1000000/933);
	double maxsignum = (double)((second*1000000)/(delay*2));
	
	for(i=0; i<maxsignum;i++){
		ioctl(fd, IOCTL_BUZZER_ON,0);
		usleep(2500);
		ioctl(fd, IOCTL_BUZZER_OFF,0);
		usleep(2500);
	}
	ioctl(fd, IOCTL_BUZZER_OFF);
	usleep(2000);

	return 0;
}

int inf1(int fd){
	int value=0;
	write(fd,&value,4);
	read(fd,&value,4);
	return value;
}

int inf2(int fd){
	int value=1;
	write(fd,&value,4);
	read(fd,&value,4);
	return value;
}
int fire_det(int fd){
	int value=0;
	read(fd,&value,4);
	return value;
}
int gas_det(int fd){
	int value =0;
	read(fd,&value,sizeof(int));
	return value;
}
int mag_det(int fd){
	int value = 0;
	read(fd,&value,sizeof(int));
	return value;
}
int motion_det(int fd){
	int value =0;
	read(fd,&value,sizeof(int));
	return value;
}
