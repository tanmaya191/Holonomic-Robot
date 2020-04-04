#include <PS4BT.h>
#include <usbhub.h>


#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb);

PS4BT PS4(&Btd, PAIR);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

double x = 0, y = 0, w = 0;
double s1, s2, s3, s4;
double lx, ly, rx, ry;

int d1 = 29, d2 = 27, d3 = 25, d4 = 23;
int m1 = 5, m2 = 4, m3 = 3, m4 = 2;

float sf = 1;
float sf_yaw = 1;

bool dir1 = LOW, dir2, dir3 = LOW, dir4;

void setup()
{
   motorSetup();
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
 
}

void motorSetup()
{
  pinMode(m1, OUTPUT); pinMode(d1, OUTPUT);
  pinMode(m2, OUTPUT); pinMode(d2, OUTPUT);
  pinMode(m3, OUTPUT); pinMode(d3, OUTPUT);
  pinMode(m4, OUTPUT); pinMode(d4, OUTPUT);
}

void loop()
{

  Usb.Task();
  PS4.setLed(Red);

  if (PS4.connected())
  {
    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117 || PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2))
    {
      x = 2 * (PS4.getAnalogHat(RightHatX) - 128);
      y = 2 * (PS4.getAnalogHat(LeftHatY) - 128);
      w = PS4.getAnalogButton(R2) ? -PS4.getAnalogButton(R2) / sf_yaw : 0;
      w = PS4.getAnalogButton(L2) ? PS4.getAnalogButton(L2) / sf_yaw : w;
      if (w >= -10 && w <= 10)  w = 0;
      if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
        PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
      oldL2Value = PS4.getAnalogButton(L2);
      oldR2Value = PS4.getAnalogButton(R2);

      if (x < 20 && x > -20)  x = 0;
      if (y < 20 && y > -20)  y = 0;
    }
    else
    {
      x = 0;
      y = 0;
      w = 0;
    }
    s1 =  sf * (-0.5 * x + 0 * y + 0.25 * w);
    s2 =  sf * (0 * x + -0.5 * y + 0.25 * w);
    s3 =  sf * (0.5 * x + 0 * y + 0.25 * w);
    s4 =  sf * (0 * x + 0.5 * y + 0.25 * w);

    //      s1 =  sf * (-0.6628433 * x - 0.0990626 * y + 0.3330024 * w);
    //      s2 =  sf * (0.4263029 * x - 0.517146 * y + 0.3300284 * w);
    //      s3 =  sf * (0.2365404 * x + 0.6162044 * y + 0.339504 * w);

    dir1 = s1 >= 0 ? 0 : 1;
    dir2 = s2 >= 0 ? 0 : 1;
    dir3 = s3 >= 0 ? 0 : 1;
    dir4 =  s4 >= 0 ? 0 : 1;

    digitalWrite(d1, dir1);
    digitalWrite(d2, dir2);
    digitalWrite(d3, dir3);
    digitalWrite(d4, dir4);

    analogWrite(m1, abs(s1));
    analogWrite(m2, abs(s2));
    analogWrite(m3, abs(s3));
    analogWrite(m4, abs(s4));



  }
  else
  {
    //Serial.print("Disconnect");
    analogWrite(m1, 0);
    analogWrite(m2, 0);
    analogWrite(m3, 0);
    analogWrite(m4, 0);
  }



  if (PS4.getButtonClick(UP))
  {
    sf += 0.5;
  }

  if (PS4.getButtonClick(DOWN))
  {
    sf -= 0.5;
  }
  if (PS4.getButtonClick(RIGHT))
  {
    sf_yaw += 0.5;
  }
  if (PS4.getButtonClick(LEFT))
  {
    sf_yaw -= 0.5;
  }
  sf = (sf > 1.5) ? 2 : sf;
  sf = (sf < 0) ? 0 : sf;



//  Serial.print("x= ");
//  Serial.print(x);
//  Serial.print("  y= ");
//  Serial.print(y);
//  Serial.print("  w= ");
//  Serial.print(w);
//
//  Serial.print("\t\t");
//  Serial.print("S1= ");
//  Serial.print(s1);
//  Serial.print("  S2= ");
//  Serial.print(s2);
//  Serial.print("  S3= ");
//  Serial.print(s3);
//  Serial.print("  S4= ");
//  Serial.print(s4);
//
//  Serial.print("\t\t");
//  Serial.print("D1= ");
//  Serial.print(dir1);
//  Serial.print("  D2= ");
//  Serial.print(dir2);
//  Serial.print("  D3= ");
//  Serial.println(dir3);
//  Serial.print("  D4= ");
//  Serial.println(dir4);
}
