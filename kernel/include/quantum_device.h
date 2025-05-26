#ifndef _QUANTUM_DEVICE_H
#define _QUANTUM_DEVICE_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include "quantum.h"
#include "quantum_memory.h"

/* IOCTL commands */
#define QUANTUM_IOC_MAGIC 'q'

#define QUANTUM_IOCTL_INIT        _IOW(QUANTUM_IOC_MAGIC, 1, int)
#define QUANTUM_IOCTL_MEASURE     _IOR(QUANTUM_IOC_MAGIC, 2, void *)
#define QUANTUM_IOCTL_APPLY_GATE  _IOWR(QUANTUM_IOC_MAGIC, 3, struct quantum_gate_params)
#define QUANTUM_IOCTL_ALLOC_MEMORY _IOWR(QUANTUM_IOC_MAGIC, 4, struct quantum_memory_params)
#define QUANTUM_IOCTL_FREE_MEMORY _IO(QUANTUM_IOC_MAGIC, 5)
#define QUANTUM_IOCTL_GET_STATS   _IOR(QUANTUM_IOC_MAGIC, 6, struct quantum_device_stats)
#define QUANTUM_IOCTL_SET_CAPS    _IOW(QUANTUM_IOC_MAGIC, 7, unsigned long)
#define QUANTUM_IOCTL_GET_CAPS    _IOR(QUANTUM_IOC_MAGIC, 8, unsigned long)

/* Device capabilities */
#define QUANTUM_CAP_NONE         0x00
#define QUANTUM_CAP_ERROR_COR    0x01  /* Error correction support */
#define QUANTUM_CAP_ENTANGLEMENT 0x02  /* Entanglement support */
#define QUANTUM_CAP_MEASUREMENT  0x04  /* Measurement support */
#define QUANTUM_CAP_GATES        0x08  /* Quantum gates support */
#define QUANTUM_CAP_MEMORY       0x10  /* Quantum memory support */

/* Quantum device parameters */
struct quantum_gate_params {
    enum quantum_gate_type gate;
    int qubit;
    void *params;
    size_t param_size;
};

struct quantum_memory_params {
    size_t num_qubits;
    unsigned long flags;
};

struct quantum_device_stats {
    atomic_t open_count;
    atomic_t operation_count;
    atomic_t error_count;
    atomic_t memory_usage;
    atomic_t gate_count;
    atomic_t measurement_count;
};

/* Device operations */
int ctrlxt_quantum_device_init(void);
void ctrlxt_quantum_device_exit(void);

/* Device management */
int ctrlxt_quantum_device_register(struct device *parent);
void ctrlxt_quantum_device_unregister(void);
int ctrlxt_quantum_device_open(void);
void ctrlxt_quantum_device_close(void);

/* Device operations */
int ctrlxt_quantum_device_init_state(int num_qubits);
int ctrlxt_quantum_device_measure(void *result);
int ctrlxt_quantum_device_apply_gate(enum quantum_gate_type gate, int qubit, void *params, size_t param_size);
int ctrlxt_quantum_device_alloc_memory(size_t num_qubits, unsigned long flags);
void ctrlxt_quantum_device_free_memory(void);

/* Device statistics */
void ctrlxt_quantum_device_get_stats(struct quantum_device_stats *stats);
void ctrlxt_quantum_device_reset_stats(void);

/* Device capabilities */
int ctrlxt_quantum_device_set_capabilities(unsigned long caps);
unsigned long ctrlxt_quantum_device_get_capabilities(void);
bool ctrlxt_quantum_device_has_capability(unsigned long cap);

/* Device state management */
struct quantum_state *ctrlxt_quantum_device_get_state(void);
int ctrlxt_quantum_device_set_state(struct quantum_state *state);
int ctrlxt_quantum_device_save_state(void *buffer, size_t size);
int ctrlxt_quantum_device_load_state(const void *buffer, size_t size);

/* Device error handling */
int ctrlxt_quantum_device_get_last_error(void);
const char *ctrlxt_quantum_device_get_error_string(int error);
void ctrlxt_quantum_device_clear_errors(void);

/* Device synchronization */
int ctrlxt_quantum_device_lock(void);
void ctrlxt_quantum_device_unlock(void);
int ctrlxt_quantum_device_wait_for_operation(void);
int ctrlxt_quantum_device_signal_operation(void);

#endif /* _QUANTUM_DEVICE_H */ 