#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define	HIGH	1
#define	LOW		0

int main (void) {
	int value=0, fire_fd;
	
	fire_fd  = open("/dev/fire_dev", O_RDWR);
	printf("%d\n",fire_fd);
	if(fire_fd < 0){
		printf("fail to open device node\n");
		return 0;
	}

	while (1){
		read(fire_fd,&value,4);
		printf("%d\n",value);
		value=0;
	}
	return 0;
}

