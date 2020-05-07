#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define	CS		8
#define	DIN		10
#define	DOUT	9
#define CLK		11

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

ssize_t inf_read(struct file *filp,char* buf, size_t count, loff_t * f_pos)
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

	if(channel == 0) gpio_set_value(DIN,LOW);
	else gpio_set_value(DIN,HIGH);
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
	}
	
	
	gpio_set_value(CS,HIGH);
	gpio_set_value(CLK,HIGH);	wait();
	gpio_set_value(CLK,LOW);	wait();	//end
	
	copy_to_user(buf, &value, 4);
	printk(KERN_INFO " : %d\n\n",value);
	printk(KERN_INFO "\n");
	
	return count;
}

ssize_t inf_write(struct file *filp,const char* buf, size_t count, loff_t * f_pos)
{	int temp=0;
	copy_from_user(&temp, buf, 4);
	channel= temp;
	return count;
}

static struct file_operations fops = {
	.write = inf_write,
	.read = inf_read,
};



int __init inf_init(void) {
	printk(KERN_INFO "infraid : init \n");

	major = register_chrdev(237, "inf_dev", &fops);

	printk(KERN_INFO "CS : %d \n",gpio_request(CS, "GPIO8"));
	gpio_direction_output(CS,HIGH);
	
	printk(KERN_INFO "DIN : %d \n",gpio_request(DIN, "GPIO10"));
	gpio_direction_output(DIN,LOW);
	
	
	printk(KERN_INFO "DOUT : %d \n",gpio_request(DOUT, "GPIO9"));
	gpio_direction_input(DOUT);
	
	
	printk(KERN_INFO "CLK : %d \n",gpio_request(CLK, "GPIO11"));
	gpio_direction_output(CLK,LOW);

	printk(KERN_INFO "infraid : init done\n");

	return 0;
}

void __exit inf_exit(void){
	
	gpio_free(CS);
	gpio_free(DIN);
	gpio_free(DOUT);
	gpio_free(CLK);

	unregister_chrdev(major, "inf_dev");

	printk(KERN_INFO "infraid : Exit");
}

module_init(inf_init);
module_exit(inf_exit);

