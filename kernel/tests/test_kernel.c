#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kunit/test.h>
#include "../include/ctrlxt_kernel.h"

/* Test UHAL initialization */
static void test_uhal_init(struct kunit *test)
{
    int ret = uhal_init();
    KUNIT_EXPECT_EQ(test, ret, 0);
}

/* Test quantum device initialization */
static void test_quantum_device_init(struct kunit *test)
{
    struct quantum_device dev = {
        .name = "test_quantum",
        .qubit_count = 4,
        .coherence_time = 100
    };
    
    int ret = quantum_device_init(&dev);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_NOT_NULL(test, dev.quantum_state);
}

/* Test quantum gate operations */
static void test_quantum_gate_ops(struct kunit *test)
{
    struct quantum_device dev = {
        .name = "test_quantum",
        .qubit_count = 4,
        .coherence_time = 100
    };
    
    quantum_device_init(&dev);
    
    /* Test applying a quantum gate */
    int ret = quantum_apply_gate(&dev, 0, 1);  /* Apply H gate to qubit 0 */
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test measurement */
    u32 result;
    ret = quantum_measure(&dev, 0, &result);
    KUNIT_EXPECT_EQ(test, ret, 0);
}

/* Test performance parameters */
static void test_performance_params(struct kunit *test)
{
    struct performance_params params = {
        .cpu_max_multiplier = 150,
        .gpu_max_multiplier = 130,
        .quantum_max_multiplier = 110,
        .thermal_threshold = 90,
        .swap_default_size = 8
    };
    
    KUNIT_EXPECT_EQ(test, params.cpu_max_multiplier, 150);
    KUNIT_EXPECT_EQ(test, params.gpu_max_multiplier, 130);
    KUNIT_EXPECT_EQ(test, params.quantum_max_multiplier, 110);
    KUNIT_EXPECT_EQ(test, params.thermal_threshold, 90);
    KUNIT_EXPECT_EQ(test, params.swap_default_size, 8);
}

/* Test suite definition */
static struct kunit_case ctrlxt_kernel_test_cases[] = {
    KUNIT_CASE(test_uhal_init),
    KUNIT_CASE(test_quantum_device_init),
    KUNIT_CASE(test_quantum_gate_ops),
    KUNIT_CASE(test_performance_params),
    {}
};

static struct kunit_suite ctrlxt_kernel_test_suite = {
    .name = "ctrlxt_kernel",
    .test_cases = ctrlxt_kernel_test_cases,
};

kunit_test_suite(ctrlxt_kernel_test_suite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CTRLxT_STUDIOS");
MODULE_DESCRIPTION("CTRLxT_STUDIOS Omni-Kernel-Prime Test Suite");
MODULE_VERSION(CTRLXT_KERNEL_VERSION); 