#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Illia Roman <roman.illia@lll.kpi.ua>");
MODULE_DESCRIPTION("Function print_hello() for 'hello2' module");
MODULE_LICENSE("Dual BSD/GPL");


struct array_node {
   struct list_head list_node;
      ktime_t time_marker;
      ktime_t exec_duration;
};

static LIST_HEAD(Head_Node);

void print_hello(unsigned int display_times)
{
	int counter;

	for (counter = 0; counter < display_times; counter++)
	{
		struct array_node *md = kmalloc(sizeof(*md), GFP_KERNEL);

		md->time_marker = ktime_get();
		list_add(&md->list_node, &Head_Node);

		pr_info("Hello, World! 👋\n");

		md->exec_duration = ktime_sub(ktime_get(), md->time_marker);
	}
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("Module 'hello1' is loaded.\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct array_node *md, *tmp;

	list_for_each_entry_safe(md, tmp, &Head_Node, list_node)
	{
		ktime_t module_duration = ktime_sub(ktime_get(), md->time_marker);

		pr_info("Module 1 execution time: %lld ns.\n  Execution time for printing: %lld ns.\n",
				ktime_to_ns(module_duration),
				ktime_to_ns(md->exec_duration));

		list_del(&md->list_node);
		kfree(md);
	}

	pr_info("Module 'hello1' is unloaded.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
