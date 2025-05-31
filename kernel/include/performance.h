#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#include <linux/types.h>
#include <linux/jiffies.h>
#include "config.h"

/* Performance metrics */
#define QUANTUM_PERF_METRIC_CPU 0
#define QUANTUM_PERF_METRIC_MEMORY 1
#define QUANTUM_PERF_METRIC_NETWORK 2
#define QUANTUM_PERF_METRIC_QUANTUM 3
#define QUANTUM_PERF_METRIC_IO 4

/* Performance thresholds */
#define QUANTUM_PERF_THRESHOLD_NORMAL 80
#define QUANTUM_PERF_THRESHOLD_WARNING 90
#define QUANTUM_PERF_THRESHOLD_CRITICAL 95

/* Performance structures */
struct quantum_perf_metric {
    unsigned int type;
    unsigned long value;
    unsigned long threshold;
    unsigned long timestamp;
};

struct quantum_perf_stats {
    atomic_t total_operations;
    atomic_t successful_operations;
    atomic_t failed_operations;
    atomic_t total_time;
    atomic_t max_time;
    atomic_t min_time;
};

struct quantum_perf_context {
    struct quantum_perf_stats stats;
    unsigned long start_time;
    unsigned long end_time;
    unsigned int flags;
    void *private_data;
};

/* Performance operations */
int quantum_perf_init(void);
void quantum_perf_exit(void);

/* Metric management */
int quantum_perf_metric_create(struct quantum_perf_metric *metric);
int quantum_perf_metric_update(struct quantum_perf_metric *metric, unsigned long value);
int quantum_perf_metric_check(struct quantum_perf_metric *metric);

/* Statistics management */
int quantum_perf_stats_init(struct quantum_perf_stats *stats);
int quantum_perf_stats_update(struct quantum_perf_stats *stats, int success, unsigned long time);
int quantum_perf_stats_reset(struct quantum_perf_stats *stats);

/* Context management */
struct quantum_perf_context *quantum_perf_context_create(unsigned int flags);
void quantum_perf_context_destroy(struct quantum_perf_context *ctx);
int quantum_perf_context_start(struct quantum_perf_context *ctx);
int quantum_perf_context_stop(struct quantum_perf_context *ctx);

/* Monitoring */
int quantum_perf_monitor_start(void);
int quantum_perf_monitor_stop(void);
int quantum_perf_monitor_pause(void);
int quantum_perf_monitor_resume(void);

/* Reporting */
int quantum_perf_report_generate(void *buffer, size_t size);
int quantum_perf_report_save(const char *filename);
int quantum_perf_report_load(const char *filename);

/* Analysis */
int quantum_perf_analyze_metrics(struct quantum_perf_metric *metrics, size_t count);
int quantum_perf_analyze_stats(struct quantum_perf_stats *stats);
int quantum_perf_analyze_context(struct quantum_perf_context *ctx);

/* Optimization */
int quantum_perf_optimize_metric(struct quantum_perf_metric *metric);
int quantum_perf_optimize_stats(struct quantum_perf_stats *stats);
int quantum_perf_optimize_context(struct quantum_perf_context *ctx);

/* Error handling */
int quantum_perf_get_last_error(void);
const char *quantum_perf_get_error_string(int error);
void quantum_perf_clear_errors(void);

#endif /* _PERFORMANCE_H */ 