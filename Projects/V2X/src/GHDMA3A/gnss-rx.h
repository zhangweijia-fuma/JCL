/***************************************************************//**
 * @addtogroup DTVL3000_GROUP2 Positioning API
 * @{
 *
 *  This module provides APIs to receive GNSS position information.
 * 
 *  
 *
 *******************************************************************/


/***************************************************************//**
 * COPYRIGHT NOTICE
 * Copyright (c) 2017 Datang Telecom Group All rights reserved
 * @file gnss-rx.h
 * @brief API file.
 * 
 *    
 *
 * @date 2017.3.15
 *******************************************************************/
#ifndef _GNSS_RX_H_
#define _GNSS_RX_H_


#include <stdint.h>


/** 
 * @brief Milliseconds since January 1, 1970.
 */ 
typedef int64_t GnssUtcTime;


/** 
 * @brief GNSS handle.
 */ 
typedef int                         GNSS_HANDLE;


/** 
 * @brief Represents a location.
 */ 
typedef struct {
    size_t          size;       /**< set to sizeof(GnssLocation) */
    
    uint16_t        flags;      /**< Contains GnssLocationFlags bits. */
#define GNSS_LOCATION_HAS_LAT_LONG   0x0001  /**< GnssLocation has valid latitude and longitude. */
#define GNSS_LOCATION_HAS_ALTITUDE   0x0002  /**< GnssLocation has valid altitude. */
#define GNSS_LOCATION_HAS_SPEED      0x0004  /**< GnssLocation has valid speed. */
#define GNSS_LOCATION_HAS_BEARING    0x0008  /**< GnssLocation has valid bearing. */
#define GNSS_LOCATION_HAS_ACCURACY   0x0010  /**< GnssLocation has valid accuracy. */
    
    double          latitude;   /**< Represents latitude in degrees. */
    double          longitude;  /**< Represents longitude in degrees. */
    double          altitude;   /**< Represents altitude in meters above the WGS 84 reference ellipsoid. */
    float           speed;      /**< Represents speed in meters per second. */
    float           bearing;    /**< Represents heading in degrees. */
    float           accuracy;   /**< Represents expected accuracy in meters. */
    GnssUtcTime     timestamp;  /**< Timestamp for the location fix. */
} GnssLocation;


/** 
 * @brief Open a relevant GNSS interface and store the handle.
 * @return  A handle on success or -1 on error.
 */
extern GNSS_HANDLE GNSS_Open();


/** 
 * @brief Close the underlying GNSS interface and clear the handle.
 * @return  0 on success or -1 on error.
 */
extern int GNSS_Close(GNSS_HANDLE handle);

/** 
 * @brief Attempts to receive GNSS location info via handle into the fix.
 *        If no data available, the receive would block.
 * @param[in] handle  GNSS handle.
 * @param[in] fix     Point to the location info.
 * @return  The number of bytes received on success or -1 on error.
 */
extern int GNSS_Recv(GNSS_HANDLE handle, GnssLocation * fix);

#endif

