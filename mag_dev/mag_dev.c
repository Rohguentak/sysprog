#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO22 5

MODULE_LICENSE("GPL");

int mag_read(struct file *filp,char * buf,size_t count, loff_t *f_pos){
	int value = gpio_get_value(GPIO22);
	copy_to_user(buf,&value,sizeof(int));
	return count;
}
static struct file_operations mag_fops = {
	.read = mag_read
};

int __init mag_init(void){
	printk(KERN_INFO "MAG INIT\n");
	register_chrdev(243, "mag_dev",&mag_fops);
	gpio_request(GPIO22,"GPIO22");
	gpio_direction_input(GPIO22);
	printk(KERN_INFO "MAG INIT DONE\n");
	return 0;
}

void __exit mag_exit(void){
	gpio_free(GPIO22);
	unregister_chrdev(243,"mag_dev");
	printk(KERN_INFO "MAG EXIT");
}
module_init(mag_init);
module_exit(mag_exit);
