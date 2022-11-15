/***********************************************************************************
    Filename: config.hpp
***********************************************************************************/

#ifndef _MY_CONFIG_ESP32_POE_H
#define _MY_CONFIG_ESP32_POE_H

//----------------------------------------------------------------------------------
//  Constants 
//----------------------------------------------------------------------------------
//CC1101 config
#define GDO0            16
#define GDO2            15

#define SPI_SCK         2
#define SPI_MISO        5
#define SPI_MOSI        13
#define SPI_SS          14


// ETH config
#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER   12
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        0
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#endif