#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct head_list {
	struct list_head item_next;
	ktime_t time;
};

MODULE_AUTHOR("Hana Khalil <hanka100h@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static struct list_head head = LIST_HEAD_INIT(head);

static uint hw_count = 1;
module_param(hw_count, uint, 0444);
MODULE_PARM_DESC(hw_count, "This parameter is counter of hello world");

static int __init hello_init(void)
{
	uint i = 0;

	struct head_list *temp_var;

	pr_info("Input %d\n", hw_count);

	if (hw_count == 0 || (hw_count >= 5 && hw_count <= 10)) {
		pr_warn("Warning! Parameter should not be 0 and should be in [5, 10]");
		pr_info("");
	}

	BUG_ON(hw_count > 10);

	for (i = 0; i < hw_count; i++) {
		temp_var = kmalloc(sizeof(struct head_list), GFP_KERNEL);

		if (i == 5)
			temp_var = 0;

		if (ZERO_OR_NULL_PTR(temp_var))
			goto error;

		temp_var->time = ktime_get();
		pr_info("Hello, world!\n");

		list_add_tail(&(temp_var->item_next), &head);
	}



	pr_info("");
	return 0;

error:
	{
		struct head_list *md, *tmp;

		pr_err("kmalloc is running out of memory");
		list_for_each_entry_safe(md, tmp, &head, item_next) {
			list_del(&md->item_next);
			kfree(md);
		}
		BUG();
		return -ENOMEM;
	}
}

static void __exit hello_exit(void)
{
	struct head_list *md, *tmp;

	list_for_each_entry_safe(md, tmp, &head, item_next) {
		pr_info("ime: %lld", md->time);
		list_del(&md->item_next);
		kfree(md);
	}
	BUG_ON(!list_empty(&head));
	pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);
