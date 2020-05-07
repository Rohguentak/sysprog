#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>

int a[10];

MODULE_LICENSE("GPL");

static struct file_operations fops = {
};



int __init dummy_init(void) {
	printk(KERN_INFO "dummy : init1 \n");

	printk(KERN_INFO "major : %d \n",a[0] = register_chrdev(0, "dummy", &fops));
	printk(KERN_INFO "major : %d \n",a[1]=register_chrdev(0, "dummy1", &fops));
	printk(KERN_INFO "major : %d \n",a[2]=register_chrdev(0, "dummy2", &fops));
	printk(KERN_INFO "major : %d \n",a[3]=register_chrdev(0, "dummy3", &fops));
	printk(KERN_INFO "major : %d \n",a[4]=register_chrdev(0, "dummy4", &fops));
	printk(KERN_INFO "major : %d \n",a[5]=register_chrdev(0, "dummy5", &fops));
	printk(KERN_INFO "major : %d \n",a[6]=register_chrdev(0, "dummy6", &fops));
	printk(KERN_INFO "major : %d \n",a[7]=register_chrdev(0, "dummy7", &fops));
	printk(KERN_INFO "major : %d \n",a[8]=register_chrdev(0, "dummy8", &fops));
	printk(KERN_INFO "major : %d \n",a[9]=register_chrdev(0, "dummy9", &fops));	
	
	printk(KERN_INFO "dummy : init done\n");

	return 0;
}

void __exit dummy_exit(void){
	

	unregister_chrdev(a[0], "dummy");
	unregister_chrdev(a[1], "dummy1");
	unregister_chrdev(a[2], "dummy2");
	unregister_chrdev(a[3], "dummy3");
	unregister_chrdev(a[4], "dummy4");
	unregister_chrdev(a[5], "dummy5");
	unregister_chrdev(a[6], "dummy6");
	unregister_chrdev(a[7], "dummy7");
	unregister_chrdev(a[8], "dummy8");
	unregister_chrdev(a[9], "dummy9");

	printk(KERN_INFO "dummy : Exit");
}

module_init(dummy_init);
module_exit(dummy_exit);

