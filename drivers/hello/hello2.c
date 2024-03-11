#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H. Turgut Uyar");

static int howmany = 1;
static char *whom = "world";

module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init(void) {
    int i;
    for (i = 0; i < howmany; i++) {
        printk(KERN_INFO "(%d) Hello, %s\n", i + 1, whom);
    }
    return 0;
}

static void hello_exit(void) {
    printk(KERN_INFO "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
