#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h> //delay header

#define A2 16

#define LOW	0
#define HIGH	1

MODULE_LICENSE("GPL");

#define IOCTL_MAGIC_NUMBER 'M'

#define IOCTL_BUZZER_ON		_IO( IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_BUZZER_OFF	_IO( IOCTL_MAGIC_NUMBER, 1)
long buzzer_ioctl (struct file *filp, unsigned int cmd, unsigned long arg){
	

	switch(cmd){
		case IOCTL_BUZZER_ON :
			gpio_set_value(A2, HIGH);
			printk(KERN_INFO "BUZZER : TURNNING ON BUZZER\n");
			break;
		case IOCTL_BUZZER_OFF:
			gpio_set_value(A2, LOW);
			printk(KERN_INFO "BUZZER : TURNNING OFF BUZZER\n");
			break;
	}
	return 0;
}

struct file_operations fops = {
	.unlocked_ioctl = buzzer_ioctl
};

static int __init buzzer_init(void){
	printk(KERN_INFO "BUZZER : Starting ---\n");

	register_chrdev(238, "buzzer_dev", &fops);

	gpio_request(A2,"A2");
	gpio_direction_output(A2,LOW);

	printk(KERN_INFO "BUZZER : Starting Complete\n");

	return 0;
}

static void __exit buzzer_exit(void){
	gpio_free(A2);

	unregister_chrdev(238, "buzzer_dev");

	printk(KERN_INFO "BUZZER : Exit Done");
}


module_init(buzzer_init);
module_exit(buzzer_exit);
