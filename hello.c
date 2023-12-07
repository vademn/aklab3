#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/export.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>, edited by Kravchenko Vadym IO-12");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

struct my_struct {
    struct list_head list_node;  // Linked list node
    ktime_t timestamp;           // ktime_t field
};

// How many times the string should be printed
static uint number_of_hellos = 7;
module_param(number_of_hellos, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(number_of_hellos, "Number of times to print \"Hello, world!\"");

// Declare a static variable for the head of the list
static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
    struct my_struct *entry;

    //Print the string based on the number_of_hellos value
    pr_info("Number_of_hellos: %u\n", number_of_hellos);

    if (number_of_hellos == 0 || (number_of_hellos > 5 && number_of_hellos <= 10)) {
        pr_warn("Warning: Invalid number_of_hellos value. Using the default value (1).\n");
    } else if (number_of_hellos > 10) {
        pr_err("Error: number_of_hellos is too large. Module is not loaded.\n");
        return -EINVAL;
    }



    while (number_of_hellos-- > 0) {
        entry = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
        if (!entry) {
            pr_err("Error allocating memory for my_struct.\n");
            return -ENOMEM;
        }

        entry->timestamp = ktime_get();  // Set the timestamp

        // Add entry to the linked list
        list_add_tail(&entry->list_node, &my_list_head);

        // Print Hello, world! and Event time
        printk(KERN_EMERG "Hello, World!(from Kravchenko Vadym) Event time in nanoseconds: %lld\n", ktime_to_ns(entry->timestamp));
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct my_struct *entry, *tmp;

    // Iterate through the list
    list_for_each_entry_safe(entry, tmp, &my_list_head, list_node) {
        // Print Event time in nanoseconds
        printk(KERN_EMERG "BYE! Event time in nanoseconds: %lld ns\n", ktime_to_ns(entry->timestamp));

        // Remove the entry from the list
        list_del(&entry->list_node);

        // Free the allocated memory
        kfree(entry);
    }
}

module_init(hello_init);
module_exit(hello_exit);

