#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include "../include/performance.h"
#include "../include/quantum.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CTRLxT_STUDIOS");
MODULE_DESCRIPTION("Quantum Performance Monitoring Module");
MODULE_VERSION("0.1.0");

/* Performance context management */
static DEFINE_SPINLOCK(perf_lock);
static struct quantum_perf_context *global_context;

/* Performance statistics */
static struct quantum_perf_stats global_stats;

/* Initialize performance monitoring */
int quantum_perf_init(void)
{
    struct quantum_perf_context *ctx;
    
    ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;
    
    ctx->ref_count = 1;
    ctx->sampling_rate = CONFIG_QUANTUM_PERFORMANCE_SAMPLING_RATE;
    
    spin_lock(&perf_lock);
    global_context = ctx;
    spin_unlock(&perf_lock);
    
    pr_info("CTRLxT_STUDIOS: Performance monitoring initialized\n");
    return 0;
}

/* Cleanup performance monitoring */
void quantum_perf_exit(void)
{
    struct quantum_perf_context *ctx;
    
    spin_lock(&perf_lock);
    ctx = global_context;
    global_context = NULL;
    spin_unlock(&perf_lock);
    
    if (ctx) {
        if (ctx->private_data)
            kfree(ctx->private_data);
        kfree(ctx);
    }
    
    pr_info("CTRLxT_STUDIOS: Performance monitoring cleaned up\n");
}

/* Create performance context */
struct quantum_perf_context *quantum_perf_create_context(void)
{
    struct quantum_perf_context *ctx;
    
    ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
    if (!ctx)
        return ERR_PTR(-ENOMEM);
    
    ctx->ref_count = 1;
    ctx->sampling_rate = CONFIG_QUANTUM_PERFORMANCE_SAMPLING_RATE;
    
    return ctx;
}

/* Get performance context */
struct quantum_perf_context *quantum_perf_get_context(void)
{
    struct quantum_perf_context *ctx;
    
    spin_lock(&perf_lock);
    ctx = global_context;
    if (ctx)
        ctx->ref_count++;
    spin_unlock(&perf_lock);
    
    return ctx;
}

/* Put performance context */
void quantum_perf_put_context(struct quantum_perf_context *ctx)
{
    if (!ctx)
        return;
    
    spin_lock(&perf_lock);
    if (--ctx->ref_count == 0) {
        if (ctx->private_data)
            kfree(ctx->private_data);
        kfree(ctx);
    }
    spin_unlock(&perf_lock);
}

/* Create performance metric */
struct quantum_perf_metric *quantum_perf_create_metric(
    enum quantum_perf_metric_type type)
{
    struct quantum_perf_metric *metric;
    
    if (type < QUANTUM_PERF_METRIC_CPU ||
        type > QUANTUM_PERF_METRIC_IO)
        return ERR_PTR(-EINVAL);
    
    metric = kzalloc(sizeof(*metric), GFP_KERNEL);
    if (!metric)
        return ERR_PTR(-ENOMEM);
    
    metric->type = type;
    metric->threshold = QUANTUM_PERF_THRESHOLD_NORMAL;
    metric->timestamp = ktime_get();
    
    return metric;
}

/* Update performance metric */
int quantum_perf_update_metric(struct quantum_perf_metric *metric,
                             unsigned long value)
{
    if (!metric)
        return -EINVAL;
    
    metric->value = value;
    metric->timestamp = ktime_get();
    
    /* Update statistics */
    spin_lock(&perf_lock);
    global_stats.total_metrics++;
    global_stats.last_update = ktime_get();
    
    if (value > metric->threshold)
        global_stats.threshold_exceeded++;
    spin_unlock(&perf_lock);
    
    return 0;
}

/* Get performance metric */
int quantum_perf_get_metric(struct quantum_perf_metric *metric,
                          unsigned long *value)
{
    if (!metric || !value)
        return -EINVAL;
    
    *value = metric->value;
    return 0;
}

/* Set performance threshold */
int quantum_perf_set_threshold(struct quantum_perf_metric *metric,
                             unsigned long threshold)
{
    if (!metric)
        return -EINVAL;
    
    metric->threshold = threshold;
    return 0;
}

/* Get performance threshold */
int quantum_perf_get_threshold(struct quantum_perf_metric *metric,
                             unsigned long *threshold)
{
    if (!metric || !threshold)
        return -EINVAL;
    
    *threshold = metric->threshold;
    return 0;
}

/* Get performance statistics */
int quantum_perf_get_stats(struct quantum_perf_stats *stats)
{
    if (!stats)
        return -EINVAL;
    
    spin_lock(&perf_lock);
    memcpy(stats, &global_stats, sizeof(*stats));
    spin_unlock(&perf_lock);
    
    return 0;
}

/* Reset performance statistics */
int quantum_perf_reset_stats(void)
{
    spin_lock(&perf_lock);
    memset(&global_stats, 0, sizeof(global_stats));
    spin_unlock(&perf_lock);
    
    return 0;
}

/* Start performance sampling */
int quantum_perf_start_sampling(struct quantum_perf_context *ctx)
{
    if (!ctx)
        return -EINVAL;
    
    if (ctx->sampling_active)
        return -EBUSY;
    
    ctx->sampling_active = true;
    ctx->sampling_start = ktime_get();
    
    pr_info("CTRLxT_STUDIOS: Performance sampling started\n");
    return 0;
}

/* Stop performance sampling */
int quantum_perf_stop_sampling(struct quantum_perf_context *ctx)
{
    if (!ctx)
        return -EINVAL;
    
    if (!ctx->sampling_active)
        return -EINVAL;
    
    ctx->sampling_active = false;
    ctx->sampling_end = ktime_get();
    
    pr_info("CTRLxT_STUDIOS: Performance sampling stopped\n");
    return 0;
}

/* Get sampling duration */
int quantum_perf_get_sampling_duration(struct quantum_perf_context *ctx,
                                     ktime_t *duration)
{
    if (!ctx || !duration)
        return -EINVAL;
    
    if (!ctx->sampling_active)
        return -EINVAL;
    
    *duration = ktime_sub(ktime_get(), ctx->sampling_start);
    return 0;
}

/* Module initialization */
static int __init performance_init(void)
{
    int ret;
    
    ret = quantum_perf_init();
    if (ret)
        return ret;
    
    pr_info("CTRLxT_STUDIOS: Performance monitoring module loaded\n");
    return 0;
}

/* Module cleanup */
static void __exit performance_exit(void)
{
    quantum_perf_exit();
    pr_info("CTRLxT_STUDIOS: Performance monitoring module unloaded\n");
}

module_init(performance_init);
module_exit(performance_exit); 