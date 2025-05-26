#ifndef _QUANTUM_CLASSICAL_H
#define _QUANTUM_CLASSICAL_H

#include <linux/types.h>
#include "quantum.h"

/* Quantum-classical interface statistics */
struct quantum_classical_stats {
    atomic_t conversion_count;
    atomic_t measurement_count;
};

/* Interface buffer sizes */
#define QC_BUFFER_SIZE_MIN 64    /* Minimum buffer size (64 bytes) */
#define QC_BUFFER_SIZE_DEF 1024  /* Default buffer size (1KB) */
#define QC_BUFFER_SIZE_MAX 8192  /* Maximum buffer size (8KB) */

/* Conversion flags */
#define QC_CONV_NONE      0x00
#define QC_CONV_SUPERPOS  0x01  /* Create superposition state */
#define QC_CONV_ENTANGLE  0x02  /* Create entangled state */
#define QC_CONV_ERROR_COR 0x04  /* Apply error correction */

/* Measurement types */
#define QC_MEAS_NONE      0x00
#define QC_MEAS_BASIS_Z   0x01  /* Measure in Z basis */
#define QC_MEAS_BASIS_X   0x02  /* Measure in X basis */
#define QC_MEAS_BASIS_Y   0x04  /* Measure in Y basis */
#define QC_MEAS_BELL      0x08  /* Perform Bell measurement */

/* Interface parameters */
struct qc_params {
    size_t buffer_size;
    unsigned int conversion_flags;
    unsigned int measurement_type;
    bool error_correction;
};

/* Initialize quantum-classical interface */
int ctrlxt_qc_init(void);

/* Convert classical data to quantum state */
int ctrlxt_qc_classical_to_quantum(const void *data, size_t size);

/* Convert quantum state to classical data */
int ctrlxt_qc_quantum_to_classical(void *data, size_t size);

/* Apply quantum operation with classical control */
int ctrlxt_qc_controlled_operation(enum quantum_gate_type gate, int qubit, const void *control_data);

/* Get interface statistics */
void ctrlxt_qc_get_stats(struct quantum_classical_stats *stats);

/* Set interface parameters */
int ctrlxt_qc_set_params(struct qc_params *params);

/* Get current interface parameters */
int ctrlxt_qc_get_params(struct qc_params *params);

/* Reset interface statistics */
void ctrlxt_qc_reset_stats(void);

/* Enable/disable error correction */
int ctrlxt_qc_set_error_correction(bool enable);

/* Get interface status */
bool ctrlxt_qc_is_active(void);

/* Get current buffer size */
size_t ctrlxt_qc_get_buffer_size(void);

/* Set buffer size */
int ctrlxt_qc_set_buffer_size(size_t size);

/* Get conversion flags */
unsigned int ctrlxt_qc_get_conversion_flags(void);

/* Set conversion flags */
int ctrlxt_qc_set_conversion_flags(unsigned int flags);

/* Get measurement type */
unsigned int ctrlxt_qc_get_measurement_type(void);

/* Set measurement type */
int ctrlxt_qc_set_measurement_type(unsigned int type);

#endif /* _QUANTUM_CLASSICAL_H */ 