/*------------------------------------------------------------------------------
 * RSU - Component :: RSU Type, Road Side Unit
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUTypes.h
 * Purpose: RSU module, type definition
 *----------------------------------------------------------------------------*/
#ifndef _RSU_TYPES_H_
#define _RSU_TYPES_H_

#include "CString.h"


#include "GBT-31024-2-Spec.h"

enum RSU_MODE
{
    RSU_MODE_Standard   = 1,                                            // 标准模式，独立组网
    RSU_MODE_Slave      = 2,                                            // 组网模式，从RSU
    RSU_MODE_Master     = 3,                                            // 组网模式，主RSU
};

#endif // _RSU_TYPES_H_
