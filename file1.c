#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/err.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/gpio.h>

#define GPIO_20 20+512
#define GPIO_21 21+512
#define GPIO_22 22+512

#define size 4

//initialize
dev_t dev=0;
static struct class *dev_class;
static struct cdev ext_device;
struct gpio_desc *desc1, *desc2, *desc3;


//function declaration
static int __init device_init(void);
static void __exit device_exit(void);
static int ext_open(struct inode* inode, struct file* file);
static int ext_release(struct inode* inode, struct file* file);
static ssize_t ext_read(struct file* flip, char __user * buf, size_t len, loff_t *off);
static ssize_t ext_write(struct file* flip, const char * buf, size_t len, loff_t *off);

//strut initaialization for device file operations
static struct file_operations fops={
	.owner  = THIS_MODULE,
	.read   = ext_read,
	.write  = ext_write,
	.open   = ext_open,
	.release= ext_release,
};

//Device file open operation
static int ext_open(struct inode* inode, struct file* file){
	printk(KERN_INFO"Driver open function called:)\n");	
	return 0;
}

//Device file release operation
static int ext_release(struct inode* inode, struct file* file){
	printk(KERN_INFO"Driver Release function called:)\n");	
	return 0;
}

//Device file read from kernel operation
static ssize_t ext_read(struct file* flip, char __user * buf, size_t len, loff_t *off){
	int gpio_state[3] = {0};//used in ext_read();
	
	gpio_state[0] = gpio_get_value(GPIO_20);//Reading Gpio 20 value
	gpio_state[1] = gpio_get_value(GPIO_21);//Reading Gpio 21 value
	gpio_state[2] = gpio_get_value(GPIO_22);//Reading Gpio 22 value
	
	if(copy_to_user(buf, gpio_state, size))
		printk(KERN_ERR"Data cannot be read form GPIO pins!!! :(\n");

	for(int i=0; i<3; i++)
	printk(KERN_INFO"GPIO pin status = %d\n", gpio_state[i]);
	return 0;
}

//Device file write into kernel operation
static ssize_t ext_write(struct file* flip, const char * buf, size_t len, loff_t *off){
	
	int rec_buf=0;//used in ext_write();
	if(copy_from_user(&rec_buf, buf, len))
		printk(KERN_ERR"Data cannot be written into GPIO_20 !!! :( \n");
		
	printk(KERN_INFO"GPIO = %d\n", rec_buf);

	switch(rec_buf){
	case -1: gpio_set_value(GPIO_20,0); break;
	case -2: gpio_set_value(GPIO_21,0); break;
	case -4: gpio_set_value(GPIO_22,0); break;
	case 1 : gpio_set_value(GPIO_20,1); break;
	case 2 : gpio_set_value(GPIO_21,1); break;
	case 4 : gpio_set_value(GPIO_22,1); break;
	case 0 : break;
	default: printk(KERN_ERR"Unkown pin entry : Please provide pin = %d\n",rec_buf);
	}
	return len;
}

//Init function 
static int __init device_init(void){
	
	//Allocating major and minor number dynamcally
	if(alloc_chrdev_region(&dev,0,1,"ext_dev")<0){
		printk(KERN_ERR"Cannot create MAJOR and MINOR number\n");
		goto r_unreg;
	}else	
		printk(KERN_INFO"MAJOR and MINOR number created \n");

	//Creating cdev structure
	cdev_init(&ext_device, &fops);

	//Adding character device to system
	if(cdev_add(&ext_device, dev, 1) < 0){
		printk(KERN_ERR"Character device cannot be added\n");
		goto r_cdev;
	}	 
	
	//creating class to store the file information in kernel
	if(IS_ERR(dev_class = class_create("ext_class"))){
		printk(KERN_ERR"Class not created\n");
		goto r_class;
	}
	
	//creating the device file
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "ext_device"))){
		printk(KERN_ERR"Device not created\n");
		goto r_device;
	}	

	//checking the valid gpio pin
	if(!gpio_is_valid(GPIO_20)){
		printk(KERN_ERR"ERROR: GPIO num %d is not valid\n", GPIO_20-512);
		goto r_device;
	}
	if(!gpio_is_valid(GPIO_21)){
		printk(KERN_ERR"ERROR: GPIO num %d is not valid\n", GPIO_21-512);
		goto r_gpio20;
	}
	if(!gpio_is_valid(GPIO_22)){
		printk(KERN_ERR"ERROR: GPIO num %d is not valid\n", GPIO_22-512);
		goto r_gpio21;
	}
	
	//Requesting the gpio number
	if(gpio_request(GPIO_20, "GPIO_20")<0){
		printk(KERN_ERR"ERROR: GPIO %d request\n", GPIO_20);
		goto r_gpio20;	
	}
	if(gpio_request(GPIO_21, "GPIO_21")<0){
		printk(KERN_ERR"ERROR: GPIO %d request\n", GPIO_21);
		goto r_gpio21;	
	}
	if(gpio_request(GPIO_22, "GPIO_22")<0){
		printk(KERN_ERR"ERROR: GPIO %d request\n", GPIO_22);
		goto r_gpio22;	
	}
		
	//setting the gpio as output direction
	gpio_direction_output(GPIO_20, 0);
	gpio_direction_output(GPIO_21, 0);
	gpio_direction_output(GPIO_22, 0);

	//Export the gpio for user space	
	desc1  = gpio_to_desc(GPIO_20);
	desc2  = gpio_to_desc(GPIO_21);
	desc3  = gpio_to_desc(GPIO_22);

	//second arg 0 means we cannot change the output direction here after
	gpiod_export(desc1, 0);
	gpiod_export(desc2, 0);
	gpiod_export(desc3, 0);
	
	printk(KERN_INFO"Kernel module inserted\n");
	return 0;

//Error Handling	
r_gpio22:
	gpio_free(GPIO_22);	
r_gpio21:
	gpio_free(GPIO_21);	
r_gpio20:
	gpio_free(GPIO_20);	
r_device:
	device_destroy(dev_class, dev);
r_class:
	class_destroy(dev_class);
r_cdev:
	cdev_del(&ext_device);
r_unreg:
	unregister_chrdev_region(dev,1);
	return -1;
}

//Exit function
static void __exit device_exit(void){
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&ext_device);
	unregister_chrdev_region(dev,1);
	gpiod_unexport(desc1);
	gpiod_unexport(desc2);
	gpiod_unexport(desc3);
	gpio_free(GPIO_20);
	gpio_free(GPIO_21);
	gpio_free(GPIO_22);
	printk(KERN_INFO"Kernel module removed\n");
}

//Module initializtion
module_init(device_init);
module_exit(device_exit);

//Information about this driver programming
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BHUVI<bhuvankmg220@gmail.com>");
MODULE_DESCRIPTION("Real Device Driver");
MODULE_VERSION("2.0");



