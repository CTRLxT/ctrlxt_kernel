#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kunit/test.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include "../include/performance.h"
#include "../include/quantum.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CTRLxT_STUDIOS");
MODULE_DESCRIPTION("Quantum Performance Benchmarking Tests");
MODULE_VERSION("0.1.0");

#define BENCHMARK_ITERATIONS 1000
#define BENCHMARK_DELAY_MS 1

/* Helper function to measure operation time */
static ktime_t measure_operation_time(void (*operation)(void *), void *arg)
{
    ktime_t start, end;
    
    start = ktime_get();
    operation(arg);
    end = ktime_get();
    
    return ktime_sub(end, start);
}

/* Benchmark context creation and destruction */
static void benchmark_context_ops(void *arg)
{
    struct quantum_perf_context *ctx = quantum_perf_create_context();
    if (ctx)
        quantum_perf_put_context(ctx);
}

static void test_context_creation_benchmark(struct kunit *test)
{
    ktime_t total_time = 0;
    int i;
    
    for (i = 0; i < BENCHMARK_ITERATIONS; i++) {
        total_time = ktime_add(total_time, measure_operation_time(benchmark_context_ops, NULL));
        msleep(BENCHMARK_DELAY_MS);
    }
    
    ktime_t avg_time = ktime_divns(total_time, BENCHMARK_ITERATIONS);
    pr_info("CTRLxT_STUDIOS: Context creation/destruction average time: %lld ns\n", avg_time);
    KUNIT_EXPECT_LT(test, avg_time, 1000000); /* Should be less than 1ms */
}

/* Benchmark metric operations */
static void benchmark_metric_ops(void *arg)
{
    struct quantum_perf_metric *metric = quantum_perf_create_metric(QUANTUM_PERF_METRIC_CPU);
    if (metric) {
        quantum_perf_update_metric(metric, 75);
        quantum_perf_set_threshold(metric, QUANTUM_PERF_THRESHOLD_WARNING);
        kfree(metric);
    }
}

static void test_metric_operations_benchmark(struct kunit *test)
{
    ktime_t total_time = 0;
    int i;
    
    for (i = 0; i < BENCHMARK_ITERATIONS; i++) {
        total_time = ktime_add(total_time, measure_operation_time(benchmark_metric_ops, NULL));
        msleep(BENCHMARK_DELAY_MS);
    }
    
    ktime_t avg_time = ktime_divns(total_time, BENCHMARK_ITERATIONS);
    pr_info("CTRLxT_STUDIOS: Metric operations average time: %lld ns\n", avg_time);
    KUNIT_EXPECT_LT(test, avg_time, 500000); /* Should be less than 500μs */
}

/* Benchmark sampling operations */
static void benchmark_sampling_ops(void *arg)
{
    struct quantum_perf_context *ctx = quantum_perf_create_context();
    if (ctx) {
        quantum_perf_start_sampling(ctx);
        msleep(1);
        quantum_perf_stop_sampling(ctx);
        quantum_perf_put_context(ctx);
    }
}

static void test_sampling_operations_benchmark(struct kunit *test)
{
    ktime_t total_time = 0;
    int i;
    
    for (i = 0; i < BENCHMARK_ITERATIONS; i++) {
        total_time = ktime_add(total_time, measure_operation_time(benchmark_sampling_ops, NULL));
        msleep(BENCHMARK_DELAY_MS);
    }
    
    ktime_t avg_time = ktime_divns(total_time, BENCHMARK_ITERATIONS);
    pr_info("CTRLxT_STUDIOS: Sampling operations average time: %lld ns\n", avg_time);
    KUNIT_EXPECT_LT(test, avg_time, 2000000); /* Should be less than 2ms */
}

/* Benchmark statistics operations */
static void benchmark_stats_ops(void *arg)
{
    struct quantum_perf_stats stats;
    quantum_perf_stats_init(&stats);
    quantum_perf_stats_update(&stats, 1, 100);
    quantum_perf_stats_reset(&stats);
}

static void test_stats_operations_benchmark(struct kunit *test)
{
    ktime_t total_time = 0;
    int i;
    
    for (i = 0; i < BENCHMARK_ITERATIONS; i++) {
        total_time = ktime_add(total_time, measure_operation_time(benchmark_stats_ops, NULL));
        msleep(BENCHMARK_DELAY_MS);
    }
    
    ktime_t avg_time = ktime_divns(total_time, BENCHMARK_ITERATIONS);
    pr_info("CTRLxT_STUDIOS: Statistics operations average time: %lld ns\n", avg_time);
    KUNIT_EXPECT_LT(test, avg_time, 300000); /* Should be less than 300μs */
}

/* Stress test for concurrent operations */
static void test_concurrent_operations(struct kunit *test)
{
    struct quantum_perf_context *ctx = quantum_perf_create_context();
    struct quantum_perf_metric *metrics[10];
    int i, ret;
    
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);
    
    /* Create multiple metrics */
    for (i = 0; i < 10; i++) {
        metrics[i] = quantum_perf_create_metric(QUANTUM_PERF_METRIC_CPU);
        KUNIT_ASSERT_NOT_ERR_OR_NULL(test, metrics[i]);
    }
    
    /* Start sampling */
    ret = quantum_perf_start_sampling(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Update metrics concurrently */
    for (i = 0; i < 10; i++) {
        ret = quantum_perf_update_metric(metrics[i], 75 + i);
        KUNIT_EXPECT_EQ(test, ret, 0);
    }
    
    /* Stop sampling */
    ret = quantum_perf_stop_sampling(ctx);
    KUNIT_EXPECT_EQ(test, ret, 0);
    
    /* Cleanup */
    for (i = 0; i < 10; i++) {
        kfree(metrics[i]);
    }
    quantum_perf_put_context(ctx);
}

/* Memory usage benchmark */
static void test_memory_usage(struct kunit *test)
{
    struct quantum_perf_context *ctx;
    struct quantum_perf_metric *metric;
    size_t initial_mem, final_mem;
    
    /* Get initial memory usage */
    initial_mem = get_memory_usage();
    
    /* Create and use resources */
    ctx = quantum_perf_create_context();
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);
    
    metric = quantum_perf_create_metric(QUANTUM_PERF_METRIC_MEMORY);
    KUNIT_ASSERT_NOT_ERR_OR_NULL(test, metric);
    
    /* Perform operations */
    quantum_perf_start_sampling(ctx);
    quantum_perf_update_metric(metric, 50);
    quantum_perf_stop_sampling(ctx);
    
    /* Get final memory usage */
    final_mem = get_memory_usage();
    
    /* Cleanup */
    kfree(metric);
    quantum_perf_put_context(ctx);
    
    /* Verify memory usage */
    KUNIT_EXPECT_LT(test, final_mem - initial_mem, 1024 * 1024); /* Should use less than 1MB */
}

/* Test suite definition */
static struct kunit_case perf_benchmark_test_cases[] = {
    KUNIT_CASE(test_context_creation_benchmark),
    KUNIT_CASE(test_metric_operations_benchmark),
    KUNIT_CASE(test_sampling_operations_benchmark),
    KUNIT_CASE(test_stats_operations_benchmark),
    KUNIT_CASE(test_concurrent_operations),
    KUNIT_CASE(test_memory_usage),
    {}
};

static struct kunit_suite perf_benchmark_test_suite = {
    .name = "perf_benchmark_tests",
    .test_cases = perf_benchmark_test_cases,
};

kunit_test_suite(perf_benchmark_test_suite);

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