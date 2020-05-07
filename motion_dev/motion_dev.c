#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO19 19

MODULE_LICENSE("GPL");

int motion_read(struct file *filp,char * buf,size_t count, loff_t *f_pos){
	int value = gpio_get_value(GPIO19);
	copy_to_user(buf,&value,sizeof(int));
	return count;
}
static struct file_operations mo_fops = {
	.read = motion_read
};

int __init motion_init(void){
	printk(KERN_INFO "MOTION INIT\n");
	printk(KERN_INFO "major : %d",register_chrdev(511, "motion_dev",&mo_fops));
	
	gpio_request(GPIO19,"GPIO19");
	gpio_direction_input(GPIO19);
	printk(KERN_INFO "Motion INIT DONE\n");
	return 0;
}

void __exit motion_exit(void){
	gpio_free(GPIO19);
	unregister_chrdev(511,"motion_dev");
	printk(KERN_INFO "Motion EXIT");
}
module_init(motion_init);
module_exit(motion_exit);
