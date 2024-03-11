#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H. Turgut Uyar");

static int hello_init(void) {
    printk(KERN_INFO "Hello, world\n");
    return 0;
}

static void hello_exit(void) {
    printk(KERN_INFO "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
