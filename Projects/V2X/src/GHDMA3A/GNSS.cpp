// ------------------------------------------------------------------------------
// RSU Driver - Component :: GNSS Driver
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    GNSS.cpp
// Purpose: RSU module
//-------------------------------------------------------------------------------

#include "Config.h"

#include "ublox_cfg.h"
#include "GNSS.h"

// #define TTYDEV  "/dev/ttyS1"

struct broadcast_node broadcast_list[] = {
    {"127.0.0.1", 40400},
    {"127.0.0.1", 40401},
};


CGNSSDriver::CGNSSDriver()
{
    m_ttyHandle = -1;
}

bool CGNSSDriver::InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam)
{
    if ( LoadConfig() == false )
    {
        LogE("CGNSSDriver LoadConfig failed");
        return false;   
    }

    if ( pCallbackParam == NULL || pCallbackParam == NULL )
    {
        LogE("CGNSSDriver::InitialBorad parameters is NULL pointer");
        return false;
    }

    m_pMsgCallBack      = pCallback;
    m_pCallbackParam    = pCallbackParam;

    LogI("需要对GNSS进行测试");
    return true;

    if ( tty_init(B9600) == -1 )
    {
        LogE("CGNSSDriver Initial TTY B9600 failed.");
        return false;
    }

    if ( gnss_receiver_init(gnss_receiver_cfg_message, gnss_receiver_cfg_message_len, cfg_message_cnt) == -1 )
    {
        LogE("CGNSSDriver Initial gnss receiver failed.");
        return false;
    }

    if ( tty_init(B115200) == -1 )
    {
        LogE("CGNSSDriver Initial TTY B115200 failed.");
        return false;
    }

    m_BroadcastHandle = gnss_broadcast_init();
    if ( m_BroadcastHandle == -1 )
    {
        LogE("CGNSSDriver Create gnss broadcast socket failed.");
        return false;
    }

    nmea_reader_init();

    m_ttyHandle = tty_open();
    if ( m_ttyHandle == -1)
    {
        LogE("CGNSSDriver::InitialBorad Create tty handle error, code: %d", m_ttyHandle);
        return false;
    }

    int nRetCode = pthread_create(&m_ThreadID, NULL, CGNSSDriver::DriverThread, (LPVOID)this);
    if ( nRetCode != 0)
    {
        LogE("CGNSSDriver::InitialBorad Create Thread error, code: %d", nRetCode);
        return false;
    }

    return true;
}


bool CGNSSDriver::LoadConfig()
{
    // 加载默认配置参数
    CString strCfgFile = g_strAppRootPath;

    strCfgFile = strCfgFile + "/conf/v2x.conf";

    CConfig configFile;
    if( configFile.OpenConfigFile(strCfgFile) == false )
    {
        LogE("Can not open config file: %s", strCfgFile.GetBuffer());
        return false;
    }

    CString strSection = "COMMON";
    CString strValue, strKey;
    UINT nValue;

    strKey = "GNSSDevice";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strTTYDevice = strValue;

    return true;
}

void* CGNSSDriver::DriverThread(LPVOID pParam)
{
    if ( pParam == NULL)
    {
        LogE("Msg Client thread parameters error.");
        return NULL;
    }

    CGNSSDriver *pGNSSDriver = (CGNSSDriver*)pParam;
    pGNSSDriver->StartDriver();

    return NULL;
}

// 查询是否接收到消息
void CGNSSDriver::StartDriver()
{
    fd_set read_set;
    struct timeval tv_timeout;
    int max_fd;
    UCHAR gnss_tty_buf[2048] = {0};

    while(1)
    {
        FD_ZERO(&read_set);
        FD_SET(m_ttyHandle, &read_set);
        max_fd = m_ttyHandle + 1;
        tv_timeout.tv_sec = 20;
        tv_timeout.tv_usec = 0;
        int r = select(max_fd, &read_set, NULL, NULL, &tv_timeout);
        if (r > 0) 
        {
            int nLen = read(m_ttyHandle, gnss_tty_buf, sizeof(gnss_tty_buf));
            if (nLen <= 0)
                continue;

            for (int i = 0; i < nLen; i++)
                nmea_reader_addc(gnss_tty_buf[i]);

            LogE("CGNSSDriver 需要确认发送的数据内容...");
            m_pMsgCallBack(m_pCallbackParam, gnss_tty_buf, nLen);
            // r = gnss_broadcast(m_BroadcastHandle, (PUCHAR)&(reader[0].fix), sizeof(GnssLocation));
        } 
        else if (r == 0)
            // timeout 
            continue;
        else
            // error 
            continue;
    }
}

