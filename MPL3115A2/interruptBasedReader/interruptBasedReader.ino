#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include "SparkFunHTU21D.h"


// I2C ADDRESS of the MPL311A2
#define MPL3115A2_ADDRESS                       (0x60)    // 1100000

#define MPL3115A2_REGISTER_STATUS               (0x00)
#define MPL3115A2_REGISTER_STATUS_TDR   0x02
#define MPL3115A2_REGISTER_STATUS_PDR   0x04
#define MPL3115A2_REGISTER_STATUS_PTDR  0x08

#define MPL3115A2_REGISTER_PRESSURE_MSB         (0x01)
#define MPL3115A2_REGISTER_PRESSURE_CSB         (0x02)
#define MPL3115A2_REGISTER_PRESSURE_LSB         (0x03)

#define MPL3115A2_REGISTER_TEMP_MSB             (0x04)
#define MPL3115A2_REGISTER_TEMP_LSB             (0x05)

#define MPL3115A2_REGISTER_DR_STATUS            (0x06)

#define MPL3115A2_OUT_P_DELTA_MSB               (0x07)
#define MPL3115A2_OUT_P_DELTA_CSB               (0x08)
#define MPL3115A2_OUT_P_DELTA_LSB               (0x09)

#define MPL3115A2_OUT_T_DELTA_MSB               (0x0A)
#define MPL3115A2_OUT_T_DELTA_LSB               (0x0B)

#define MPL3115A2_WHOAMI                        (0x0C)

#define MPL3115A2_PT_DATA_CFG                    0x13
#define MPL3115A2_PT_DATA_CFG_TDEFE   0x01
#define MPL3115A2_PT_DATA_CFG_PDEFE   0x02
#define MPL3115A2_PT_DATA_CFG_DREM    0x04

#define MPL3115A2_CTRL_REG1                     (0x26)
#define MPL3115A2_CTRL_REG1_SBYB   0x01
#define MPL3115A2_CTRL_REG1_OST    0x02
#define MPL3115A2_CTRL_REG1_RST    0x04
#define MPL3115A2_CTRL_REG1_OS1    0x00
#define MPL3115A2_CTRL_REG1_OS2    0x08
#define MPL3115A2_CTRL_REG1_OS4    0x10
#define MPL3115A2_CTRL_REG1_OS8    0x18
#define MPL3115A2_CTRL_REG1_OS16   0x20
#define MPL3115A2_CTRL_REG1_OS32   0x28
#define MPL3115A2_CTRL_REG1_OS64   0x30
#define MPL3115A2_CTRL_REG1_OS128  0x38
#define MPL3115A2_CTRL_REG1_RAW    0x40
#define MPL3115A2_CTRL_REG1_ALT    0x80
#define MPL3115A2_CTRL_REG1_BAR    0x00

#define MPL3115A2_CTRL_REG2                     (0x27)

#define MPL3115A2_CTRL_REG3                     (0x28)

#define MPL3115A2_CTRL_REG4                     (0x29)

#define MPL3115A2_CTRL_REG5                     (0x2A)

#define MPL3115A2_REGISTER_STARTCONVERSION      (0x12)

uint8_t read8(uint8_t a) {
  Wire.beginTransmission(MPL3115A2_ADDRESS); 
  Wire.write(a);
  Wire.endTransmission(false);
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)1);
  uint8_t valueRead = Wire.read();
  return valueRead;
}

void write8(uint8_t a, uint8_t d) {
  Wire.beginTransmission(MPL3115A2_ADDRESS); 
  Wire.write(a);
  Wire.write(d);
  Wire.endTransmission(false); 
}

float getTemperature() {
  uint8_t sta = 0;
  while (! (sta & MPL3115A2_REGISTER_STATUS_TDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  Wire.beginTransmission(MPL3115A2_ADDRESS);  
  Wire.write(MPL3115A2_REGISTER_TEMP_MSB); 
  Wire.endTransmission(false); 
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)2);
  uint16_t t = Wire.read(); 
  t <<= 8;
  t |= Wire.read(); 
  t >>= 4;

  float temp = t;
  temp /= 16.0;
  return temp;
}

float getPressure() {
  write8(MPL3115A2_CTRL_REG1, 
   MPL3115A2_CTRL_REG1_SBYB |
   MPL3115A2_CTRL_REG1_OS128 |
   MPL3115A2_CTRL_REG1_BAR);

  uint8_t sta = 0;
  while (! (sta & MPL3115A2_REGISTER_STATUS_PDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  Wire.beginTransmission(MPL3115A2_ADDRESS);  
  Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB); 
  Wire.endTransmission(false); 
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)3);
  uint32_t pressure = Wire.read(); 
  pressure <<= 8;
  pressure |= Wire.read(); 
  pressure <<= 8;
  pressure |= Wire.read(); 
  pressure >>= 4;

  float baro = pressure;
  baro /= 4.0;
  return baro;
}

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
HTU21D myHumidity;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup() ...");
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);

  myHumidity.begin();
  
  Wire.begin();
  uint8_t whoami = read8(MPL3115A2_WHOAMI);
  if (whoami == 0xC4) {
    write8(MPL3115A2_CTRL_REG1, 
      MPL3115A2_CTRL_REG1_SBYB |
      MPL3115A2_CTRL_REG1_OS128 |
      MPL3115A2_CTRL_REG1_ALT);
    write8(MPL3115A2_PT_DATA_CFG, 
      MPL3115A2_PT_DATA_CFG_TDEFE |
      MPL3115A2_PT_DATA_CFG_PDEFE |
      MPL3115A2_PT_DATA_CFG_DREM);
    write8(MPL3115A2_CTRL_REG2, 0x00);
  } else {
    Serial.println(" *** ERROR couldn't find MPL3115A2 ***");
  }
  Serial.println("... setup() finished");
}

void loop() {
  float pressure = getPressure() / 1000.0;
  Serial.print("Pressure is ");
  Serial.print(pressure);
  Serial.println("kPa");
  float temperature = getTemperature();
  Serial.print("Temperature is ");
  Serial.print(temperature);
  Serial.println("C");

  float humdity = myHumidity.readHumidity();
  float temperature2 = myHumidity.readTemperature();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(temperature);
  lcd.print("C (");
  lcd.print(temperature2);
  lcd.print(")");
  
  lcd.setCursor(0,1);
  lcd.print(pressure);
  lcd.print("kPa ");
  lcd.print(humdity);
  lcd.print("%");
  
  delay(500);
}

