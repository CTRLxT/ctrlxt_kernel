#ifndef _SECURITY_H
#define _SECURITY_H

#include <linux/types.h>
#include "config.h"

/* Security levels */
#define QUANTUM_SECURITY_LEVEL_NONE    0
#define QUANTUM_SECURITY_LEVEL_BASIC   1
#define QUANTUM_SECURITY_LEVEL_STANDARD 2
#define QUANTUM_SECURITY_LEVEL_HIGH    3
#define QUANTUM_SECURITY_LEVEL_MAXIMUM 4

/* Security flags */
#define QUANTUM_SECURITY_FLAG_NONE     0x00
#define QUANTUM_SECURITY_FLAG_ENCRYPT  0x01
#define QUANTUM_SECURITY_FLAG_AUTH     0x02
#define QUANTUM_SECURITY_FLAG_ACCESS   0x04
#define QUANTUM_SECURITY_FLAG_AUDIT    0x08
#define QUANTUM_SECURITY_FLAG_ISOLATE  0x10

/* Security structures */
struct quantum_security_context {
    unsigned int level;
    unsigned long flags;
    void *private_data;
    atomic_t ref_count;
};

struct quantum_security_policy {
    unsigned int min_level;
    unsigned long required_flags;
    unsigned long optional_flags;
};

/* Security operations */
int quantum_security_init(void);
void quantum_security_exit(void);

/* Context management */
struct quantum_security_context *quantum_security_context_create(unsigned int level, unsigned long flags);
void quantum_security_context_destroy(struct quantum_security_context *ctx);
int quantum_security_context_set_level(struct quantum_security_context *ctx, unsigned int level);
int quantum_security_context_set_flags(struct quantum_security_context *ctx, unsigned long flags);

/* Policy management */
int quantum_security_policy_create(struct quantum_security_policy *policy);
int quantum_security_policy_validate(struct quantum_security_context *ctx, struct quantum_security_policy *policy);
int quantum_security_policy_enforce(struct quantum_security_context *ctx, struct quantum_security_policy *policy);

/* Encryption */
int quantum_security_encrypt(void *data, size_t size, struct quantum_security_context *ctx);
int quantum_security_decrypt(void *data, size_t size, struct quantum_security_context *ctx);

/* Authentication */
int quantum_security_authenticate(void *data, size_t size, struct quantum_security_context *ctx);
int quantum_security_verify(void *data, size_t size, struct quantum_security_context *ctx);

/* Access control */
int quantum_security_check_access(void *resource, struct quantum_security_context *ctx);
int quantum_security_grant_access(void *resource, struct quantum_security_context *ctx);
int quantum_security_revoke_access(void *resource, struct quantum_security_context *ctx);

/* Audit logging */
int quantum_security_audit_log(const char *action, struct quantum_security_context *ctx);
int quantum_security_audit_query(const char *filter, void *result, size_t size);

/* Isolation */
int quantum_security_isolate(struct quantum_security_context *ctx);
int quantum_security_deisolate(struct quantum_security_context *ctx);

/* Error handling */
int quantum_security_get_last_error(void);
const char *quantum_security_get_error_string(int error);
void quantum_security_clear_errors(void);

#endif /* _SECURITY_H */ 