// ------------------------------------------------------------------------------
// RSU Driver - Component :: GNSS Driver
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    GNSS.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _GNSS_H_
#define _GNSS_H_

#include "Debug.h"
#include "MsgBase.h"

#define GNSS_LOCATION_HAS_LAT_LONG      0x0001                          // GnssLocation has valid latitude and longitude.
#define GNSS_LOCATION_HAS_ALTITUDE      0x0002                          // GnssLocation has valid altitude. 
#define GNSS_LOCATION_HAS_SPEED         0x0004                          // GnssLocation has valid speed. 
#define GNSS_LOCATION_HAS_BEARING       0x0008                          // GnssLocation has valid bearing. 
#define GNSS_LOCATION_HAS_ACCURACY      0x0010                          // GnssLocation has valid accuracy. 

// Maximum number of SVs. 
#define GNSS_MAX_SVS                    32

typedef int64_t                         GnssUtcTime;                    // Milliseconds since January 1, 1970.
typedef int                             GNSS_HANDLE;

// Represents a location. 
typedef struct 
{
    size_t                  size;                                       // set to sizeof(GnssLocation) 
    uint16_t                flags;                                      // Contains GnssLocationFlags bits. 
    double                  latitude;                                   // Represents latitude in degrees. 
    double                  longitude;                                  // Represents longitude in degrees. 
    double                  altitude;                                   // Represents altitude in meters above the WGS 84 reference ellipsoid. 
    float                   speed;                                      // Represents speed in meters per second. 
    float                   bearing;                                    // Represents heading in degrees. 
    float                   accuracy;                                   // Represents expected accuracy in meters. 
    GnssUtcTime             timestamp;                                  // Timestamp for the location fix. 
}GnssLocation;

// Open a relevant GNSS interface and store the handle.
// return a handle on success or -1 on error.
extern GNSS_HANDLE GNSS_Open();

// Close the underlying GNSS interface and clear the handle.
// return 0 on success or -1 on error.
extern int GNSS_Close(GNSS_HANDLE handle);

// Attempts to receive GNSS location info via handle into the fix.
// If no data available, the receive would block.
// param[in] handle:  GNSS handle.
// param[in] fix:     Point to the location info.
// return: The number of bytes received on success or -1 on error.
extern int GNSS_Recv(GNSS_HANDLE handle, GnssLocation * fix);


// Represents SV information.
typedef struct 
{
    size_t                  size;                                       // set to sizeof(GnssSvInfo) 
    int                     prn;                                        // Pseudo-random number for the SV. 
    float                   snr;                                        // Signal to noise ratio. 
    float                   elevation;                                  // Elevation of SV in degrees. 
    float                   azimuth;                                    // Azimuth of SV in degrees. 
}GnssSvInfo;

// Represents SV status.
typedef struct 
{
    size_t                  size;                                       // set to sizeof(GnssSvStatus) 
    int                     num_svs;                                    // Number of SVs currently visible. 
    GnssSvInfo              sv_list[GNSS_MAX_SVS];                      // Contains an array of SV information. 
    uint32_t                ephemeris_mask;                             // Represents a bit mask indicating which SVs have ephemeris data. 
    uint32_t                almanac_mask;                               // Represents a bit mask indicating which SVs have almanac data. 
    uint32_t                used_in_fix_mask;                           // Represents a bit mask indicating which SVs were used for computing the most recent position fix. 
} GnssSvStatus;

// Parse GNSS
#define  NMEA_MAX_SIZE  255
typedef struct 
{
    int                     pos;
    int                     overflow;
    int                     utc_year;
    int                     utc_mon;
    int                     utc_day;
    int                     utc_diff;
    int                     sv_flag;
    GnssLocation            fix;
    GnssSvStatus            svs;
    UCHAR                   in[ NMEA_MAX_SIZE + 1 ];
}NmeaReader;


// ***************************************************************
// ***************************************************************
// ****                                                       ****
// ****       N M E A   T O K E N I Z E R                     ****
// ****                                                       ****
// ***************************************************************
// ***************************************************************
typedef struct 
{
    PUCHAR                  p;
    PUCHAR                  end;
}Token;

#define  MAX_NMEA_TOKENS    32

typedef struct 
{
    int                     count;
    Token                   tokens[ MAX_NMEA_TOKENS ];
}NmeaTokenizer;

struct broadcast_node 
{
    char                    ip_addr[32];
    int                     port;
};


class CGNSSDriver : public CObjBase
{
    public:
        CGNSSDriver();

    public:
        bool                InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam);

    protected:
        static void*        DriverThread(LPVOID pParam);
        void                StartDriver();

        bool                LoadConfig();

        int                 gnss_set_chk(PUCHAR msg, int size);
        int                 gnss_write_tty(int fd, PUCHAR buf, int size);
        int                 tty_open();
        int                 tty_init(speed_t speed);
        int                 gnss_receiver_init(PUCHAR *msg, int *msg_len, int count);
        int                 gnss_broadcast_init();
        int                 gnss_broadcast(int fd, PUCHAR data, int len);
        void                nmea_reader_init();
        void                nmea_reader_update_utc_diff();
        int                 nmea_tokenizer_init(NmeaTokenizer* t, PUCHAR p, PUCHAR  end );
        Token               nmea_tokenizer_get(NmeaTokenizer* t, int  index );
        int                 nmea_reader_update_time(Token  tok );
        int                 nmea_reader_update_time_s(Token  tok );
        int                 nmea_reader_update_date(Token  date, Token  time );
        int                 nmea_reader_update_latlong(Token latitude, char latitudeHemi, Token longitude, char longitudeHemi);
        int                 nmea_reader_update_latlong_s(Token latitude, Token longitude );
        int                 nmea_reader_update_altitude(Token altitude, Token units);
        int                 nmea_reader_update_bearing(Token bearing);
        int                 nmea_reader_update_speed(Token speed);
        int                 nmea_reader_update_accuracy(Token accuracy);
        void                nmea_reader_parse();
        void                nmea_reader_addc(int  c );

        int                 str2int(PUCHAR p, PUCHAR end);
        double              str2float(PUCHAR p, PUCHAR end);
        double              convert_from_hhmm(Token tok);

    protected:

        MSG_CALLBACK        m_pMsgCallBack;                             // 消息回调函数
        LPVOID              m_pCallbackParam;

        pthread_t           m_ThreadID;

        int                 m_ttyHandle;
        int                 m_BroadcastHandle;

        NmeaReader          m_Reader;
        CString             m_strTTYDevice;                             // GNSS TTY 设备名称
};

#endif
