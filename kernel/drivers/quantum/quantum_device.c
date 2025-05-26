#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include "../../include/ctrlxt_kernel.h"
#include "../../include/quantum.h"
#include "../../include/quantum_memory.h"

/* Quantum device structure */
struct ctrlxt_quantum_device {
    struct device *dev;
    struct cdev cdev;
    dev_t devt;
    unsigned int minor;
    struct quantum_state *state;
    spinlock_t lock;
    atomic_t open_count;
    atomic_t operation_count;
    struct quantum_memory_block *memory;
    unsigned long capabilities;
    void *private_data;
};

static struct ctrlxt_quantum_device *quantum_dev;
static struct class *quantum_class;
static int quantum_major;

/* Device operations */
static int quantum_open(struct inode *inode, struct file *file)
{
    struct ctrlxt_quantum_device *dev = container_of(inode->i_cdev, struct ctrlxt_quantum_device, cdev);
    
    if (atomic_inc_return(&dev->open_count) > 1) {
        atomic_dec(&dev->open_count);
        return -EBUSY;
    }
    
    file->private_data = dev;
    return 0;
}

static int quantum_release(struct inode *inode, struct file *file)
{
    struct ctrlxt_quantum_device *dev = file->private_data;
    
    atomic_dec(&dev->open_count);
    return 0;
}

static long quantum_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct ctrlxt_quantum_device *dev = file->private_data;
    int ret = 0;
    
    switch (cmd) {
        case QUANTUM_IOCTL_INIT:
            ret = quantum_state_init(dev->state, arg);
            break;
            
        case QUANTUM_IOCTL_MEASURE:
            ret = quantum_state_measure(dev->state, (void *)arg);
            break;
            
        case QUANTUM_IOCTL_APPLY_GATE:
            {
                struct quantum_gate_params params;
                if (copy_from_user(&params, (void *)arg, sizeof(params)))
                    return -EFAULT;
                ret = quantum_gate_apply(params.gate, dev->state, params.qubit, NULL, 0);
            }
            break;
            
        case QUANTUM_IOCTL_ALLOC_MEMORY:
            {
                struct quantum_memory_params params;
                if (copy_from_user(&params, (void *)arg, sizeof(params)))
                    return -EFAULT;
                dev->memory = ctrlxt_qmem_alloc(params.num_qubits, params.flags);
                if (IS_ERR(dev->memory))
                    ret = PTR_ERR(dev->memory);
            }
            break;
            
        case QUANTUM_IOCTL_FREE_MEMORY:
            if (dev->memory) {
                ctrlxt_qmem_free(dev->memory);
                dev->memory = NULL;
            }
            break;
            
        default:
            ret = -ENOTTY;
            break;
    }
    
    if (ret == 0)
        atomic_inc(&dev->operation_count);
    
    return ret;
}

static const struct file_operations quantum_fops = {
    .owner = THIS_MODULE,
    .open = quantum_open,
    .release = quantum_release,
    .unlocked_ioctl = quantum_ioctl,
    .compat_ioctl = quantum_ioctl,
};

/* Initialize quantum device */
int __init ctrlxt_quantum_device_init(void)
{
    int ret;
    
    pr_info("CTRLxT_STUDIOS: Initializing quantum device driver\n");
    
    /* Allocate device structure */
    quantum_dev = kzalloc(sizeof(struct ctrlxt_quantum_device), GFP_KERNEL);
    if (!quantum_dev) {
        pr_err("CTRLxT_STUDIOS: Failed to allocate quantum device structure\n");
        return -ENOMEM;
    }
    
    /* Initialize device structure */
    spin_lock_init(&quantum_dev->lock);
    atomic_set(&quantum_dev->open_count, 0);
    atomic_set(&quantum_dev->operation_count, 0);
    
    /* Allocate quantum state */
    quantum_dev->state = quantum_state_alloc(8);  // 8 qubits for device
    if (!quantum_dev->state) {
        pr_err("CTRLxT_STUDIOS: Failed to allocate quantum state\n");
        ret = -ENOMEM;
        goto error;
    }
    
    /* Register character device */
    ret = alloc_chrdev_region(&quantum_dev->devt, 0, 1, "quantum");
    if (ret < 0) {
        pr_err("CTRLxT_STUDIOS: Failed to allocate device number\n");
        goto error;
    }
    
    quantum_major = MAJOR(quantum_dev->devt);
    quantum_dev->minor = MINOR(quantum_dev->devt);
    
    /* Initialize character device */
    cdev_init(&quantum_dev->cdev, &quantum_fops);
    quantum_dev->cdev.owner = THIS_MODULE;
    
    ret = cdev_add(&quantum_dev->cdev, quantum_dev->devt, 1);
    if (ret < 0) {
        pr_err("CTRLxT_STUDIOS: Failed to add character device\n");
        goto error;
    }
    
    /* Create device class */
    quantum_class = class_create(THIS_MODULE, "quantum");
    if (IS_ERR(quantum_class)) {
        pr_err("CTRLxT_STUDIOS: Failed to create device class\n");
        ret = PTR_ERR(quantum_class);
        goto error;
    }
    
    /* Create device */
    quantum_dev->dev = device_create(quantum_class, NULL, quantum_dev->devt, NULL, "quantum0");
    if (IS_ERR(quantum_dev->dev)) {
        pr_err("CTRLxT_STUDIOS: Failed to create device\n");
        ret = PTR_ERR(quantum_dev->dev);
        goto error;
    }
    
    pr_info("CTRLxT_STUDIOS: Quantum device driver initialized successfully\n");
    return 0;

error:
    if (quantum_dev) {
        if (quantum_dev->state)
            quantum_state_free(quantum_dev->state);
        if (quantum_dev->memory)
            ctrlxt_qmem_free(quantum_dev->memory);
        kfree(quantum_dev);
    }
    return ret;
}

/* Module initialization */
static int __init quantum_init(void)
{
    return ctrlxt_quantum_device_init();
}

/* Module cleanup */
static void __exit quantum_exit(void)
{
    if (quantum_dev) {
        device_destroy(quantum_class, quantum_dev->devt);
        class_destroy(quantum_class);
        cdev_del(&quantum_dev->cdev);
        unregister_chrdev_region(quantum_dev->devt, 1);
        
        if (quantum_dev->state)
            quantum_state_free(quantum_dev->state);
        if (quantum_dev->memory)
            ctrlxt_qmem_free(quantum_dev->memory);
        
        kfree(quantum_dev);
    }
    
    pr_info("CTRLxT_STUDIOS: Quantum device driver unloaded\n");
}

module_init(quantum_init);
module_exit(quantum_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CTRLxT_STUDIOS");
MODULE_DESCRIPTION("CTRLxT_STUDIOS Omni-Kernel-Prime Quantum Device Driver");
MODULE_VERSION(CTRLXT_KERNEL_VERSION); 