// ***************************************************************
// ***************************************************************
// ****                                                       ****
// ****       GPS Config & Interface                          ****
// ****                                                       ****
// ***************************************************************
// ***************************************************************
int CGNSSDriver::gnss_set_chk(PUCHAR msg, int size)
{
    if( (0 == msg) || (size <= 0) )
    {
        LogE("gnss_set_chk failed");
        return -1;
    }

    for(int i = 0; i < (size-4); i++)
    {
        msg[size-2] += msg[i+2];
        msg[size-1] += msg[size-2];
    }

    return 0;
}

int CGNSSDriver::gnss_write_tty(int fd, PUCHAR buf, int size)
{
    int cnt = 0;
    int sum = 0;

    if( 0 == buf || size <= 0 || fd < 0 )
    {
        LogE("gnss_write_tty parameters ");
        return -1;
    }

    while (sum < size)
    {
        cnt = write(fd, (buf+sum), (size-sum));
        if(cnt <= 0)
        {
            LogE("gnss_set_chk failed");
            return -1;
        }

        sum += cnt;
    }

    return 0;
}

int CGNSSDriver::tty_open()
{
    int flags = 0;
    flags = O_RDWR | O_NOCTTY;
    return open(m_strTTYDevice.GetBuffer(), flags);
}

int CGNSSDriver::tty_init(speed_t speed)
{
    int fd;
    int ret;
    struct termios old_tty_opt;
    struct termios new_tty_opt;

    fd = tty_open();
    if (fd < 0)
    {
        LogE("open %s, error %s...", m_strTTYDevice.GetBuffer(), strerror(errno));
        return -1;
    }

    memset(&old_tty_opt, 0, sizeof(old_tty_opt));
    memset(&new_tty_opt, 0, sizeof(new_tty_opt));
    if ( tcgetattr(fd, &old_tty_opt) < 0) 
    {
        close(fd);
        LogE("GNSS Driver tcgetattr error %s...", strerror(errno));
        return -1;
    }

    new_tty_opt = old_tty_opt;
    cfsetispeed(&new_tty_opt, speed);
    cfsetospeed(&new_tty_opt, speed);
    new_tty_opt.c_iflag &= ~(IXON | IXOFF);
    new_tty_opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    new_tty_opt.c_oflag &= ~OPOST;
    tcflush(fd, TCIOFLUSH);
    
    if ( tcsetattr(fd, TCSANOW, &new_tty_opt) < 0) 
    {
        close(fd);
        LogE("GNSS Driver tcsetattr error %s...", strerror(errno));
        return -1;
    }

    close(fd);
    return 0;
}

int CGNSSDriver::gnss_receiver_init(PUCHAR *msg, int *msg_len, int count)
{
    int i = 0;
    int retry = 3;
    int fd;

    fd = tty_open();
    if (fd < 0) 
    {
        LogE("open %s, error %s...", m_strTTYDevice.GetBuffer(), strerror(errno));
        return -1;
    }

    for (i = 0;i < count;i++)
    {
        gnss_set_chk(msg[i], msg_len[i]);
        gnss_write_tty(fd, msg[i], msg_len[i]);
    }

    close(fd);
    return 0;
}

int CGNSSDriver::gnss_broadcast_init()
{
    int fd = -1;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    return fd;
}

int CGNSSDriver::gnss_broadcast(int fd, PUCHAR data, int len)
{
    struct sockaddr_in peer_gnss_sockaddr;
    int count;
    int r;

    count = sizeof(broadcast_list)/sizeof(struct broadcast_node);
    for (int i = 0; i < count; i++) 
    {
        memset((void*)(&peer_gnss_sockaddr), 0, sizeof(peer_gnss_sockaddr));
        peer_gnss_sockaddr.sin_family = AF_INET;
        peer_gnss_sockaddr.sin_addr.s_addr = inet_addr(broadcast_list[i].ip_addr);
        peer_gnss_sockaddr.sin_port = htons(broadcast_list[i].port);
        r = sendto(fd, data, len, 0, (struct sockaddr*)&peer_gnss_sockaddr, sizeof(peer_gnss_sockaddr));
        if (r != len)
            break;
    }

    return r;
}


