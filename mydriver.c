#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define MAJOR_NUMBER 61
#define SIZE 4194304
/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
static void onebyte_exit(void);
loff_t my_llseek(struct file *filp, loff_t off, int whence);
/* definition of file_operation structure */
struct file_operations onebyte_fops = {
	llseek:my_llseek,
	read: onebyte_read,
	write: onebyte_write,
	open: onebyte_open,
	release: onebyte_release

};

struct four_byte_data{
	char *data;
	size_t count;
};

struct four_byte_data *gb_fbd=NULL;
//char *onebyte_data = NULL;
//int data_count=0;
int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

loff_t my_llseek(struct file *filp, loff_t off, int whence){
	  	loff_t newpos;

        switch(whence)
        {
        case 0: /* SEEK_SET */
                newpos = off;
                break;

        case 1: /* SEEK_CUR */
                newpos = filp->f_pos + off;
                break;

        case 2: /* SEEK_END */
                newpos = gb_fbd->count-1 + off;
                break;

        default: /* can't happen */
                return -EINVAL;
        }
        if (newpos < 0||newpos>=gb_fbd->count){
                return -EINVAL;
		}
        filp->f_pos = newpos;
        return newpos;
}
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	ssize_t bytes_read = 0;
	if(!gb_fbd||!gb_fbd->data){
		return 0;
	}
	char *dataArr=gb_fbd->data;
	//Only proceed when (*f_pos) is zero	
	if((*f_pos)<gb_fbd->count){
		char info=(dataArr[*f_pos]);
		//put to user space
		put_user(info,buf);
		++bytes_read;	
		++(*f_pos);
	}

	return bytes_read;
}

ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	if(!gb_fbd||!gb_fbd->data){
		return -ENOSPC;
	}
	char *dataArr=gb_fbd->data;
	size_t byte_read=0;
	printk(KERN_ALERT "DEBUG FPOS:%ld\n",*f_pos);
	printk(KERN_ALERT "DEBUG count:%ld\n",count);
	while((*f_pos<SIZE)&&!get_user((dataArr[*f_pos]),buf)&&count){
		++(*f_pos);
		++buf;
		gb_fbd->count=*f_pos;
		++byte_read;
		--count;
	}
	
	printk(KERN_ALERT "Readed Bytes:%ld\n",byte_read);
	 if(count>0){
		//if (*f_pos) is more than 0, return no space error
		return -ENOSPC;
		
	}else{
		
		return byte_read;
	}
	
}


static int onebyte_init(void)
{
	int result;
	// register the device
	result = register_chrdev(MAJOR_NUMBER, "onebyte", &onebyte_fops);
	if (result < 0) {
		return result;
	}
	// allocate one byte of memory for storage
	// kmalloc is just like malloc, the second parameter is
	// the type of memory to be allocated.
	// To release the memory allocated by kmalloc, use kfree.
	gb_fbd = kmalloc(sizeof(struct four_byte_data), GFP_KERNEL);
	if (!gb_fbd) {
		onebyte_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}
	gb_fbd->data=NULL;	
	gb_fbd->data=kmalloc(SIZE,GFP_KERNEL);
	if(!gb_fbd->data){
		onebyte_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}
	// initialize the value to be X
	//onebyte_data[0] = 'X';
	(gb_fbd->data)[0]='X';
	gb_fbd->count=1;
	printk(KERN_ALERT "This is a onebyte device module\n");

	//printk(KERN_ALERT "SIZE:%ld\n",SIZE);
	return 0;
}

static void onebyte_exit(void)
{
	// if the pointer is pointing to something
	if (gb_fbd) {
	// free the memory and assign the pointer to NULL
		if(gb_fbd->data){
			kfree(gb_fbd->data);
			gb_fbd->data=NULL;
		}
	kfree(gb_fbd);
	gb_fbd = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);
