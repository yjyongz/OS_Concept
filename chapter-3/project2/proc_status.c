#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define PROC_NAME "pid"

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);
static long pid;
static struct file_operations proc_ops = {
    owner:  THIS_MODULE,
    read:  proc_read,
    write:  proc_write,
};

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[256];
    struct pid *spid = NULL;
    struct task_struct *task = NULL;
    static int completed = 0;
    
    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;

    spid = find_vpid(pid);
    task = pid_task(spid, PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_INFO "task is null pid:%ld\n", pid);
        return 0;
    }
    rv = snprintf(buffer, 256, "command[%s] pid[%ld] state=[%ld]\n", task->comm, pid, task->state);
    //printk(KERN_INFO " buffer: %s\n", buffer);
    /* copies kernel space buffer to user space usr_buf */
    raw_copy_to_user(usr_buf, buffer, rv);
    return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem = kmalloc(count, GFP_KERNEL);

    raw_copy_from_user(k_mem, usr_buf, count);
    k_mem[count-1] = 0;
    //printk(KERN_INFO "writing %s\n-----%ld", k_mem, count);
    kstrtol(k_mem, 10, &pid);
    //printk(KERN_INFO "writing pid: %ld\n", pid);
    kfree(k_mem);
    return count;
}

int proc_task_init(void) {
    printk(KERN_INFO "proc task init\n");
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    return 0;
}

void proc_task_exit(void) {
    printk(KERN_INFO "proc task exit\n");
    remove_proc_entry(PROC_NAME, NULL);
    return;
}


module_init(proc_task_init);
module_exit(proc_task_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("proc task");
MODULE_AUTHOR("Shengjun Ma");