// ***************************************************************
// ***************************************************************
// ****                                                       ****
// ****       N M E A   P A R S E R                           ****
// ****                                                       ****
// ***************************************************************
// ***************************************************************

int CGNSSDriver::str2int(PUCHAR p, PUCHAR end)
{
    int   result = 0;
    int   len    = end - p;

    for ( ; len > 0; len--, p++)
    {
        int  c;

        if (p >= end)
            return -1;

        c = *p - '0';
        if ((unsigned)c >= 10)
            return -1;

        result = result*10 + c;
    }

    return  result;
}

double CGNSSDriver::str2float(PUCHAR p, PUCHAR end)
{
    int   result = 0;
    int   len    = end - p;
    char  temp[16];

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len);
    temp[len] = 0;
    return strtod( temp, NULL);
}

double CGNSSDriver::convert_from_hhmm(Token tok)
{
    double  val     = str2float((PUCHAR)tok.p, tok.end);
    int     degrees = (int)(floor(val) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;
    return dcoord;
}

int CGNSSDriver::nmea_tokenizer_init(NmeaTokenizer* t, PUCHAR p, PUCHAR end)
{
    int    count = 0;
    char*  q;

    // the initial '$' is optional
    if (p < end && (p[0] == '$' || p[0] == '#'))
        p += 1;

    // remove trailing newline
    if (end > p && end[-1] == '\n') 
    {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && end[-3] == '*')
        end -= 3;
    
    while (p < end)
    {
        PUCHAR  q = p;

        q = (PUCHAR)memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p)
        {
            if (count < MAX_NMEA_TOKENS) 
            {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }

        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

Token CGNSSDriver::nmea_tokenizer_get(NmeaTokenizer* t, int  index)
{
    Token  tok;
    static PUCHAR  dummy = (PUCHAR)"";

    if (index < 0 || index >= t->count) 
        tok.p = tok.end = dummy; 
    else
        tok = t->tokens[index];

    return tok;
}

void CGNSSDriver::nmea_reader_update_utc_diff()
{
    m_Reader.utc_diff = 8*60*60;
}

int CGNSSDriver::nmea_reader_update_time(Token  tok)
{
    int        msec;
    struct tm  tm;
    time_t     fix_time;

    if (tok.p + 8 > tok.end)
        return -1;

    if (m_Reader.utc_year < 0)
    {
        // no date yet, get current one
        time_t  now = time(NULL);
        gmtime_r( &now, &tm);
        m_Reader.utc_year = tm.tm_year + 1900;
        m_Reader.utc_mon  = tm.tm_mon + 1;
        m_Reader.utc_day  = tm.tm_mday;
    }

    msec    = str2int(tok.p+7, tok.p+8);

    tm.tm_hour  = str2int(tok.p,   tok.p+2);
    tm.tm_min   = str2int(tok.p+2, tok.p+4);
    tm.tm_sec   = str2int(tok.p+4, tok.p+6);
    tm.tm_year  = m_Reader.utc_year - 1900;
    tm.tm_mon   = m_Reader.utc_mon - 1;
    tm.tm_mday  = m_Reader.utc_day;
    tm.tm_isdst = -1;

    fix_time = mktime( &tm) + m_Reader.utc_diff;
    // unit: mseconds
    m_Reader.fix.timestamp = ((long long)fix_time * 1000l) + ((long long)msec * 100l);
    return 0;
}

int CGNSSDriver::nmea_reader_update_time_s(Token  tok)
{
    if (tok.p >= tok.end)
    {
        LogE("nmea_reader_update_time_s failed");
        return -1;
    }

    m_Reader.fix.timestamp  = (long long)(str2float(tok.p, tok.end))*1000;
    return 0;
}

int CGNSSDriver::nmea_reader_update_date(Token  date, Token  time)
{
    Token  tok = date;
    int    day, mon, year;

    if (tok.p + 6 != tok.end)
    {
        LogE("nmea_reader_update_date failed");
        return -1;
    }

    day  = str2int(tok.p, tok.p+2);
    mon  = str2int(tok.p+2, tok.p+4);
    year = str2int(tok.p+4, tok.p+6) + 2000;

    m_Reader.utc_year  = year;
    m_Reader.utc_mon   = mon;
    m_Reader.utc_day   = day;

    return nmea_reader_update_time(time);
}


int CGNSSDriver::nmea_reader_update_latlong(Token latitude, char latitudeHemi, Token longitude, char longitudeHemi)
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p + 6 > tok.end)
    {
        LogE("nmea_reader_update_latlong failed");
        return -1;
    }

    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end)
    {
        LogE("nmea_reader_update_latlong failed");
        return -1;
    }

    lon = convert_from_hhmm(tok);
    if (longitudeHemi == 'W')
        lon = -lon;

    m_Reader.fix.flags    |= GNSS_LOCATION_HAS_LAT_LONG;
    m_Reader.fix.latitude  = lat;
    m_Reader.fix.longitude = lon;
    return 0;
}


