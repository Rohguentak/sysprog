#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define	HIGH	1
#define	LOW		0

int main (void) {
	int value=0;
	int dist_fd;
	dist_fd = open("/dev/inf_dev", O_RDWR);
	printf("%d\n",dist_fd);
	if(dist_fd < 0){
		printf("fail to open device node\n");
		return 0;
	}

	while (1){
		
		read(dist_fd,&value,4);
		printf("%d\n",value);
		value=0;
	}
	return 0;
}

