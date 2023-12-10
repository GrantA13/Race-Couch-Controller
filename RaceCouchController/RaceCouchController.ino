/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>
#include <math.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

int x;
int y;
double angle;
double anglePercent;
double rightOut;
double leftOut;
double out;
int input = 9;
int val = 0;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, INPUT);
  
}
void loop() {
  Usb.Task();
  analogWrite(5, 255);
  if (PS4.connected()) {

    if (PS4.getAnalogButton(R2)) {
      out = PS4.getAnalogButton(R2);
    }else{
      out = 0;
    }  

    x = PS4.getAnalogHat(LeftHatX) - 128;
    y = 126 - PS4.getAnalogHat(LeftHatY);
    angle = atan2(y, x);
    anglePercent = abs(angle / (M_PI/2));
    
    if (angle > 0){
      digitalWrite(5, LOW);
      digitalWrite(3, LOW);
      if (anglePercent < 1){
        leftOut = PS4.getAnalogButton(R2);
        rightOut = PS4.getAnalogButton(R2) * anglePercent;
      }else if(anglePercent > 1){
        rightOut = PS4.getAnalogButton(R2);
        leftOut = PS4.getAnalogButton(R2) * (1-(anglePercent-1));
      }
    }else if (angle < 0){
      digitalWrite(5, HIGH);
      digitalWrite(3, HIGH);
      if (anglePercent < 1){
        leftOut = PS4.getAnalogButton(R2);
        rightOut = PS4.getAnalogButton(R2) * anglePercent;
      }else if(anglePercent > 1){
        rightOut = PS4.getAnalogButton(R2);
        leftOut = PS4.getAnalogButton(R2) * (1-(anglePercent-1));
      }
    }

    // }else if(anglePercent > ){

    // }

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
    }

    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS4.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));
      Serial.print(F("\tAngle: "));
      Serial.print(angle);
      Serial.print(F("\tAngle Percent: "));
      Serial.print(anglePercent);
      Serial.print(F("\tLeft Out: "));
      Serial.print(leftOut);
      Serial.print(F("\tRight Out: "));
      Serial.print(rightOut);
      Serial.print(F("\tReversing: "));
      Serial.print(digitalRead(4));
    }
    
    analogWrite(8, leftOut);
    analogWrite(10, rightOut);
    // val = digitalRead(input);
    // Serial.println(val);
    // delay(val);
  }
}
