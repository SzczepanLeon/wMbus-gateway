/***********************************************************************************
    Filename: config_wt32-eth01_v1.2.hpp
***********************************************************************************/

#ifndef _MY_CONFIG_WT32_ETH01_V12_H
#define _MY_CONFIG_WT32_ETH01_V12_H

//----------------------------------------------------------------------------------
//  Constants 
//----------------------------------------------------------------------------------
//CC1101 config
#define GDO0            14
#define GDO2            35

#define SPI_SCK         12
#define SPI_MISO        4
#define SPI_MOSI        15
#define SPI_SS          2


// ETH config
#define ETH_CLK_MODE    ETH_CLOCK_GPIO0_IN
#define ETH_PHY_POWER   16
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#endif