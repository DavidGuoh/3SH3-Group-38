#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds" //state the name of proc

unsigned long start_seconds;

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

int proc_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &proc_ops); //create proc
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	start_seconds = jiffies/HZ; 
    return 0;
}
/* This function is called when the module is removed. */
void proc_exit(void) 
{
    remove_proc_entry(PROC_NAME, NULL); //Remove proc
	printk(KERN_INFO "Removing Module\n");
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        unsigned long current_seconds;
        static int completed = 0;

        current_seconds = jiffies/HZ - start_seconds; //calculate the current time using jiffies and HZ now and start_seconds recorded

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;

        rv = sprintf(buffer, "Elapsed Time: %lu S\n",current_seconds); //Print the seconds to prompt

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );
// Reference: Hello.c Proc.c