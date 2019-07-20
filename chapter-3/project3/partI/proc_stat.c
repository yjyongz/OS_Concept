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
#include <linux/timex.h>

// CPU percentage nees to be implemented
int pcpu(struct task_struct *task) {
    int percentage = 0;
    return percentage;
}
int proc_init(void) {
    struct task_struct *task;
    char comms[16] = {0};
    char time[16] = {0};
    char wchan[16] = {0};
    char file[16] = {0};
    unsigned long lt;
    int physize = 0;

    printk(KERN_INFO "F\tS\tUID\tPID\tPPID\tC\tPRI\tNI\tSZ\tTIME\tCMD\n");
    
    for_each_process(task) {
        memset(comms, 0, 16);
        memset(time, 0, 16);
        memset(wchan, 0, 16);
        memset(file, 0, 16);
        get_task_comm(comms, task);
        //lt = (cputime_to_jiffies(task->utime) + cputime_to_jiffies(task->stime)) >> (SHIFT_HZ + 3);
        // cumulative CPU time doesn' show correctly
        lt = (task->utime + task->stime);
        lt /= HZ;
        if (task->mm && task->mm) {
            physize = task->mm->total_vm;
        } else {
            physize = 0;
        }
        snprintf(time, 16, "%lu-%02ld:%02ld:%02ld",
                (lt/(60*60))/24, (lt/(60*60))%24, (lt/60)%60, lt%60);
        printk(KERN_INFO "%d\t%ld\t%u\t%d\t%d\t"
               "%d\t%d\t%d\t%d\t%s\t%s\n",
               task->flags, task->state, task_uid(task).val, task->pid, task_ppid_nr(task), 
               pcpu(task), task->rt_priority, task_nice(task), 
               physize, time, comms);
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
