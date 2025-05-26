# CTRLxT_STUDIOS Omni-Kernel-Prime Makefile

obj-m += ctrlxt_kernel.o
obj-m += ctrlxt_kernel_test.o

ctrlxt_kernel-objs := init/main.o \
                      quantum/quantum.o \
                      quantum/error_correction.o \
                      quantum/quantum_classical.o \
                      quantum/quantum_memory.o \
                      drivers/uhal/uhal_driver.o \
                      drivers/quantum/quantum_device.o \
                      mm/memory.o \
                      fs/ctrlxt_fs.o \
                      net/ctrlxt_net.o \
                      arch/x86/arch.o \
                      arch/arm/arch.o \
                      arch/riscv/arch.o

ctrlxt_kernel_test-objs := tests/test_kernel.o

# Architecture-specific configuration
ifeq ($(ARCH),x86)
    ctrlxt_kernel-objs += arch/x86/arch.o
endif

ifeq ($(ARCH),arm)
    ctrlxt_kernel-objs += arch/arm/arch.o
endif

ifeq ($(ARCH),riscv)
    ctrlxt_kernel-objs += arch/riscv/arch.o
endif

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

test:
	@echo "Running kernel module tests..."
	@sudo insmod ctrlxt_kernel.ko
	@sudo insmod ctrlxt_kernel_test.ko
	@sudo rmmod ctrlxt_kernel_test
	@sudo rmmod ctrlxt_kernel
	@dmesg | grep "CTRLxT_STUDIOS"

checkpatch:
	@echo "Checking code style..."
	@scripts/checkpatch.pl --no-tree -f *.c */*.c */*/*.c

.PHONY: all clean install test checkpatch 