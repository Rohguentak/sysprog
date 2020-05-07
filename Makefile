

connect :
	sudo insmod inf/inf_dev.ko 
	sudo mknod -m 666 /dev/inf_dev c 237 0	
	sudo insmod fire/fire_dev.ko
	sudo mknod -m 666 /dev/fire_dev c 241 0
	sudo insmod servo1/servo1_mod.ko
	sudo mknod -m 666 /dev/servo1_dev c 240 0
	sudo insmod servo2/servo2_mod.ko
	sudo mknod -m 666 /dev/servo2_dev c 239 0
	sudo insmod buzzor/buzzer_mod.ko
	sudo mknod -m 666 /dev/buzzer_dev c 238 0
	sudo insmod gasdev/gasdev.ko
	sudo mknod -m 666 /dev/gasdev c 242 0
	sudo insmod mag_dev/mag_dev.ko
	sudo mknod -m 666 /dev/mag_dev c 243 0	
	sudo insmod motion_dev/motion_dev.ko
	sudo mknod -m 666 /dev/motion_dev c 511 0


