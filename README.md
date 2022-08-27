# wMbus eth gateway
It is very simple wMbus to ethernet gateway.
Mbus telegram is without CRC i.e. can be used by wmbusmeters as device=stdin

## Required components
- ESP32 with ethernet: [WT32-ETH01](http://www.wireless-tag.com/portfolio/wt32-eth01/), [Olimex ESP32-POE](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/), etc.
- CC1101 module (868MHz)

## Wiring
```
CC1101 -> ESP32
=================
SCK    -> D12
MISO   -> D4
MOSI   -> D15
SS     -> D2
GDO0   -> D35
GDO2   -> D14
VCC    -> 3V3
GND    -> GND
```

## Building and usage
The app is based on PlatformIO and is build through PlatformIO's toolchain.

Confiruration is in file [include/config.hpp](include/config.hpp)

## Related materials:
- CC1101: https://www.ti.com/product/CC1101
- culfw-esp8266: https://github.com/Man-fred/culfw-esp8266
- Wmbusmeters: https://github.com/weetmuts/wmbusmeters
- wMbus quick start guide: https://www.silabs.com/documents/public/quick-start-guides/wireless-m-bus-quick-start-guide.pdf
- wMbus solutions: http://www.emcu.it/WirelessMBUS/Wireless_M-BUS_Solutions_and_more.pdf
- Parser for analysis of wMbus packets: https://www.miller-alex.de/WMbus
- Online wMbus or Mbus telegrams analyzer: https://wmbusmeters.org
- wMbus Izar meter: https://zewaren.net/wmbus-izar-meter.html
