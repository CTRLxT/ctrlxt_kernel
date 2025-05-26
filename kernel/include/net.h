#ifndef _NET_H
#define _NET_H

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/in.h>

/* Network device operations */
extern const struct net_device_ops ctrlxt_netdev_ops;

/* Network initialization */
int ctrlxt_net_init(void);

/* Quantum networking functions */
int init_quantum_networking(void);
int create_quantum_connection(struct socket *sock);
void close_quantum_connection(struct socket *sock);

/* Network device operations */
int ctrlxt_net_open(struct net_device *dev);
int ctrlxt_net_stop(struct net_device *dev);
netdev_tx_t ctrlxt_net_xmit(struct sk_buff *skb, struct net_device *dev);
struct net_device_stats *ctrlxt_net_get_stats(struct net_device *dev);
int ctrlxt_net_set_mac(struct net_device *dev, void *addr);

/* Packet processing */
int process_quantum_packet(struct sk_buff *skb);
int process_classical_packet(struct sk_buff *skb);

/* Network statistics */
unsigned long get_quantum_connections(void);
unsigned long get_classical_connections(void);
unsigned long get_total_packets(void);
unsigned long get_total_bytes(void);

/* Network configuration */
int set_quantum_network_params(struct net_device *dev, void *params);
int get_quantum_network_params(struct net_device *dev, void *params);

/* Network synchronization */
void sync_quantum_network(void);
void sync_classical_network(void);

/* Network security */
int enable_quantum_encryption(struct socket *sock);
int disable_quantum_encryption(struct socket *sock);
int set_quantum_security_level(struct socket *sock, int level);

#endif /* _NET_H */ 