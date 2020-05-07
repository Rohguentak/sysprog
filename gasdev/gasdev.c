#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO26	26

MODULE_LICENSE("GPL");

int gas_read(struct file *filp,char * buf,size_t count,loff_t * f_pos){
	int value = gpio_get_value(GPIO26);
	copy_to_user(buf,&value,sizeof(int));
	return count;
}

static struct file_operations fops = {
	.read = gas_read
};

int __init gas_init(void){
	printk(KERN_INFO "GAS : INIT \n");
	register_chrdev(242,"gasdev",&fops);
	gpio_request(GPIO26,"GPIO26");
	gpio_direction_input(GPIO26);
	printk(KERN_INFO "GAS : INIT DONE\n");
	return 0;
}
void __exit gas_exit(void){
	gpio_free(GPIO26);
	unregister_chrdev(242,"gasdev");
	printk(KERN_INFO "GAS : EXIT");
}

module_init(gas_init);
module_exit(gas_exit);
