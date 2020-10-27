/*
 * linux/v2x.h
 *
 * Definitions for V2X network layer (socket addr / AID)
 *
 * Authors: Haikun Wang<wanghaikun@gohigh.com.cn>
 *
 * Copyright (c) 2020-2025 Gohigh Co., Ltd
 *
 * All rights reserved.
 *
 */

/*
 * Release notes
 * Init version, Haikun Wang<wanghaikun@ghrdc.com.cn>, 2020/04/10
 * AF_V2X=42, Haikun Wang<wanghaikun@ghrdc.com.cn>, 2020/04/11
 */

#ifndef _UAPI_LINUX_V2X_H
#define _UAPI_LINUX_V2X_H

#include <linux/types.h>
#include <linux/socket.h>

/* Address family */
#ifndef AF_V2X
#define AF_V2X                          42
#endif

/* Protocol family */
#ifndef PF_V2X
#define PF_V2X                          AF_V2X
#endif

/* Protocols definitions */
#define V2XPROTO_ADAP                   0 /* adaptation layer */
#define V2XPROTO_ACCESS                 1 /* access layer */
#define V2XPROTO_DSMP                   2 /* dsmp layer */

/* AID bytes length */
#define AID_BYTES                       (sizeof(__u64))
/* Options length in socakaddr */
#define OPTIONS_LENGTH                  96 

/* Special value to identify NON-DSMP transmit */
#define SLONGLONG_NONDSMP_AID_VALUE     ((__s64)-1)

/* Options detail definitions in sockaddr */
/* options[0] control whether bind to a NON-AID mode */
#define POS_NONAID                      0

/*
 * AID in union
 * --------------------------------------
 * AID definitions in specs by 2020/04
 * Big endian
 *   Byte0     Length   Ranges
 * bit7 bit6
 *  0    x        1     [00~0x7f] 
 *  1    0        2     [0x8000~0xbfff] 
 *  1    1        >=3   Reserved
 * --------------------------------------
 * Usages:
 * Use a AID which length is one byte
 * aid.byte = 0x12
 * Use a AID which length is two bytes
 * aid.be_word = htons(0x1234)
 * Use special AID to identify NON-DSMP
 * aid.be_slonglong = SLONGLONG_NONDSMP_AID_VALUE
 * SLONGLONG_NONDSMP_AID_VALUE also uses as a invalid
 * AID when you get AID from the kernel
 */
union aid
{
    __u8 bytes[AID_BYTES];
    __u8 byte;
    __u16 be_word;
    __u32 be_dword;
    __s64 be_slonglong;
    __u64 be_ulonglong;
};

/*
 * additional informations when rx a message
 * by calling recvfrom
 */
struct v2x_adap_ind_addl
{
    __u64 utc;
    __u32 psn;
};

/*
 * V2X network layer address definition
 * @family, AF_V2X
 * @ifindex, ifindex of a bridge netdevice
 * @aid, AID union
 * @options, for bind or tx, control xmit type
 *           for rx, return the additional informations
 * max size limit, see struct __kernel_sockaddr_storage 
 */
struct sockaddr_v2x
{
	__kernel_sa_family_t family;
	int ifindex;
	union aid aid;
	__u8 options[OPTIONS_LENGTH];
};

#endif
