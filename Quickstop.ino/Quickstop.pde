// Quickstop.pde
// -*- mode: C++ -*-
//
// Check stop handling.
// Calls stop() while the stepper is travelling at full speed, causing
// the stepper to stop as quickly as possible, within the constraints of the
// current acceleration.
//
// Copyright (C) 2012 Mike McCauley
// $Id:  $

#include <AccelStepper.h>
const int dirPin = 2;
const int stepPin = 0;
const int enable=4;
const int led_forward=5;
const int led_back=14;

#define motorInterfaceType 1

// Define a stepper and the pins it will use
AccelStepper stepper(motorInterfaceType, stepPin, dirPin); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

int spd = 1000;    // The current speed in steps/second
int sign = 1;      // Either 1, 0 or -1

int buttonState = 0;


void setup()
{
  Serial.begin(9600);
  stepper.setMaxSpeed(30000);
  stepper.setSpeed(10000);
  stepper.setAcceleration(500);
  stepper.moveTo(50000);
  
}

void loop()
{
  
  char c;
  if (Serial.available()) {
    c = Serial.read();
    if (c == 'f') {  // forward
      sign = 1;
    }
    if (c == 'r') {  // reverse
      sign = -1;
    }
    if (c == 's') {  // stop
      sign = 0;
    }
    if (c == '1') {  // super slow
      spd = 100;
    }
    if (c == '2') {  // medium
      spd = 900;
    }
    if (c == '3') {  // fast
      spd = 30000;
    }
    if (c == '4') {
      //not working, does anyone know how to do this?
      stepper.moveTo(500);
    }
    stepper.setSpeed(sign * spd);
  }
  stepper.run(); //You need to call stepper.run() instead of runSpeed, for moveTo to work. – Gerben
}