#ifndef _UHAL_H
#define _UHAL_H

#include <linux/types.h>
#include <linux/ioctl.h>

/* UHAL IOCTL commands */
#define UHAL_MAGIC 'U'
#define UHAL_CMD_INIT _IOR(UHAL_MAGIC, 1, int)
#define UHAL_CMD_DETECT_HW _IOR(UHAL_MAGIC, 2, int)
#define UHAL_CMD_SETUP_QUANTUM _IOR(UHAL_MAGIC, 3, int)

/* UHAL architecture structure */
struct uhal_arch {
    const char *name;
    u8 arch_id;
    int (*init)(void);
    void (*detect_hardware)(void);
    void (*setup_quantum)(void);
};

/* UHAL device structure */
struct uhal_device {
    const char *name;
    u32 device_id;
    u32 capabilities;
    void *private_data;
};

/* UHAL initialization function */
int uhal_init(void);

/* Hardware detection function */
void uhal_detect_hardware(void);

/* Quantum computing setup function */
void uhal_setup_quantum(void);

/* Architecture registration function */
int uhal_register_arch(const struct uhal_arch *arch);

/* Device registration function */
int uhal_register_device(const struct uhal_device *dev);

/* Device unregistration function */
void uhal_unregister_device(const struct uhal_device *dev);

#endif /* _UHAL_H */ 