#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h> //delay header

#define GPIO20 20

#define LOW	0
#define HIGH	1

MODULE_LICENSE("GPL");

#define IOCTL_MAGIC_NUMBER 'L'

#define IOCTL_SERVO_ON		_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_SERVO_OFF		_IO( IOCTL_MAGIC_NUMBER, 1)
long servo_ioctl (struct file *filp, unsigned int cmd, unsigned long arg){
	

	switch(cmd){
		case IOCTL_SERVO_ON :
			gpio_set_value(GPIO20, HIGH);
			printk(KERN_INFO "SERVO : TURNNING ON SERVO\n");
			break;
		case IOCTL_SERVO_OFF:
			gpio_set_value(GPIO20, LOW);
			printk(KERN_INFO "SERVO : TURNNING OFF SERVO\n");
			break;
	}
	return 0;
}

struct file_operations fops = {
	.unlocked_ioctl = servo_ioctl
};

static int __init servo_init(void){
	printk(KERN_INFO "SERVO2 : Starting ---\n");

	register_chrdev(239, "servo2_dev", &fops);

	gpio_request(GPIO20,"GPIO20");
	gpio_direction_output(GPIO20,LOW);

	printk(KERN_INFO "SERVO2 : Starting Complete\n");

	return 0;
}

static void __exit servo_exit(void){
	gpio_free(GPIO20);

	unregister_chrdev(239, "servo2_dev");

	printk(KERN_INFO "servo2 : Exit Done");
}


module_init(servo_init);
module_exit(servo_exit);
