#include <Arduino.h>
#include "config.hpp"
#include "ota.hpp"

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 100

#include <string>
#include "ethernet.hpp"
#include "rf_mbus.hpp"
#include "crc.hpp"
#include "mbus_packet.hpp"
#include "utils.hpp"

uint8_t MBpacket[291];

WiFiClient client;

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      ETH.setHostname(MY_HOSTNAME);
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}


void setup() {
  Serial.begin(112500);

  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);               //add current thread to WDT watch

  Serial.println("wMbus GW started");

  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_PHY_POWER, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);

  setupOta();
  ArduinoOTA.begin();

  memset(MBpacket, 0, sizeof(MBpacket));
  
  rf_mbus_init();
}


void loop() {
  ArduinoOTA.handle();
  int rssi = 0;

  if (rf_mbus_task(MBpacket, rssi)) {
    esp_task_wdt_reset();
    uint8_t lenWithoutCrc = crcRemove(MBpacket, packetSize(MBpacket[0]));
    Serial.print(" T: ");
    dumpHex(MBpacket, lenWithoutCrc);

    if (eth_connected) {
      // hex format
      if (client.connect(CLIENT_HEX_IP, CLIENT_HEX_PORT)) {
        client.write((const uint8_t *) MBpacket, lenWithoutCrc);
        client.stop();
      }
      // rtlwmbus format
      if (client.connect(CLIENT_RTLWMBUS_IP, CLIENT_RTLWMBUS_PORT)) {
        client.printf("T1;1;1;;%d;;;0x", rssi);
        for (int i = 0; i < lenWithoutCrc; i++){
          client.printf("%02X", MBpacket[i]);
        }
        client.print("\n");
        client.stop();
      }
    }
    memset(MBpacket, 0, sizeof(MBpacket));
 }
}
