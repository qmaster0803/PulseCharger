#include <Wire.h>
#include <LiquidCrystal_I2C.h>

byte x = 1;
byte state = 1;
LiquidCrystal_I2C lcd(0x26, 16, 2);

void setup()
{
  lcd.init();
  lcd.backlight();
  Wire.begin();
}

void loop()
{
  writeOutput(0x00, 0x01);
  writeOutput(0x01, x);
  writeOutput(0x02, x);
  x = x + state;

  if (x <= 0x01 || x >= 0xFF)
  {
    state = -state;
    writeOutput(0x00, 0x00);
    delay(3000);
    writeOutput(0x00, 0x01);
  }
  delay(50);
  lcd.setCursor(0, 0);
  if(x < 10) lcd.print("00");
  if(x >= 10 && x < 100) lcd.print("0");
  lcd.print(x);
}

void writeOutput(byte command, byte value)
{
  Wire.beginTransmission(8);
  Wire.write(command);
  Wire.write(value);
  Wire.endTransmission();
}
