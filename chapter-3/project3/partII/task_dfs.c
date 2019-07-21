#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/list.h>

extern struct task_struct init_task;

void dfs(struct task_struct *task, int depth) {
    struct list_head *list;
    struct task_struct *another_task;
    char comms[16] = {0};
    list_for_each(list, &task->children) {
            another_task = list_entry(list, struct task_struct, sibling);
            memset(comms, 0, 16);
            get_task_comm(comms, another_task);
            printk(KERN_INFO "%ld\t%d\t%*s%s\n", another_task->state, another_task->pid, depth, depth == 0 ? "" : "\\", comms);
            dfs(another_task, depth+1);
    }
}
int proc_init(void) {
    printk(KERN_INFO "S\tPID\tCMD\n");
    dfs(&init_task, 0);
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
