#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h> //delay header

#define A2 23

#define LOW	0
#define HIGH	1

MODULE_LICENSE("GPL");

#define IOCTL_MAGIC_NUMBER 'H'

#define IOCTL_SERVO_ON		_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_SERVO_OFF		_IO( IOCTL_MAGIC_NUMBER, 1)
long servo_ioctl (struct file *filp, unsigned int cmd, unsigned long arg){
	

	switch(cmd){
		case IOCTL_SERVO_ON :
			gpio_set_value(A2, HIGH);
			printk(KERN_INFO "SERVO : TURNNING ON SERVO\n");
			break;
		case IOCTL_SERVO_OFF:
			gpio_set_value(A2, LOW);
			printk(KERN_INFO "SERVO : TURNNING OFF SERVO\n");
			break;
	}
	return 0;
}

struct file_operations fops = {
	.unlocked_ioctl = servo_ioctl
};

static int __init servo_init(void){
	printk(KERN_INFO "SERVO : Starting ---\n");

	register_chrdev(240, "servo_dev", &fops);

	gpio_request(A2,"A2");
	gpio_direction_output(A2,LOW);

	printk(KERN_INFO "SERVO : Starting Complete\n");

	return 0;
}

static void __exit servo_exit(void){
	gpio_free(A2);

	unregister_chrdev(240, "servo_dev");

	printk(KERN_INFO "servo : Exit Done");
}


module_init(servo_init);
module_exit(servo_exit);
