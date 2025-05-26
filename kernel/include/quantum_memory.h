#ifndef _QUANTUM_MEMORY_H
#define _QUANTUM_MEMORY_H

#include <linux/types.h>
#include "quantum.h"

/* Memory block flags */
#define QMEM_FLAG_NONE        0x00
#define QMEM_FLAG_ERROR_COR   0x01  /* Enable error correction */
#define QMEM_FLAG_ENTANGLED   0x02  /* Block contains entangled states */
#define QMEM_FLAG_PERSISTENT  0x04  /* Block should persist across operations */
#define QMEM_FLAG_SHARED      0x08  /* Block can be shared between processes */

/* Memory pool sizes */
#define QMEM_POOL_SIZE        (1024 * 1024)  /* 1MB memory pool */
#define QMEM_MAX_QUBITS       1024           /* Maximum total qubits */
#define QMEM_MAX_QUBITS_PER_BLOCK 64         /* Maximum qubits per block */

/* Quantum memory block structure */
struct quantum_memory_block {
    struct list_head list;
    struct quantum_state *state;
    size_t size;
    unsigned long flags;
    atomic_t ref_count;
    void *private_data;
};

/* Quantum memory statistics */
struct quantum_memory_stats {
    atomic_t total_qubits;
    atomic_t allocated_qubits;
    atomic_t max_qubits;
    atomic_t free_qubits;
};

/* Initialize quantum memory manager */
int ctrlxt_qmem_init(void);

/* Allocate quantum memory block */
struct quantum_memory_block *ctrlxt_qmem_alloc(size_t num_qubits, unsigned long flags);

/* Free quantum memory block */
void ctrlxt_qmem_free(struct quantum_memory_block *block);

/* Get quantum memory statistics */
void ctrlxt_qmem_get_stats(struct quantum_memory_stats *stats);

/* Set maximum number of qubits */
int ctrlxt_qmem_set_max_qubits(unsigned long max_qubits);

/* Get memory block reference */
struct quantum_memory_block *ctrlxt_qmem_get(struct quantum_memory_block *block);

/* Put memory block reference */
void ctrlxt_qmem_put(struct quantum_memory_block *block);

/* Memory block operations */
int ctrlxt_qmem_block_init(struct quantum_memory_block *block, int initial_state);
int ctrlxt_qmem_block_measure(struct quantum_memory_block *block, void *result);
int ctrlxt_qmem_block_apply_gate(struct quantum_memory_block *block, enum quantum_gate_type gate, int qubit);
int ctrlxt_qmem_block_entangle(struct quantum_memory_block *block1, struct quantum_memory_block *block2);

/* Memory pool operations */
int ctrlxt_qmem_pool_resize(size_t new_size);
void ctrlxt_qmem_pool_clear(void);
size_t ctrlxt_qmem_pool_get_size(void);
size_t ctrlxt_qmem_pool_get_free(void);

/* Memory block flags operations */
unsigned long ctrlxt_qmem_block_get_flags(struct quantum_memory_block *block);
int ctrlxt_qmem_block_set_flags(struct quantum_memory_block *block, unsigned long flags);
int ctrlxt_qmem_block_clear_flags(struct quantum_memory_block *block, unsigned long flags);

/* Memory block state operations */
struct quantum_state *ctrlxt_qmem_block_get_state(struct quantum_memory_block *block);
int ctrlxt_qmem_block_set_state(struct quantum_memory_block *block, struct quantum_state *state);
int ctrlxt_qmem_block_copy_state(struct quantum_memory_block *dst, struct quantum_memory_block *src);

/* Memory block reference counting */
int ctrlxt_qmem_block_get_ref_count(struct quantum_memory_block *block);
bool ctrlxt_qmem_block_is_shared(struct quantum_memory_block *block);

#endif /* _QUANTUM_MEMORY_H */ 