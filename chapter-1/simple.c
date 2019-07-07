#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gcd.h>
#include <linux/hash.h>
#include <asm/param.h>


int start;
int end;
/* This function is called when the module is loaded. */
int simple_init(void)
{
   start = jiffies;
   printk(KERN_INFO "loading kernel module\n");
   printk(KERN_INFO "GOLDEN RATIO PRIME: %llu\n", GOLDEN_RATIO_PRIME);
   printk(KERN_INFO "HZ:%d entering jiffies: %lu\n", HZ, jiffies);
   return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
   end = jiffies;
   printk(KERN_INFO "Removing Kernel Module\n");
   printk(KERN_INFO "gcd(21,93)= %lu\n", gcd(21,93));
   printk(KERN_INFO "HZ:%d exit jiffies: %lu\n", HZ, jiffies);
   printk(KERN_INFO "time lapsed: %lu\n", (end - start) / HZ);
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Shengjun Ma");
