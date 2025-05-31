#ifndef _CONFIG_H
#define _CONFIG_H

#include "version.h"

/* Build configuration options */
#define CONFIG_DEBUG 1
#define CONFIG_QUANTUM_DEBUG 1
#define CONFIG_ERROR_CORRECTION 1
#define CONFIG_QUANTUM_MEMORY 1
#define CONFIG_QUANTUM_NETWORKING 1
#define CONFIG_QUANTUM_FILESYSTEM 1

/* Memory configuration */
#define CONFIG_QUANTUM_MEMORY_POOL_SIZE (1024 * 1024)  /* 1MB */
#define CONFIG_QUANTUM_MAX_QUBITS 1024
#define CONFIG_QUANTUM_MAX_QUBITS_PER_BLOCK 64

/* Network configuration */
#define CONFIG_QUANTUM_NETWORK_BUFFER_SIZE 4096
#define CONFIG_QUANTUM_NETWORK_MAX_CONNECTIONS 1024
#define CONFIG_QUANTUM_NETWORK_TIMEOUT 5000  /* 5 seconds */

/* File system configuration */
#define CONFIG_QUANTUM_FS_BLOCK_SIZE 4096
#define CONFIG_QUANTUM_FS_MAX_FILES 1024
#define CONFIG_QUANTUM_FS_MAX_PATH_LENGTH 256

/* Error correction configuration */
#define CONFIG_QUANTUM_ERROR_CORRECTION_LEVEL 3
#define CONFIG_QUANTUM_ERROR_CORRECTION_THRESHOLD 0.1

/* Performance configuration */
#define CONFIG_QUANTUM_PERFORMANCE_MONITORING 1
#define CONFIG_QUANTUM_PERFORMANCE_SAMPLING_RATE 1000  /* 1 second */

/* Security configuration */
#define CONFIG_QUANTUM_SECURITY_ENCRYPTION 1
#define CONFIG_QUANTUM_SECURITY_AUTHENTICATION 1
#define CONFIG_QUANTUM_SECURITY_ACCESS_CONTROL 1

/* Debug configuration */
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(fmt, ...) \
    printk(KERN_DEBUG "CTRLxT_STUDIOS: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

/* Quantum debug configuration */
#ifdef CONFIG_QUANTUM_DEBUG
#define QUANTUM_DEBUG_PRINT(fmt, ...) \
    printk(KERN_DEBUG "CTRLxT_STUDIOS QUANTUM: " fmt, ##__VA_ARGS__)
#else
#define QUANTUM_DEBUG_PRINT(fmt, ...)
#endif

/* Error handling */
#define QUANTUM_ERROR(fmt, ...) \
    printk(KERN_ERR "CTRLxT_STUDIOS ERROR: " fmt, ##__VA_ARGS__)

#define QUANTUM_WARNING(fmt, ...) \
    printk(KERN_WARNING "CTRLxT_STUDIOS WARNING: " fmt, ##__VA_ARGS__)

#define QUANTUM_INFO(fmt, ...) \
    printk(KERN_INFO "CTRLxT_STUDIOS INFO: " fmt, ##__VA_ARGS__)

/* Performance monitoring */
#ifdef CONFIG_QUANTUM_PERFORMANCE_MONITORING
#define QUANTUM_PERF_START() \
    do { \
        unsigned long start_time = jiffies; \
        do { \
            if (time_after(jiffies, start_time + CONFIG_QUANTUM_PERFORMANCE_SAMPLING_RATE)) { \
                QUANTUM_DEBUG_PRINT("Performance sampling at %lu jiffies\n", jiffies); \
                start_time = jiffies; \
            } \
        } while (0)
#else
#define QUANTUM_PERF_START()
#endif

#endif /* _CONFIG_H */ 