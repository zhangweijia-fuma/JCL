/***************************************************************//**
 * @addtogroup DTVL3000_GROUP3 Time API
 * @{
 *
 *  This module provides APIs to get elapsed time.
 * 
 *  
 *
 *******************************************************************/


/***************************************************************//**
 * COPYRIGHT NOTICE
 * Copyright (c) 2017 Datang Telecom Group All rights reserved
 * @file time-get.h
 * @brief API file.
 * 
 *    
 *
 * @date 2017.3.15
 *******************************************************************/
#ifndef _TIME_GET_H_
#define _TIME_GET_H_


/** 
 * @brief Initialize the time module.
 * @return  0 on success or -1 on error.
 */
extern int Time_Init();


/** 
 * @brief Get elapsed time from the beginning of PP1s in us.
 * @return  Time count on success or -1 on error.
 */
extern int Time_GetElapsedTime_us();


#endif

/**
 * @}
 */
