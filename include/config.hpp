/***********************************************************************************
    Filename: config.hpp
***********************************************************************************/
#include "config_esp32-poe.hpp"

#ifndef _MY_CONFIG_H
#define _MY_CONFIG_H

//----------------------------------------------------------------------------------
//  Constants 
//----------------------------------------------------------------------------------
//Client config
#define CLIENT_HEX_IP           "10.0.0.88"
#define CLIENT_HEX_PORT          2404

#define CLIENT_RTLWMBUS_IP       "10.0.0.27"
#define CLIENT_RTLWMBUS_PORT     2406

// OTA + ETH config
#define MY_HOSTNAME     "wMbus-GW"
#define OTA_PORT        3232

#endif