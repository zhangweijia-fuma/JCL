#ifndef _V2X_IOCTL_H
#define _V2X_IOCTL_H

/*
 * Release nodes
 * Init version, Haikun Wang <wanghaikun@ghrdc.com.cn>, 2020/04/10
 */

#include <linux/sockios.h>

struct register_aids {
    int cnt;
    int aids[0];
};
/*
 * Ioctl commands definitions
 */
/* register legal AIDs, arg: struct register_aids * */
#define REGAIDS             SIOCPROTOPRIVATE
/* get the count of registered legal AIDs, arg: struct register_aids * */
#define GETREGDAIDSCNT      (SIOCPROTOPRIVATE+1)
/* get registered legal AIDs, arg: struct register_aids * */
#define GETREGDAIDS         (SIOCPROTOPRIVATE+2)
/* get the PSN of the last packet send by the calling socket, args: unsigned int * */
/* the PSN of the last queued packet, the packet maybe not xmit to modem  */
#define GETLASTPSN          (SIOCPROTOPRIVATE+3)
/* get the UTC of the last packet send by the calling socket, args: unsigned long long *, unit: us  */
/* the UTC of the last queued packet, the packet maybe not xmit to modem  */
#define GETLASTUTC          (SIOCPROTOPRIVATE+4)
/* get the PSN of the last packet sent by the calling socket, args: unsigned int * */
/* the PSN of the last xmited packet, the packet has been xmited to modem  */
#define GETLASTXMITEDPSN          (SIOCPROTOPRIVATE+5)
/* get the UTC of the last packet sent by the calling socket, args: unsigned long long *, unit: us  */
/* the UTC of the last xmited packet, the packet has been xmited to modem  */
#define GETLASTXMITEDUTC          (SIOCPROTOPRIVATE+6)
/* get the next PSN, global PSN when multi sockets exist, args: unsigned int * */
#define GETNEXTPSN          (SIOCPROTOPRIVATE+7)
/* get the next PSN of the calling socket, args: unsigned int * */
#define GETNEXTSOCKPSN          (SIOCPROTOPRIVATE+8)
/* get the delay between af_v2x and bridge of the last xmited packet  of the calling socket, args: unsigned int *()[2] */
#define GETLASTPSNDELAY            (SIOCPROTOPRIVATE+9)

#endif
