
# Smart AQ Project

This is for final Embedded Project in Chulalongkorn University


## About this Project

BOP Project beginning with the effect of CO2 is attracked to people.

Typical outdoor CO2 concentrations are around 400 ppm. The onset CO2 concentration is 1500 ppm. Concentrations tend to be higher in confined structures without adequate ventilation. High carbon dioxide levels may be an indicator of pollution levels in other buildings. is also higher This is an indication that the building is not properly ventilated. Carbon dioxide concentrations also affect the human brain and overall health.  

Today's places like schools, cars, gyms, offices, conference rooms and many bedrooms have high levels of carbon dioxide that can affect health. but was ignored is the reason why with a point for Leads to the project with the solution is to turn on the ventilation fans and windows automatically to allow better ventilation
## Schrematic

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)


## Requirements 

- Arduino Mega 2560 x 1
- LCD x 1			
- matrix keypad	x 1		
- RTC x 1			
- Relay module x 1	
- Servo x 1			
- Temp & humidity sensor x 1	
-	RGB LED module × 1		
-	LED x 3				
-	Buzzer  x 1			
-	CO2 Sensor [mq135]	x 1	
-	DC motor x 1			
- R 1kohm x 3		
- Breadboard Power Module x 1
- 9V to DC Jack × 1 	

## Description 
- matrix keypad : use only 4 keypad
    - 1 : manual fan
    - 2 : manual window
    - 3 : manual both
    - 4 : changing mode between auto and manual
- RGB LED : show status of CO2 [ppm]
    - Green : lower than 1000 ppm
    - Yellow : between 1000 and 1500 ppm
    - Red : more than 1500 ppm
- LED : show status
    - green 1 : status of window
    - green 2 : status of fan
    - yellow : in auto mode `Yellow LED On`
- RTC : `[Auto Mode]` Set a time when no one stay such as during the day
    - at 12.00 : open window, close fan
    - at 13.00 : following CO2 data
- LCD 
    - show data of Temperature `T`, Humidity `H`, CO2 `Co2`
    - show date & real time

## Installation

Before start, you need to install these library 

```bash
    #include <LiquidCrystal_I2C.h>
    #include <Servo.h>
    #include "DHT.h"
    #include <DS3231.h> 
```

## Documentation

- [Documentation](https://linktodocumentation)
- [Presentation]()


## Authors

- 6230106221 Chaiwat Lophansri [@oatchaiwat](https://github.com/oatchaiwat)
- 6230119421 Thanida prasertchai [@proudtnd](https://github.com/proudtnd)
- 6230120021 Thanupong Wongthanyawat  

