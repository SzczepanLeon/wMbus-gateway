/***********************************************************************************
    Filename: config.hpp
***********************************************************************************/

#ifndef _MY_CONFIG_H
#define _MY_CONFIG_H

//----------------------------------------------------------------------------------
//  Constants 
//----------------------------------------------------------------------------------
//Client config
#define CLIENT_IP       "10.0.0.88"
#define CLIENT_PORT     2404


//CC1101 config
#define GDO0            35
#define GDO2            14

#define SPI_SCK         12
#define SPI_MISO        4
#define SPI_MOSI        15
#define SPI_SS          2


// OTA + ETH config
#define MY_HOSTNAME     "wMbus-GW"
#define OTA_PORT        3232

#define ETH_CLK_MODE    ETH_CLOCK_GPIO0_IN
#define ETH_PHY_POWER   16
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#endif