int CGNSSDriver::nmea_reader_update_latlong_s( Token latitude, Token longitude)
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p > tok.end)
    {
        LogE("nmea_reader_update_latlong_s failed");
        return -1;
    }

    lat = str2float(tok.p, tok.end);

    tok = longitude;
    if (tok.p > tok.end)
    {
        LogE("nmea_reader_update_latlong_s failed");
        return -1;
    }

    lon = str2float(tok.p, tok.end);

    m_Reader.fix.flags    |= GNSS_LOCATION_HAS_LAT_LONG;
    m_Reader.fix.latitude  = lat;
    m_Reader.fix.longitude = lon;
    return 0;
}

int CGNSSDriver::nmea_reader_update_altitude(Token altitude, Token units)
{
    Token   tok = altitude;

    if (tok.p >= tok.end)
    {
        LogE("nmea_reader_update_altitude failed");
        return -1;
    }

    m_Reader.fix.flags   |= GNSS_LOCATION_HAS_ALTITUDE;
    m_Reader.fix.altitude = str2float(tok.p, tok.end);
    return 0;
}

int CGNSSDriver::nmea_reader_update_bearing(Token bearing)
{
    Token   tok = bearing;

    if (tok.p >= tok.end)
    {
        LogE("nmea_reader_update_bearing failed");
        return -1;
    }

    m_Reader.fix.flags   |= GNSS_LOCATION_HAS_BEARING;
    m_Reader.fix.bearing  = str2float(tok.p, tok.end);
    return 0;
}

#define KN2MS(x) ((x) * 1852.0 / 3600.0)
int CGNSSDriver::nmea_reader_update_speed(Token speed)
{
    Token   tok = speed;

    if (tok.p >= tok.end)
    {
        LogE("nmea_reader_update_speed failed");
        return -1;
    }

    m_Reader.fix.flags   |= GNSS_LOCATION_HAS_SPEED;
    m_Reader.fix.speed    = str2float(tok.p, tok.end);
    m_Reader.fix.speed    = KN2MS(m_Reader.fix.speed);
    return 0;
}

int CGNSSDriver::nmea_reader_update_accuracy(Token accuracy)
{
    Token   tok = accuracy;
    
    if (tok.p >= tok.end)
    {
        LogE("nmea_reader_update_accuracy failed");
        return -1;
    }

    m_Reader.fix.flags    |= GNSS_LOCATION_HAS_ACCURACY;
    m_Reader.fix.accuracy  = str2float(tok.p, tok.end);
    return 0;
}


