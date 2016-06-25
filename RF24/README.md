# nRF24L01 - 2.4 GHz Radio/Wireless Transceiver
### Notes
* Be sure to include a large, eg. 10uF, capacitor across +3.3V and GND pins to stabalize the voltage.  Not having this capacitor causes lots of stability problems and it will frustrate to endlessly :)


### Wiring
For the sample from HowTo (see below) 
```
    Ardunio -> Module
    GND     -> GND   (1)
    3.3V    -> Vcc   (2)
    pin 7   -> CE    (3)
    pin 8   -> CSN   (4)
    pin 13  -> SCK   (5)
    pin 11  -> MOSI  (6)
    pin 12  -> MOSO  (7)
    NC      -> IRQ   (8)
```

### Links
* [nRF24L01-2.4GHx-HowTo](https://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo)
