#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H. Turgut Uyar");

static int pseudo_major = 0;
static int pseudo_minor = 0;

static int pseudo_init(void) {
    dev_t devno = 0;
    int result = -1;

    if (pseudo_major == 0) {
        result = alloc_chrdev_region(&devno, pseudo_minor, 1, "pseudo");
        pseudo_major = MAJOR(devno);
    } else {
        devno = MKDEV(pseudo_major, pseudo_minor);
        result = register_chrdev_region(devno, 1, "pseudo");
    }
    if (result < 0) {
        printk(KERN_WARNING "pseudo: can't get major %d\n", pseudo_major);
        return result;
    }
    return 0;
}

static void pseudo_exit(void) {
    dev_t devno = MKDEV(pseudo_major, pseudo_minor);
    unregister_chrdev_region(devno, 1);
}

module_init(pseudo_init);
module_exit(pseudo_exit);
