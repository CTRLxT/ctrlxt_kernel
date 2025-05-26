#ifndef CTRLXT_KERNEL_H
#define CTRLXT_KERNEL_H

#include <linux/types.h>
#include <linux/compiler.h>

/* Core Kernel Version Information */
#define CTRLXT_KERNEL_VERSION "0.1.0"
#define CTRLXT_KERNEL_NAME "CTRLxT_STUDIOS Omni-Kernel-Prime"

/* Universal Hardware Abstraction Layer (UHAL) */
struct uhal_device {
    char name[64];
    u32 device_id;
    u32 device_class;
    void *driver_data;
    struct list_head list;
};

struct uhal_arch {
    char name[32];
    u32 arch_id;
    void (*init)(void);
    void (*detect_hardware)(void);
    void (*setup_quantum)(void);
};

/* Quantum Computing Support */
struct quantum_device {
    char name[64];
    u32 qubit_count;
    u32 coherence_time;
    void *quantum_state;
    struct list_head list;
};

/* Performance Management */
struct performance_params {
    u32 cpu_max_multiplier;
    u32 gpu_max_multiplier;
    u32 quantum_max_multiplier;
    u32 thermal_threshold;
    u32 swap_default_size;
};

/* Core Kernel Structure */
struct ctrlxt_kernel {
    struct uhal_arch *arch;
    struct list_head devices;
    struct list_head quantum_devices;
    struct performance_params perf_params;
    bool quantum_enabled;
};

/* Function Declarations */
int ctrlxt_kernel_init(void);
int uhal_init(void);
int quantum_init(void);
int performance_init(void);

/* Architecture-specific functions */
#ifdef CONFIG_ARCH_X86
int x86_arch_init(void);
#endif

#ifdef CONFIG_ARCH_ARM
int arm_arch_init(void);
#endif

#ifdef CONFIG_ARCH_RISCV
int riscv_arch_init(void);
#endif

#ifdef CONFIG_QUANTUM
int quantum_arch_init(void);
#endif

#endif /* CTRLXT_KERNEL_H */ 