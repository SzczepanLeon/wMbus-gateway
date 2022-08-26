/***********************************************************************************
    Filename: ethernet.h
***********************************************************************************/

#ifndef _ETHERNET_H
#define _ETHERNET_H


//----------------------------------------------------------------------------------
//  Constants 
//----------------------------------------------------------------------------------

#define ETH_CLK_MODE    ETH_CLOCK_GPIO0_IN
#define ETH_PHY_POWER   16
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#include <ETH.h>

#endif