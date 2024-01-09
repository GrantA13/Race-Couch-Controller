/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4USB.h>
#include <math.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

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
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, INPUT);
  
}
void loop() {
  Usb.Task();
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
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
      if (anglePercent < 1){
        leftOut = out;
        rightOut = out * anglePercent;
      }else if(anglePercent > 1){
        rightOut = out;
        leftOut = out * (1-(anglePercent-1));
      }
    }else if (angle < 0){
      if (anglePercent < 1){
        leftOut = out;
        rightOut = out * anglePercent;
      }else if(anglePercent > 1){
        rightOut = out;
        leftOut = out * (1-(anglePercent-1));
      }
    }
    // if (PS4.getButtonClick(CROSS)) {
    //   buttonPressed = !buttonPressed;
    //   digitalWrite(13, HIGH);
    //   digitalWrite(11, HIGH);
    // }

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
      Serial.print(digitalRead(13));
    }
    if(leftOut == 0){
      digitalWrite(5, LOW);
    }else if(leftOut == 255){
      digitalWrite(5, HIGH);
    }else{
      analogWrite(5, leftOut);
    }
    
    if(rightOut == 0){
      digitalWrite(3, LOW);
    }else if(rightOut == 255){
      digitalWrite(3, HIGH);
    }else{
      analogWrite(3, rightOut);
    }
    // val = digitalRead(input);
    // Serial.println(val);
    // delay(val);
  }
}
