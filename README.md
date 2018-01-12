# MK64_Car

## Platform
MSP430G2553

## Recommended IDE
[Code Composer Studio 7.4.0](http://processors.wiki.ti.com/index.php/Download_CCS#Code_Composer_Studio_Version_7_Downloads)

## Features
- Generates PWM signals to control motors and servos (engine / steering)
- Measures color sensor output signal frequency to determine color values (RGBC)
- Communicates with MK64_Car_Modem (TODO: add link) via SPI

## Used GPIO Pins

### PWM Signals

| Function | GPIO Pin | DIR     | PxSEL   | PxSEL2  |
| -------- | -------- |:-------:|:-------:|:-------:|
| steering | P2.1     | 1       | 1       | 0       |
| engine   | P2.4     | 1       | 1       | 0       |

### Color Sensor

#### Links
* [Product Info](https://www.conrad.at/de/arduino-erweiterungs-platine-colorimeter-tcs3200-schwarz-1503748.html?sc.ref=Search%20Results)
* [TCS3200 Datasheet](https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf)

| Name | Function          | GPIO Pin | DIR     | PxSEL   | PxSEL2  |
| ---- | ----------------- | -------- |:-------:|:-------:|:-------:|
| S0   | frequency scaling | P1.3     | 1       | 0       | 0       |
| S1   | frequency scaling | P1.2     | 1       | 0       | 0       |
| S2   | color selection   | P2.3     | 1       | 0       | 0       |
| S3   | color selection   | P2.0     | 1       | 0       | 0       |
| OE   | output enable     | P1.0     | 1       | 0       | 0       |
| OUT  | output signal     | P1.1     | 0       | 1       | 0       |

### SPI Communication

| Name | Function              | GPIO Pin | DIR       | PxSEL   | PxSEL2  |
| ---- | --------------------- | -------- |:---------:|:-------:|:-------:|
| STE  | slave transmit enable | P1.4     | 0         | 0       | 0       |
| CLK  | clock input           | P1.5     | from USCI | 1       | 1       |
| SIMO | slave input           | P1.7     | from USCI | 1       | 1       |
| SOMI | slave output          | P1.6     | from USCI | 1       | 1       |

