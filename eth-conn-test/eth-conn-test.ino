/*  simple sketch to test the basic functionality of 
 *  LILYGO® TTGO T-Internet-POE ESP32-WROOM LAN8720A Chip Ethernet Adapter
 *  
 *  Arduino IDE settings:
 *  Board: ESP32 WROVER module
 *  Upload speed: 115200
 *  The other settings I left to default values
 *  
 *  The onboard LED near the USB-C port is only a power indicator (hard wired between 3,3V and GND)
 *  
 *  Connect an LED with a resistor of around 270ohm between pin 32 and GND
 *  If you connect the board to 5V power(OR the programming board) without network connection the led will blink
 *  If the board gets connected to ethernet the led will turn on statically
 *  Ofcourse you don't need the 5V (OR programming board) if your switch delivers PoE (but then this LED might only briefly blink during connection negotiation)
*/

// ## Configure the ethernet interface
#include <ETH.h>

// #### Settings from the example GIT code
#define ETH_PHY_POWER 12

// #define ETH_CLK_MODE    ETH_CLOCK_GPIO0_OUT          // Version with PSRAM
#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT            // Version without PSRAM

// Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_POWER_PIN   -1

// Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_TYPE        ETH_PHY_LAN8720

// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_ADDR        0

// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDC_PIN     23

// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_MDIO_PIN    18

static bool eth_connected = false;
uint64_t chipid;

// ## setup the micro SD card interface
#include <SPI.h>
#include <SD.h>
#define SD_MISO         2
#define SD_MOSI         15
#define SD_SCLK         14
#define SD_CS           13

// ## my variables
int ledPin = 32;
int ledState = LOW; // initial ledState

//ESP32 Specific Ethernet
//It says wifi but it's wired network config!
void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
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

void setup()
{
  Serial.begin(115200);

  // Init ledPin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // print MAC address
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.

  // Check if we can find a micro SD card in the slot
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
      Serial.println("SDCard MOUNT FAIL");
  } else {
      uint32_t cardSize = SD.cardSize() / (1024 * 1024);
      String str = "SDCard Size: " + String(cardSize) + "MB";
      Serial.println(str);
  }

  // Setup ethernet event
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
}

void blinkledPin()
{ 
  // Just a dirty blink function with delay
  // if the LED is off turn it on and vice-versa:
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  delay(2000);
  Serial.println("Turning ledPin " + String(ledPin) + " to state: " + String(ledState));
  digitalWrite(ledPin, ledState);
}

void loop() {
  // check if ethernet is connected
  if (eth_connected) {
    digitalWrite(ledPin, HIGH);
  } else {
    blinkledPin();
  }
}
