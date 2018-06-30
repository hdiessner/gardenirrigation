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

| I/O Description | PIN |  Remark  | Wire Color |
|-----------------|-----|----------|------------|
| SOILFRONT       | A13 | Analog   | 11 Yellow  |
| SOILBACK        | A14 | Analog   | 12 Orange  |
| LEAFSENSOR      | A15 | Analog   | 08 Pink    |
| REL1            |  22 | Output   | Blue       |
| REL2            |  24 | Output   | Green      |
| REL3            |  26 | Output   | Yellow     |
| REL4            |  28 | Output   | Orange     |
| WATERLEVEL1     |  31 | Pullup   | 01 Yellow  |
| WATERLEVEL2     |  33 | Pullup   | 02 Orange  |
| WATERLEVEL3     |  35 | Pullup   | 03 Red     |
| WATERLEVEL4     |  37 | Pullup   | 04 Brown   |
| WATERLEVEL5     |  39 | Pullup   | 05 Black   |
| MOTION          |  41 | Floating | 09 Blue    |
| DOOR            |  43 | Pullup   | 07 Grey    |
| DALLASPIN       |  45 | DS18B20  | 10 Green   |
| DHTPIN          |  47 | DHT22    | 06 White   |

## DHT22
Left to right 1,2,3,4

| Number | Purpose | Wire colour |
|--------|---------|-------------|
| 1      | VCC     | Red         |
| 2      | Data    | Yellow      |
| 3      | NC      | White       |
| 4      | GND     | Black       |

## SOIL
Numbers are defined by DMX connectors

| Number | Purpose | Wire colour |
|--------|---------|-------------|
| 1|     | GND     | Black       |
| 2|     | 5V      | Red         |
| 3|     | Data    | Blank wire  |

## DS18B20
Numbers are defined by DMX connectors

| Number | Purpose | Wire colour |
|--------|---------|-------------|
| 1|     | GND     | Black       |
| 2|     | 5V      | Red         |
| 3|     | Data    | Yellow      |

## Schematics
![schematics](https://github.com/hdiessner/gardenirrigation/blob/master/hardware/Schematics.png "Schematics")

## Print connectors

      1 2 3 4 5 6
  
                       7
                       8
                       9
                       0
      ABCDEFGHIJKL

| Nummer | Description   |
|--------|---------------|
| 1      | Water 100 %   |
| 2      | Water  75 %   |
| 3      | Water  50 %   |
| 4      | Water  25 %   |
| 5      | Water  15 %   |
| 6      | GND           |
| 7      | Temper Switch |
| 8      | GND           |
| 9      | Leaf Sensor   |
| 0      | Leaf Sensor   |
| A      | 5V            |
| B      | GND           |
| C      | Motion        |
| D      | 5V            | 
| E      | GND           |
| F      | DS18B20       |
| G      | 5V            |
| H      | GND           |
| I      | Soil Front    |
| J      | 5V            |
| K      | GND           |
| L      | Soil Back     |

# MQTT Topics

## Out
| Topic                         | Payload          | Comment               |
|-------------------------------|------------------|-----------------------|
| garden/waterbox/system        | "startup"        | Startup / Reboot      |  
| garden/waterbox/temperature   | float            |                       |
| garden/waterbox/humidity      | float            |                       |
| garden/irrigation/waterlevel  | int 0..100       |                       |
| garden/irrigation/leaf        | int 0..(2^10)-1  |                       |
| garden/irrigation/soilfront   | int 0..(2^10)-1  |                       |
| garden/irrigation/soilback    | int 0..(2^10)-1  |                       |
| garden/waterbox/door          | "OPEN" / "CLOSE" |                       |
| garden/cellarentrance/motion  | int              | seconds since startup |
| garden/irrigation/tempdeep    | float            |                       |
| garden/irrigation/tempsurface | float            |                       |

## In
| Topic                      | Payload          |
|----------------------------|------------------|
| garden/irrigation/rainpump | "on" / "off"     |
| garden/irrigation/rel2     | "on" / "off"     |
| garden/irrigation/rel3     | "on" / "off"     |
| garden/irrigation/rel4     | "on" / "off"     |

# Links
https://gist.github.com/lx-88/413b48ced6b79300ea76
