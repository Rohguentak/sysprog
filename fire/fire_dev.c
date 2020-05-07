#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>


#define	CS		18
#define	DIN		17
#define	DOUT	27
#define CLK		22

#define	HIGH	1
#define	LOW		0

#define	DELAY	300


MODULE_LICENSE("GPL");

int channel = 0;
int value=0;
int x=0;
int major =0;

void wait(void)
{
	udelay(DELAY);
}

ssize_t fire_read(struct file *filp,char* buf, size_t count, loff_t * f_pos)
{
	
	value=0;
	x=0;
	
	printk(KERN_INFO "read");
	gpio_set_value(CS,LOW);
	gpio_set_value(DIN,HIGH);
	
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//start
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//single channel
	
	gpio_set_value(DIN,LOW);
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();
	
	gpio_set_value(DIN,LOW);
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();

	gpio_set_value(DIN,LOW);
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait(); //for 0 channel
	gpio_set_value(DIN,LOW);				//DIN finish
	
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//GAP
	
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//NULL BIT
	
	while(x<10){
		gpio_set_value(CLK,HIGH);	wait();
		value = value*2 + gpio_get_value(DOUT);
		gpio_set_value(CLK,LOW);	wait();
		x++;
		x++;
	}
	
	
	gpio_set_value(CS,HIGH);
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//end
	
	copy_to_user(buf, &value, 4);
	printk(KERN_INFO " : %d\n\n",value);
	printk(KERN_INFO "\n");
	
	return count;
}


static struct file_operations fops = {
	.read = fire_read,
};



int __init fire_init(void) {
	printk(KERN_INFO "fire : init1 \n");

	major = register_chrdev(241, "fire_dev", &fops);

	gpio_request(CS, "GPIO18");
	gpio_direction_output(CS,HIGH);
	
	gpio_request(DIN, "GPIO17");
	gpio_direction_output(DIN,LOW);
	
	gpio_request(DOUT, "GPIO27");
	gpio_direction_input(DOUT);
	
	gpio_request(CLK, "GPIO22");
	gpio_direction_output(CLK,LOW);

	printk(KERN_INFO "fire : init done\n");

	return 0;
}

void __exit fire_exit(void){
	
	gpio_free(CS);
	gpio_free(DIN);
	gpio_free(DOUT);
	gpio_free(CLK);

	unregister_chrdev(major, "fire_dev");

	printk(KERN_INFO "fire : Exit");
}

module_init(fire_init);
module_exit(fire_exit);
