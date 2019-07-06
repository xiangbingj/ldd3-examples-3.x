#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */
/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define SCULL_IOC_MAGIC  'k'
/* Please use a different 8-bit number in your code */

#define SCULL_P_IOCTSIZE _IO(SCULL_IOC_MAGIC,   0)
#define SCULL_P_IOCQSIZE _IO(SCULL_IOC_MAGIC,   1)