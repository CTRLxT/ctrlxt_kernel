#ifndef _FS_H
#define _FS_H

#include <linux/types.h>
#include <linux/fs.h>

/* File system type */
extern struct file_system_type ctrlxt_fs_type;

/* File system operations */
extern struct super_operations ctrlxt_super_ops;
extern struct inode_operations ctrlxt_inode_ops;
extern struct file_operations ctrlxt_file_ops;

/* File system initialization */
int ctrlxt_fs_init(void);

/* Quantum file operations */
int create_quantum_file(const char *name, size_t size);
int remove_quantum_file(const char *name);

/* File system statistics */
unsigned long get_inode_count(void);
unsigned long get_dentry_count(void);

/* File operations */
int quantum_file_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
int quantum_file_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
int quantum_file_open(struct inode *inode, struct file *file);
int quantum_file_release(struct inode *inode, struct file *file);

/* Directory operations */
int create_quantum_dir(const char *name);
int remove_quantum_dir(const char *name);
int list_quantum_dir(const char *name, struct dir_context *ctx);

/* File system mount operations */
int mount_quantum_fs(const char *dev_name, const char *dir_name, const char *type);
int umount_quantum_fs(const char *dir_name);

/* File system synchronization */
void sync_quantum_fs(void);
void sync_quantum_file(const char *name);

/* File system permissions */
int set_quantum_file_perms(const char *name, mode_t mode);
int get_quantum_file_perms(const char *name, mode_t *mode);

#endif /* _FS_H */ 