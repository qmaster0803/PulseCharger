#include <Wire.h>

bool output_enabled = false;
long on_period = 200;
long off_period =  200;

void setup()
{
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  pinMode(6, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if(output_enabled)
  {
    digitalWrite(6, HIGH);
    digitalWrite(13, HIGH);
    if(on_period < 15000) delayMicroseconds(on_period);
    else delay(on_period/1000);
    digitalWrite(6, LOW);
    digitalWrite(13, LOW);
    if(off_period < 15000) delayMicroseconds(off_period);
    else delay(off_period/1000);
  }
}

void receiveEvent(int c)
{
  byte i = 0;
  byte buff[5] = {0, 0};
  while(Wire.available() > 0)
  {
    buff[i] = Wire.read();
    i++;
  }
  if(buff[0] == 0x00) output_enabled = buff[1];
  if(buff[0] == 0x01) on_period = buff[1]*100;
  if(buff[0] == 0x02) off_period = buff[1]*100;
}
