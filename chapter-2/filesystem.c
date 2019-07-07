#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/gcd.h>
#include <linux/hash.h>
#include <asm/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME "helloworld"
#define JIFFIES "jiffies"
#define SECONDS "seconds"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
ssize_t proc_read_jiffies(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
ssize_t proc_read_seconds(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops1 = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

static struct file_operations proc_ops2 = {
    .owner = THIS_MODULE,
    .read = proc_read_jiffies,
};

static struct file_operations proc_ops3 = {
    .owner = THIS_MODULE,
    .read = proc_read_seconds,
};

int start = 0;
/* This function is called when the module is loaded. */
int proc_init(void)
{
    start = jiffies;
    /* creates the /proc/hello entry */
    proc_create(PROC_NAME, 0666, NULL, &proc_ops1);
    proc_create(JIFFIES, 0666, NULL, &proc_ops2);
    proc_create(SECONDS, 0666, NULL, &proc_ops3);
    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void)
{
    /* removes the /proc/hello entry */
    remove_proc_entry(PROC_NAME, NULL);
    remove_proc_entry(JIFFIES, NULL);
    remove_proc_entry(SECONDS, NULL);
}


/* This function is called each time /proc/helloworld is read */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
  int rv = 0;
  char buffer[BUFFER_SIZE];
  static int completed = 0;

  if (completed) {
  	completed = 0;
  	return 0;
  }
  completed = 1;
  rv = sprintf(buffer, "Hello World\n");
  /* copies kernel space buffer to user space usr_buf */
  raw_copy_to_user(usr_buf, buffer, rv);
  return rv;
}

ssize_t proc_read_jiffies(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
  int rv = 0;
  char buffer[BUFFER_SIZE];
  static int completed1 = 0;

  if (completed1) {
  	completed1 = 0;
  	return 0;
  }
  completed1 = 1;
  rv = sprintf(buffer, "jiffies:%lu\n", jiffies);
  /* copies kernel space buffer to user space usr_buf */
  raw_copy_to_user(usr_buf, buffer, rv);
  return rv;
}

ssize_t proc_read_seconds(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
  int rv = 0;
  char buffer[BUFFER_SIZE];
  static int completed2 = 0;

  int end = jiffies;

  if (completed2) {
  	completed2 = 0;
  	return 0;
  }
  completed2 = 1;
  rv = sprintf(buffer, "time lapsed since load: %d\n", (end- start) / HZ);
  /* copies kernel space buffer to user space usr_buf */
  raw_copy_to_user(usr_buf, buffer, rv);
  return rv;
}
module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HelloWorld Module");
MODULE_AUTHOR("Shengjun Ma");
