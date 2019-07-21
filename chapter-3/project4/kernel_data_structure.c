#include <linux/init.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>

struct color {
    int red;
    int green;
    int blue;
    struct list_head list;
};

static LIST_HEAD(color_list);
static int start = 25;

void traverse_linkedlist(struct list_head *head) {
    struct color *color;
    list_for_each_entry(color, head, list) {
        printk(KERN_INFO "traverse red: %d green:%d blue:%d", color->red, color->green, color->blue);
    }
}

int proc_init(void) {
    struct color *violet;
    int index = 0;
    for (index = 0; index < start; index++) { 
            violet = kmalloc(sizeof(*violet), GFP_KERNEL);
            violet->red = index;
            violet->blue = index;
            violet->green = index;
            INIT_LIST_HEAD(&violet->list);
            list_add_tail(&violet->list, &color_list);
    }
    
    printk(KERN_INFO "proc_init DONE");
    traverse_linkedlist(&color_list);
    return 0;
}

void proc_exit(void) {
    struct color *color, *ncolor;
    
    list_for_each_entry_safe(color, ncolor, &color_list, list) {
        printk(KERN_INFO "remove red: %d green:%d blue:%d", color->red, color->green, color->blue);
        list_del(&color->list);
        kfree(color);
    }
    printk(KERN_INFO "proc_exit DONE");
    return;
}

module_param(start, int, 0);
module_init(proc_init);
module_exit(proc_exit);
