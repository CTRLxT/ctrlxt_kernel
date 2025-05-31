#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kunit/test.h>
#include "../include/security.h"
#include "../include/performance.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CTRLxT_STUDIOS");
MODULE_DESCRIPTION("Security and Performance Integration Tests");
MODULE_VERSION("0.1.0");

/* Test security context creation and management */
static void test_security_context(struct kunit *test)
{
    struct quantum_security_context *ctx;
    int ret;
    
    /* Test context creation */
    ctx = quantum_security_create_context(QUANTUM_SECURITY_LEVEL_STANDARD,
                                        QUANTUM_SECURITY_FLAG_ENCRYPTION |
                                        QUANTUM_SECURITY_FLAG_AUTHENTICATION);
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);
    KUNIT_EXPECT_EQ(test, ctx->security_level, QUANTUM_SECURITY_LEVEL_STANDARD);
    
    /* Test level setting */
    ret = quantum_security_set_level(ctx, QUANTUM_SECURITY_LEVEL_HIGH);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, ctx->security_level, QUANTUM_SECURITY_LEVEL_HIGH);
    
    /* Test flag setting */
    ret = quantum_security_set_flags(ctx, QUANTUM_SECURITY_FLAG_ACCESS_CONTROL);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, ctx->flags, QUANTUM_SECURITY_FLAG_ACCESS_CONTROL);
    
    /* Test policy checking */
    struct quantum_security_policy policy = {
        .min_security_level = QUANTUM_SECURITY_LEVEL_STANDARD,
        .required_flags = QUANTUM_SECURITY_FLAG_ACCESS_CONTROL
    };
    ret = quantum_security_check_policy(ctx, &policy);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Cleanup */
    quantum_security_put_context(ctx);
}

/* Test security operations */
static void test_security_operations(struct kunit *test)
{
    struct quantum_security_context *ctx;
    int ret;
    
    ctx = quantum_security_create_context(QUANTUM_SECURITY_LEVEL_STANDARD,
                                        QUANTUM_SECURITY_FLAG_ENCRYPTION |
                                        QUANTUM_SECURITY_FLAG_AUTHENTICATION |
                                        QUANTUM_SECURITY_FLAG_ACCESS_CONTROL |
                                        QUANTUM_SECURITY_FLAG_AUDIT_LOG |
                                        QUANTUM_SECURITY_FLAG_ISOLATION);
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);
    
    /* Test encryption initialization */
    ret = quantum_security_init_encryption(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test authentication initialization */
    ret = quantum_security_init_authentication(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test access control initialization */
    ret = quantum_security_init_access_control(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test audit logging initialization */
    ret = quantum_security_init_audit_log(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test isolation initialization */
    ret = quantum_security_init_isolation(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Cleanup */
    quantum_security_put_context(ctx);
}

/* Test performance context creation and management */
static void test_perf_context(struct kunit *test)
{
    struct quantum_perf_context *ctx;
    int ret;
    
    /* Test context creation */
    ctx = quantum_perf_create_context();
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);
    KUNIT_EXPECT_EQ(test, ctx->sampling_rate, CONFIG_QUANTUM_PERFORMANCE_SAMPLING_RATE);
    
    /* Test sampling start */
    ret = quantum_perf_start_sampling(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_TRUE(test, ctx->sampling_active);
    
    /* Test sampling stop */
    ret = quantum_perf_stop_sampling(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_FALSE(test, ctx->sampling_active);
    
    /* Cleanup */
    quantum_perf_put_context(ctx);
}

/* Test performance metrics */
static void test_perf_metrics(struct kunit *test)
{
    struct quantum_perf_metric *metric;
    unsigned long value;
    int ret;
    
    /* Test metric creation */
    metric = quantum_perf_create_metric(QUANTUM_PERF_METRIC_CPU);
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, metric);
    KUNIT_EXPECT_EQ(test, metric->type, QUANTUM_PERF_METRIC_CPU);
    
    /* Test metric update */
    ret = quantum_perf_update_metric(metric, 75);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, metric->value, 75);
    
    /* Test threshold setting */
    ret = quantum_perf_set_threshold(metric, QUANTUM_PERF_THRESHOLD_WARNING);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, metric->threshold, QUANTUM_PERF_THRESHOLD_WARNING);
    
    /* Test metric retrieval */
    ret = quantum_perf_get_metric(metric, &value);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, value, 75);
    
    /* Cleanup */
    kfree(metric);
}

/* Test performance statistics */
static void test_perf_stats(struct kunit *test)
{
    struct quantum_perf_stats stats;
    int ret;
    
    /* Test stats initialization */
    ret = quantum_perf_stats_init(&stats);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Test stats update */
    ret = quantum_perf_stats_update(&stats, 1, 100);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, atomic_read(&stats.successful_operations), 1);
    
    /* Test stats reset */
    ret = quantum_perf_stats_reset(&stats);
    KUNIT_EXPECT_EQ(test, ret, 0);
    KUNIT_EXPECT_EQ(test, atomic_read(&stats.successful_operations), 0);
}

/* Test suite definition */
static struct kunit_case security_perf_test_cases[] = {
    KUNIT_CASE(test_security_context),
    KUNIT_CASE(test_security_operations),
    KUNIT_CASE(test_perf_context),
    KUNIT_CASE(test_perf_metrics),
    KUNIT_CASE(test_perf_stats),
    {}
};

static struct kunit_suite security_perf_test_suite = {
    .name = "security_perf_tests",
    .test_cases = security_perf_test_cases,
};

kunit_test_suite(security_perf_test_suite);

/* Module initialization */
static int __init test_init(void)
{
    return kunit_run_all_tests();
}

/* Module cleanup */
static void __exit test_exit(void)
{
    /* Nothing to clean up */
}

module_init(test_init);
module_exit(test_exit); 