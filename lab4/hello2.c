#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Illia Roman <roman.illia@lll.kpi.ua>");
MODULE_DESCRIPTION("Print 'Hello, World' module");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int display_times = 1;
module_param(display_times, uint, 0444);
MODULE_PARM_DESC(display_times, "Number of times to show 'Hello, World!'");


static int __init hello2_init(void)
{
   pr_info("Module 'hello2' is loaded.\n");
   ktime_t module_duration = ktime_sub(ktime_get(), md->time_marker);

   if (display_times == 0 || (display_times >= 5 && display_times <= 10))
   {
      pr_warn("Please, choose number from 1 to 4!\n");
   }
   else if (display_times > 10)
   {
      pr_err("Error! Your number is too large.\n");
      return -EINVAL;
   }

   print_hello(display_times);

   return 0;
}

static void __exit hello2_exit(void)
{
   pr_info("Module 'hello2' in unloaded.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);