void CGNSSDriver::nmea_reader_parse()
{
    static int s_sv_num = -2;
    int sv_num;
    // we received a complete sentence, now parse it to generate a new GNSS fix...
    
    NmeaTokenizer  tzer[1];
    Token          tok;
    
    if (m_Reader.pos < 9)
    {
        LogE("nmea_reader_parse failed");
        return;
    }

    nmea_tokenizer_init(tzer, m_Reader.in, m_Reader.in + m_Reader.pos);

    tok = nmea_tokenizer_get(tzer, 0);
    if (tok.p + 5 > tok.end)
    {
        LogE("nmea_reader_parse failed");
        return;
    }

    // ignore first two characters.
    tok.p += 2;
    if ( !memcmp(tok.p, "STPOSA", 6))
    {
        Token  tok_time          = nmea_tokenizer_get(tzer,6);
        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,10);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,11);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,12);
        Token  tok_svn           = nmea_tokenizer_get(tzer,22);

        if ( memcmp(tok_fixStatus.p, "NONE", 4))
        {
            m_Reader.sv_flag = 1;
            nmea_reader_update_time_s(tok_time);
            nmea_reader_update_latlong_s(tok_latitude, tok_longitude);

            sv_num = str2int(tok_svn.p, tok_svn.end);
            sv_num /= 6;
            if (s_sv_num != sv_num)
                s_sv_num = sv_num;
        }
        else
        {
            m_Reader.sv_flag = 0;
            if (s_sv_num != -1)
                s_sv_num = -1;
        }
    }
    else if ( !memcmp(tok.p, "GGA", 3))
    {
        // GNSS fix
        Token  tok_time          = nmea_tokenizer_get(tzer,1);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
        Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
        Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
        Token  tok_altitude      = nmea_tokenizer_get(tzer,9);
        Token  tok_altitudeUnits = nmea_tokenizer_get(tzer,10);

        nmea_reader_update_time(tok_time);
        nmea_reader_update_latlong(tok_latitude, tok_latitudeHemi.p[0], tok_longitude, tok_longitudeHemi.p[0]);
        nmea_reader_update_altitude(tok_altitude, tok_altitudeUnits);
    }
    else if ( !memcmp(tok.p, "GSA", 3))
    {
        Token tok_fixStatus = nmea_tokenizer_get(tzer,2);
        if (tok_fixStatus.p != tok_fixStatus.end && (tok_fixStatus.p[0]=='2' || tok_fixStatus.p[0]=='3'))
        {
            m_Reader.sv_flag = 1;
            Token tok_accuracy = nmea_tokenizer_get(tzer,15);
            nmea_reader_update_accuracy(tok_accuracy);
            int i;
            for (i=3; i<=14; i++)
            {
                Token tok_prn = nmea_tokenizer_get(tzer,i);
                int n_prn = str2int(tok_prn.p, tok_prn.end);
                if (n_prn>0)
                    m_Reader.svs.used_in_fix_mask |= (1<<(n_prn-1));
            }
        }
        else
        {
            m_Reader.sv_flag = 0;
            if (s_sv_num != -1)
                s_sv_num = -1;
        }
    } 
    else if ( !memcmp(tok.p, "GSV", 3) && *(tok.p-1)=='P') 
    {
        Token tok_noSatellites = nmea_tokenizer_get(tzer, 3);
        int noSatellites = str2int(tok_noSatellites.p, tok_noSatellites.end);
        Token tok_noSentences = nmea_tokenizer_get(tzer, 1);
        Token tok_sentence = nmea_tokenizer_get(tzer, 2);
        int sentence = str2int(tok_sentence.p, tok_sentence.end);
        int totalSentences = str2int(tok_noSentences.p, tok_noSentences.end);
        int i,ns;

        if (1 == sentence)
        {
            m_Reader.svs.num_svs = noSatellites;
            m_Reader.svs.ephemeris_mask = 0;
            m_Reader.svs.almanac_mask = 0;

            sv_num = noSatellites;
            sv_num /= 6;
            if (s_sv_num != sv_num && m_Reader.sv_flag != 0)
                s_sv_num = sv_num;
        }

        for (i=0; i<4; i++)
        {
            ns = (sentence-1)*4+i;
            if (ns >= noSatellites)
                break;

            Token tok_prn = nmea_tokenizer_get(tzer, i*4+4);
            Token tok_elevation = nmea_tokenizer_get(tzer, i*4+5);
            Token tok_azimuth = nmea_tokenizer_get(tzer, i*4+6);
            Token tok_snr = nmea_tokenizer_get(tzer, i*4+7);
            m_Reader.svs.sv_list[ns].prn = str2int(tok_prn.p, tok_prn.end);
            m_Reader.svs.sv_list[ns].elevation = str2float(tok_elevation.p, tok_elevation.end);
            m_Reader.svs.sv_list[ns].azimuth = str2float(tok_azimuth.p, tok_azimuth.end);
            m_Reader.svs.sv_list[ns].snr = str2float(tok_snr.p, tok_snr.end);
            if (m_Reader.svs.sv_list[ns].prn > 0) 
            {
                m_Reader.svs.ephemeris_mask |= (1<<(m_Reader.svs.sv_list[ns].prn-1));
                m_Reader.svs.almanac_mask |= (1<<(m_Reader.svs.sv_list[ns].prn-1));
            }
        }
    } 
    else if ( !memcmp(tok.p, "RMC", 3)) 
    {
        Token  tok_time          = nmea_tokenizer_get(tzer,1);
        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,3);
        Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,4);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,5);
        Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,6);
        Token  tok_speed         = nmea_tokenizer_get(tzer,7);
        Token  tok_bearing       = nmea_tokenizer_get(tzer,8);
        Token  tok_date          = nmea_tokenizer_get(tzer,9);
        
        m_Reader.svs.used_in_fix_mask = 0;

        if (tok_fixStatus.p[0] == 'A')
        {
            nmea_reader_update_date(tok_date, tok_time);
            nmea_reader_update_latlong(tok_latitude, tok_latitudeHemi.p[0], tok_longitude, tok_longitudeHemi.p[0]);
            nmea_reader_update_bearing(tok_bearing);
            nmea_reader_update_speed(tok_speed);
        }

    }
    else if ( !memcmp(tok.p, "VTG", 3)) 
    {
        Token  tok_bearing       = nmea_tokenizer_get(tzer,1);
        Token  tok_speed         = nmea_tokenizer_get(tzer,7);

        nmea_reader_update_bearing(tok_bearing);
        nmea_reader_update_speed  (tok_speed);

    } 
    else 
    {
        tok.p -= 2;
    }
}

