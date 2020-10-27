#ifndef _UAPI_LINUX_DSMP_H
#define _UAPI_LINUX_DSMP_H

#include <linux/types.h>
#include <asm/byteorder.h>

struct dsmphhdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
    __u8 reserved:4;
         extent_indicator:1;
         version:3;
#elif defined (__BIG_ENDIAN_BITFIELD)
    __u8 version:3;
         extent_indicator:1;
         reserved:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
};

struct dsmphdr_extension {

};

#endif
