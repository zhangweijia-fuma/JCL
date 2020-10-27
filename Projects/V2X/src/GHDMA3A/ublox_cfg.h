#ifndef UBLOX_CFG_H
#define UBLOX_CFG_H


// CFG-TP set 1HZ time pulse
static unsigned char g_ubx_cfg_tp_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */ 
    0x06,
    /* id */
    0x07,
    /* payload length */ 
    0x14, 0x00,
    /* payload */
    0x40, 0x42, 0x0f, 0x00, 0xa0,
    0x86, 0x01, 0x00, 0x01, 0x01, 
    0x00, 0x00, 0x32, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-TP set gnss Ê§Ëø£¬Ö¸Ê¾µÆÃð
static unsigned char g_ubx_cfg_tp5_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */ 
    0x06,
    /* id */
    0x31,
    /* payload length */ 
    0x20, 0x00,
    /* payload */
    0x00, 0x01, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x00, 0x40, 0x42, 
    0x0f, 0x00, 0x40, 0x42, 0x0f, 
    0x00, 0x00, 0x00, 0x00, 0x00,
    0xa0, 0x86, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf7, 0x00,  /*0xf7-gps time; 0x77-utc time*/
    0x00, 0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-NAV5 set automotive platform model
static unsigned char g_ubx_cfg_nav5_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/                                       
    0x62,                                  
    /* class */
    0x06,
    /* id */
    0x24,
    /* payload length */
    0x24, 0x00,
    /* payload */
    0xff, 0xff, 0x04/*0x03*/, 0x03, 0x00,      //3:pedestrian 4:automotive
    0x00, 0x00, 0x00, 0x10, 0x27,
    0x00, 0x00, 0x05, 0x00, 0xfa,
    0x00, 0xfa, 0x00, 0x64, 0x00,
    0x2c, 0x01, 0x00, 0x3c, 0x00,
    0x00, 0x00, 0x00, 0xc8, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// PMS set
static unsigned char g_ubx_cfg_pms_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x86,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// PMS set
static unsigned char g_ubx_sav_pms_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x09,
    /* payload length */
    0x0d, 0x00,
    /* payload */
    0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-PRT set UART Ports input & output rates 115200
static unsigned char g_ubx_cfg_prt_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */ 
    0x06,
    /* id */
    0x00,
    /* payload length */ 
    0x14, 0x00,
    /* payload */
    0x01, 0x00, 0x00, 0x00, 0xd0,
    0x08, 0x00, 0x00, 0x00, 0xc2, 
    0x01, 0x00, 0x07, 0x00, 0x03, 
    0x00, 0x00, 0x00, 0x00, 0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNRMC
static unsigned char g_ubx_cfg_rmc_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x04, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNGGA
static unsigned char g_ubx_cfg_gga_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNGLL
static unsigned char g_ubx_cfg_gll_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,  //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNGSA
static unsigned char g_ubx_cfg_gsa_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GPGSV
static unsigned char g_ubx_cfg_gsv_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNVTG
static unsigned char g_ubx_cfg_vtg_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,  //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-MSG NMEA GNZDA
static unsigned char g_ubx_cfg_zda_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x01,
    /* payload length */
    0x08, 0x00,
    /* payload */
    0xf0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //disable, enable
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-INF
static unsigned char g_ubx_cfg_inf_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2*/
    0x62,
    /* class */
    0x06,
    /* id */
    0x02,
    /* payload length */
    0x0a, 0x00,
    /* payload */
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00,
    /* ch_A */
    0x00,
    /* ch_B*/
    0x00
};

// CFG-RATE set 5HZ NMEA message
static unsigned char g_ubx_cfg_rate_msg[] =
{
    /* sync char 1 */
    0xb5, 
    /* sync char 2 */
    0x62,
    /* class */
    0x06,
    /* id */
    0x08,
    /* payload length */ 
    0x06, 0x00,
    /* payload */
    0x64/*0xc8*//*0xe8*/, 0x00/*0x03*/, 0x01, 0x00, 0x01, 0x00,//100ms, 200ms, 1000ms
    /*ch_A*/
    0x00,
    /*ch_B*/
    0x00
};

// CFG-CFG save current cfg into permanent device
static unsigned char g_ubx_cfg_cfg_msg[] =
{
    /* sync char 1 */
    0xb5,
    /* sync char 2 */
    0x62,
    /* class */
    0x06,
    /* id */
    0x09,
    /* payload length */
    0x0d, 0x00,
    /* payload */
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x03,
    /*ch_A*/
    0x00,
    /*ch_B*/
    0x00
};


// CFG-GNSS enable Beidou&GPS
static unsigned char g_ubx_cfg_gnss_msg[] =
{
    /* sync char 1 */
    0xB5,
    /* sync char 2 */
    0x62, 
    /* class */
    0x06,
    /* id */
    0x3E,
    /* payload length */
    0x2C, 0x00,
    /* payload */
    0x00, 0x00, 0x20, 0x05, 
    0x00, 0x08, 0x10, 0x00, 
    0x01, 0x00, 0x01, 0x01, 
    0x01, 0x01, 0x03, 0x00,
    0x00, 0x00, 0x01, 0x01, 
    0x03, 0x08, 0x10, 0x00,
    0x01, 0x00, 0x01, 0x01,
    0x05, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x06, 0x08, 0x0E, 0x00,
    0x00, 0x00, 0x01, 0x01,
    /*ch_A*/
    0x00,
    /*ch_B*/
    0x00
};

// configure by the sequence 
static unsigned char *gnss_receiver_cfg_message[] = {
    g_ubx_cfg_tp_msg,
    g_ubx_cfg_tp5_msg,
    g_ubx_cfg_nav5_msg,
    g_ubx_cfg_pms_msg,
    g_ubx_sav_pms_msg,
    g_ubx_cfg_prt_msg,
    g_ubx_cfg_rmc_msg,
    g_ubx_cfg_gga_msg,
    g_ubx_cfg_gll_msg,
    g_ubx_cfg_gsa_msg,
    g_ubx_cfg_gsv_msg,
    g_ubx_cfg_vtg_msg,
    g_ubx_cfg_zda_msg,
    g_ubx_cfg_inf_msg,
    g_ubx_cfg_rate_msg,
    g_ubx_cfg_cfg_msg,
    g_ubx_cfg_gnss_msg,
};

static int gnss_receiver_cfg_message_len[] = {
    sizeof(g_ubx_cfg_tp_msg),
    sizeof(g_ubx_cfg_tp5_msg),
    sizeof(g_ubx_cfg_nav5_msg),
    sizeof(g_ubx_cfg_pms_msg),
    sizeof(g_ubx_sav_pms_msg),
    sizeof(g_ubx_cfg_prt_msg),
    sizeof(g_ubx_cfg_rmc_msg),
    sizeof(g_ubx_cfg_gga_msg),
    sizeof(g_ubx_cfg_gll_msg),
    sizeof(g_ubx_cfg_gsa_msg),
    sizeof(g_ubx_cfg_gsv_msg),
    sizeof(g_ubx_cfg_vtg_msg),
    sizeof(g_ubx_cfg_zda_msg),
    sizeof(g_ubx_cfg_inf_msg),
    sizeof(g_ubx_cfg_rate_msg),
    sizeof(g_ubx_cfg_cfg_msg),
    sizeof(g_ubx_cfg_gnss_msg),
};

static int cfg_message_cnt = sizeof(gnss_receiver_cfg_message)/sizeof(char *);

#endif
