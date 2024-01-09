#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Illia Roman <roman.illia@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello world program in Linux Kernel");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int display_times = 1;
module_param(display_times, uint, 0444);
MODULE_PARM_DESC(display_times, "Number of times to show 'Hello, World!'");

struct array_node
{
	struct list_head list_node;
	ktime_t timemarker;
};

static LIST_HEAD(Head_Node);

static int __init hello_init(void)
{
	int counter;

	if (display_times == 0 || (display_times >= 5 && display_times <= 10))
	{
		pr_warn("Choose smaller number!\n");
	}
	else if (display_times > 10)
	{
		pr_err("Error! Your number is too large.\n");
		return -EINVAL;
	}

	for (counter = 0; counter < display_times; counter++)
	{
		struct array_node *md = kmalloc(sizeof(*md), GFP_KERNEL);

		md->timemarker = ktime_get();
		list_add(&md->list_node, &Head_Node);

		printk(KERN_EMERG "Hello, World! 👋\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct array_node *md, *tmp;

	list_for_each_entry_safe(md, tmp, &Head_Node, list_node)
	{
		ktime_t duration = ktime_sub(ktime_get(), md->timemarker);

		pr_info("Event time: %lld ns\n", ktime_to_ns(duration));
		list_del(&md->list_node);
		kfree(md);
	}
}

module_init(hello_init);
module_exit(hello_exit);
