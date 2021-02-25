#include <TrueRMS.h>
#include <avdweb_AnalogReadFast.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define UPDATE_PERIOD 1000

byte x = 1;
byte state = 1;
int loopCount = 0;

Average Ch1Vol;
Average Ch1Cur;
LiquidCrystal_I2C lcd(0x26, 16, 2);

void setup()
{
  lcd.init();
  lcd.backlight();
  Wire.begin();
}

void loop()
{
  unsigned long loopTime = micros()+UPDATE_PERIOD;
  doMeasure();
  loopCount++;

  if(loopCount >= 200)
  {
    //REGULATOR HERE
    writeCommand(0x00, 0x01);
    writeCommand(0x01, x);
    writeCommand(0x02, x);
    x = x + state;
  
    if (x <= 0x01 || x >= 0xFF)
    {
      state = -state;
      writeCommand(0x00, 0x00);
      delay(3000);
      writeCommand(0x00, 0x01);
    }
    lcd.setCursor(0, 0);
    if(x < 10) lcd.print("00");
    if(x >= 10 && x < 100) lcd.print("0");
    lcd.print(x);
    loop_count = 0;
  }
  while(loopTime > micros());
}

void doMeasure()
{
  adcVol = 1021-analogReadFast(A3); // read the ADC
  adcCur = abs(1023-analogReadFast(A2)-adcVol*2);
  Ch1Vol.update(adcVol);
  Ch1Cur.update(adcCur);
}

void setOutput(bool enabled, byte on_period, byte off_period)
{
  if(enabled) writeCommand(0x00, 0x00);
  else        writeCommand(0x00, 0x01);
  writeCommand(0x01, on_period);
  writeCommand(0x02, off_period);
}

void writeCommand(byte command, byte value)
{
  Wire.beginTransmission(8);
  Wire.write(command);
  Wire.write(value);
  Wire.endTransmission();
}
