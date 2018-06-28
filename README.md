# gardenirrigation
Automated Garden Irrigation Control with Sensors and Actors

# build
To compile and upload the code for an arduino mega 2560 and 
an ethernet shield please use [platformio](https://platformio.org).

```bash
pio run -t upload
```

all the neccessary libraries will be automatically cloned from
github.

# Hardware

## Pinout ATMeta

| I/O Description | PIN |  Remark  |
|-----------------|-----|----------|
| SOIL1           | A13 | Analog   |
| SOIL2           | A14 | Analog   |
| LEAFSENSOR      | A15 | Analog   |
| REL1            |  22 | Output   |
| REL2            |  24 | Output   |
| REL3            |  26 | Output   |
| REL4            |  28 | Output   |
| WATERLEVEL1     |  31 | Pullup   |
| WATERLEVEL2     |  33 | Pullup   |
| WATERLEVEL3     |  35 | Pullup   |
| WATERLEVEL4     |  37 | Pullup   |
| WATERLEVEL5     |  39 | Pullup   |
| MOTION          |  41 | Floating |
| DOOR            |  43 | Pullup   |
| DALLASPIN       |  45 | DS18B20  |
| DHTPIN          |  47 | DHT22    |

## DHT22
Left to right 1,2,3,4

| Number | Purpose | Wire colour |
|--------|---------|-------------|
| 1      | VCC     | Red         |
| 2      | Data    | Yellow      |
| 3      | NC      | White       |
| 4      | GND     | Black       |



## Schematics
![schematics](https://github.com/hdiessner/gardenirrigation/blob/master/hardware/Schematics.png "Schematics")

# MQTT Topics

## Out
| Topic                         | Payload          | Comment            |
|-------------------------------|------------------|--------------------|
| garden/waterbox/system        | "startup"        | Startup / Reboot   |  
| garden/waterbox/temperature   | float            |                    |
| garden/waterbox/humidity      | float            |                    |
| garden/irrigation/waterlevel  | float            |                    |
| garden/irrigation/leaf        | float            |                    |
| garden/irrigation/soilfront   | float            |                    |
| garden/irrigation/soilback    | float            |                    |
| garden/waterbox/door          | "OPEN" / "CLOSE" |                    |
| garden/cellarentrance/motion  | millis           | time since startup |
| garden/irrigation/tempdeep    | float            |                    |
| garden/irrigation/tempsurface | float            |                    |

## In
| Topic                      | Payload          |
|----------------------------|------------------|
| garden/irrigation/rainpump | "on" / "off"     |
| garden/irrigation/rel2     | "on" / "off"     |
| garden/irrigation/rel3     | "on" / "off"     |
| garden/irrigation/rel4     | "on" / "off"     |

# Links
https://gist.github.com/lx-88/413b48ced6b79300ea76
