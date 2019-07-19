#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/cred.h>

int pcpu(struct task_struct *task) {
    int percentage = 0;
    //int total_time = task->utime + task->stime;
    //total_time += 
    //task->real_start_time -  
    return percentage;
}
int proc_init(void) {
    struct task_struct *task;
    //unsigned long long used_jiffies= 0;
    char comms[16] = {0};

    printk(KERN_INFO "F\tS\tUID\tPID\tPPID\tC\tPRI\tNI\tADDR\tSZ\tWCHAN\tTTY\tTIME\tCMD\n");
    
    for_each_process(task) {
        memset(comms, 0, 16);
        get_task_comm(comms, task);
        printk(KERN_INFO "%d\t%ld\t%u\t%d\t%d\t"
               "%d\t%d\t%d\t%s\t%lld\t%lld\n", 
               task->flags, task->state, task_uid(task).val, task->pid, task->parent->pid, 
               pcpu(task), task->rt_priority, task_nice(task), comms, 
               task->start_time, task->real_start_time);
    }
    return 0;
}

void proc_exit(void) {
    return;
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("proc stat");
MODULE_AUTHOR("Shengjun Ma");