void CGNSSDriver::nmea_reader_init()
{
    memset(&m_Reader, 0, sizeof(m_Reader));

    m_Reader.pos      = 0;
    m_Reader.overflow = 0;
    m_Reader.utc_year = -1;
    m_Reader.utc_mon  = -1;
    m_Reader.utc_day  = -1;
    m_Reader.fix.size = sizeof(m_Reader.fix);
    m_Reader.sv_flag  = 0;
    m_Reader.svs.size = sizeof(m_Reader.svs);

    nmea_reader_update_utc_diff();
}

void CGNSSDriver::nmea_reader_addc(int c)
{
    if (m_Reader.overflow) 
    {
        m_Reader.overflow = (c != '\n');
        return;
    }

    if (m_Reader.pos >= (int) sizeof(m_Reader.in)-1)
    {
        m_Reader.overflow = 1;
        m_Reader.pos      = 0;
        return;
    }

    m_Reader.in[m_Reader.pos] = (char)c;
    m_Reader.pos       += 1;

    if (c == '\n') 
    {
        nmea_reader_parse();
        m_Reader.pos = 0;
    }
}


 // 1. configure AP's uart
 // 2. configure GPS receiver
 // 3. receive GPS receiver message
 // 4. parse the NMEA message
 // 5. send the GNSS data to other tasks
 int GNSS_Initial(int argc, char **argv)
{
    return 0;

    // int m_BroadcastHandle = -1;
    // int nn;
    // int r = -1;
    // NmeaReader  reader[1];
    // fd_set read_set;
    // struct timeval tv_timeout;
    // int max_fd;
    // int tty_fd;

    // tty_init(TTYDEV, B9600);
    // gnss_receiver_init(TTYDEV,
    //                    gnss_receiver_cfg_message,
    //                    gnss_receiver_cfg_message_len,
    //                    cfg_message_cnt
    //                 );
    // tty_init(TTYDEV, B115200);

    // m_BroadcastHandle = gnss_broadcast_init();
    // nmea_reader_init(reader);
    // tty_fd = tty_open(TTYDEV);
    // while (1) {
    //     FD_ZERO(&read_set);
    //     FD_SET(tty_fd, &read_set);
    //     max_fd = tty_fd + 1;
    //     tv_timeout.tv_sec = 20;
    //     tv_timeout.tv_usec = 0;
    //     r = select(max_fd, &read_set, NULL, NULL, &tv_timeout);
    //     if (r > 0) {
    //         r = read(tty_fd, gnss_tty_buf, sizeof(gnss_tty_buf));
    //         if (r <= 0) {
    //             continue;
    //         }
    //         for (nn = 0; nn < r; nn++)
    //             nmea_reader_addc(reader, gnss_tty_buf[nn]);
    //         r = gnss_broadcast(m_BroadcastHandle, (PUCHAR)&(reader[0].fix), sizeof(GnssLocation));
    //     } else if (r == 0) {
    //         /* timeout */
    //         continue;
    //     } else {
    //         /* error */
    //         continue;
    //     }
    // }

    // exit(0);
}
