#ifndef _QUANTUM_ERROR_H
#define _QUANTUM_ERROR_H

#include <linux/types.h>
#include "quantum.h"

/* Quantum error correction statistics */
struct quantum_error_stats {
    atomic_t error_count;
    atomic_t correction_count;
    unsigned int success_rate;
};

/* Quantum error correction initialization */
int ctrlxt_qec_init(void);

/* Apply quantum error correction to a quantum state */
int ctrlxt_qec_apply(struct quantum_state *state);

/* Get quantum error correction statistics */
void ctrlxt_qec_get_stats(struct quantum_error_stats *stats);

/* Error correction codes */
#define QEC_CODE_NONE    0
#define QEC_CODE_BITFLIP 1
#define QEC_CODE_PHASEFLIP 2
#define QEC_CODE_SHOR    3
#define QEC_CODE_SURFACE 4

/* Error types */
#define QEC_ERROR_NONE   0
#define QEC_ERROR_X      1  /* Bit flip */
#define QEC_ERROR_Z      2  /* Phase flip */
#define QEC_ERROR_Y      3  /* Both bit and phase flip */

/* Syndrome measurement results */
#define SYNDROME_NONE    0
#define SYNDROME_X1      1  /* Bit flip on qubit 0 */
#define SYNDROME_X2      2  /* Bit flip on qubit 1 */
#define SYNDROME_X3      3  /* Bit flip on qubit 2 */
#define SYNDROME_Z1      4  /* Phase flip on qubit 0 */
#define SYNDROME_Z2      5  /* Phase flip on qubit 1 */
#define SYNDROME_Z3      6  /* Phase flip on qubit 2 */
#define SYNDROME_Y1      7  /* Both bit and phase flip */

/* Error correction thresholds */
#define QEC_THRESHOLD_MIN 0.85  /* Minimum success rate threshold */
#define QEC_THRESHOLD_MAX 0.99  /* Maximum success rate threshold */

/* Error correction parameters */
struct qec_params {
    unsigned int code_type;
    unsigned int error_threshold;
    unsigned int max_iterations;
    bool adaptive_correction;
};

/* Set error correction parameters */
int ctrlxt_qec_set_params(struct qec_params *params);

/* Get current error correction parameters */
int ctrlxt_qec_get_params(struct qec_params *params);

/* Reset error correction statistics */
void ctrlxt_qec_reset_stats(void);

/* Enable/disable adaptive error correction */
int ctrlxt_qec_set_adaptive(bool enable);

/* Get error correction status */
bool ctrlxt_qec_is_active(void);

/* Get current error correction code type */
unsigned int ctrlxt_qec_get_code_type(void);

/* Set error correction code type */
int ctrlxt_qec_set_code_type(unsigned int code_type);

#endif /* _QUANTUM_ERROR_H */ 