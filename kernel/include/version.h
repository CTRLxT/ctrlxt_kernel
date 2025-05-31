#ifndef _VERSION_H
#define _VERSION_H

/* Kernel version information */
#define CTRLXT_KERNEL_VERSION_MAJOR 0
#define CTRLXT_KERNEL_VERSION_MINOR 1
#define CTRLXT_KERNEL_VERSION_PATCH 0

/* Version string */
#define CTRLXT_KERNEL_VERSION_STRING "0.1.0"

/* Full version string with additional information */
#define CTRLXT_KERNEL_VERSION_FULL "CTRLxT_STUDIOS Omni-Kernel-Prime v" CTRLXT_KERNEL_VERSION_STRING

/* Version code for binary compatibility */
#define CTRLXT_KERNEL_VERSION_CODE ((CTRLXT_KERNEL_VERSION_MAJOR << 16) | \
                                  (CTRLXT_KERNEL_VERSION_MINOR << 8) | \
                                  CTRLXT_KERNEL_VERSION_PATCH)

/* Build information */
#define CTRLXT_KERNEL_BUILD_DATE __DATE__
#define CTRLXT_KERNEL_BUILD_TIME __TIME__

/* Feature flags */
#define CTRLXT_FEATURE_QUANTUM 1
#define CTRLXT_FEATURE_ERROR_CORRECTION 1
#define CTRLXT_FEATURE_MULTI_ARCH 1
#define CTRLXT_FEATURE_NETWORKING 1
#define CTRLXT_FEATURE_FILESYSTEM 1

/* Architecture support */
#ifdef CONFIG_X86
#define CTRLXT_ARCH_X86 1
#endif

#ifdef CONFIG_ARM
#define CTRLXT_ARCH_ARM 1
#endif

#ifdef CONFIG_RISCV
#define CTRLXT_ARCH_RISCV 1
#endif

/* Compiler information */
#define CTRLXT_COMPILER_NAME "GCC"
#define CTRLXT_COMPILER_VERSION __VERSION__

/* Module version information */
#define MODULE_VERSION(version) \
    MODULE_INFO(version, version)

#endif /* _VERSION_H */ 