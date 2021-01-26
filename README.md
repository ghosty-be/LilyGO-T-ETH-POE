# LilyGO-T-ETH-POE (aka: TTGO T-Internet-POE)

- Official module info: http://www.lilygo.cn/prod_view.aspx?TypeId=50033&Id=1307&FId=t3:50033:3
- Example code: https://github.com/Xinyuan-LilyGO/LilyGO-T-ETH-POE

## Module Type
- My circuitboard indicates v1.0 on the back
  - date code on the back: 20-7-20 
  - ESP32-WROOM without PSRAM
- There also exists a v1.2 
  - date code on the back: 20-5-15
  - supposedly discontinued
  - ESP32-WROVER-B with 8MB of PSRAM (from which 4MB can be easily accessible from Arduino sketch) 
  - See the comment behind ETH_CLK_MODE in the code to change it if you have a v1.2 board

## Arduino IDE settings for this specific board
- Board: ESP32 WROVER module
- Upload speed: 115200
- the other settings I left to default

## Some additional information and notes
- The onboard blue LED near the USB-C port is only a power indicator (hard wired between 3,3V and GND)
- You can connect/power the board in 3 ways:
  - Connect the Ethernet interface to a PoE port
  - Connect the Ethernet interface to a non-PoE port and connect the USB-C interface onboard to 5V (power only, no programming interface!)
  - Connect the Ethernet interface to a non-PoE port, attach the programming board to the backside and connect this last one through USB-C or micro-USB to 5V power 
- When you attach the programming module you should disconnect power through the USB-C port onboard (and I guess the same goes if you connect to PoE delivery port)!
