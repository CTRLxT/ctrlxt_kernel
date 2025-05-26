#ifndef _MEMORY_H
#define _MEMORY_H

#include <linux/types.h>

/* Memory region flags */
#define MEMORY_REGION_NORMAL    0x00000001
#define MEMORY_REGION_QUANTUM   0x00000002
#define MEMORY_REGION_HIGHMEM   0x00000004
#define MEMORY_REGION_DMA       0x00000008

/* Quantum memory flags */
#define QUANTUM_MEMORY_COHERENT     0x00000001
#define QUANTUM_MEMORY_ENTANGLED    0x00000002
#define QUANTUM_MEMORY_ERROR_CORRECT 0x00000004

/* Memory management functions */
int ctrlxt_mm_init(void);

/* Quantum memory functions */
void *quantum_memory_alloc(size_t size);
void quantum_memory_free(void *addr, size_t size);

/* Memory region functions */
int register_memory_region(unsigned long start, unsigned long end, unsigned long flags);
void unregister_memory_region(unsigned long start);

/* Memory statistics */
unsigned long get_total_memory(void);
unsigned long get_free_memory(void);
unsigned long get_quantum_memory(void);

/* Memory protection */
int protect_memory_region(unsigned long start, unsigned long end, unsigned long flags);
int unprotect_memory_region(unsigned long start, unsigned long end);

/* Memory mapping */
void *map_memory_region(unsigned long start, size_t size);
void unmap_memory_region(void *addr, size_t size);

/* Memory synchronization */
void sync_memory_region(unsigned long start, size_t size);
void sync_quantum_memory(unsigned long start, size_t size);

#endif /* _MEMORY_H */ 